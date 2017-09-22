/*
 * Copyright (C) 2009-2010 Dynare Team
 *
 * This file is part of Dynare.
 *
 * Dynare is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dynare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dynare.  If not, see <http://www.gnu.org/licenses/>.
 */

///////////////////////////////////////////////////////////
//  LogLikelihoodMain.cpp
//  Implementation of the Class LogLikelihoodMain
//  Created on:      02-Feb-2010 12:57:09
///////////////////////////////////////////////////////////

#include "LogLikelihoodMain.hh"

LogLikelihoodMain::LogLikelihoodMain(const std::string &dynamicDllFile, EstimatedParametersDescription &estiParDesc, size_t n_endo, size_t n_exo,
                                     const std::vector<size_t> &zeta_fwrd_arg, const std::vector<size_t> &zeta_back_arg,
                                     const std::vector<size_t> &zeta_mixed_arg, const std::vector<size_t> &zeta_static_arg, const double qz_criterium,
                                     const std::vector<size_t> &varobs, double riccati_tol, double lyapunov_tol, int &info_arg)

  : estSubsamples(estiParDesc.estSubsamples),
    logLikelihoodSubSample(dynamicDllFile, estiParDesc, n_endo, n_exo, zeta_fwrd_arg, zeta_back_arg, zeta_mixed_arg, zeta_static_arg, qz_criterium,
                           varobs, riccati_tol, lyapunov_tol, info_arg),
    vll(estiParDesc.getNumberOfPeriods()), // time dimension size of data
    detrendedData(varobs.size(), estiParDesc.getNumberOfPeriods())
{

}

LogLikelihoodMain::~LogLikelihoodMain()
{

}

double
LogLikelihoodMain::compute(Matrix &steadyState, const Vector &estParams, Vector &deepParams, const MatrixConstView &data, Matrix &Q, Matrix &H, size_t start, int &info)
{
  double logLikelihood = 0;
  for (size_t i = 0; i < estSubsamples.size(); ++i)
    {
      VectorView vSteadyState = mat::get_col(steadyState, i);

      MatrixConstView dataView(data, 0, estSubsamples[i].startPeriod,
                               data.getRows(), estSubsamples[i].endPeriod-estSubsamples[i].startPeriod+1);
      MatrixView detrendedDataView(detrendedData, 0, estSubsamples[i].startPeriod,
                                   data.getRows(), estSubsamples[i].endPeriod-estSubsamples[i].startPeriod+1);

      VectorView vllView(vll, estSubsamples[i].startPeriod, estSubsamples[i].endPeriod-estSubsamples[i].startPeriod+1);
      logLikelihood += logLikelihoodSubSample.compute(vSteadyState, dataView, estParams, deepParams,
                                                      Q, H, vllView, detrendedDataView, info, start, i);
    }
  return logLikelihood;
};

