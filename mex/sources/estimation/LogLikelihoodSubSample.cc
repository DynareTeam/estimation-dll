/*
 * Copyright (C) 2009-2011 Dynare Team
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
//  LogLikelihoodSubSample.cpp
//  Implementation of the Class LogLikelihoodSubSample
//  Created on:      14-Jan-2010 22:39:14
///////////////////////////////////////////////////////////

//#include "LogLikelihoodSubSample.hh"
#include "LogLikelihoodMain.hh" // use ...Main.hh for testing only
#include <algorithm>
#include "LapackBindings.hh"

LogLikelihoodSubSample::~LogLikelihoodSubSample()
{
};

LogLikelihoodSubSample::LogLikelihoodSubSample(const std::string &dynamicDllFile, EstimatedParametersDescription &INestiParDesc, size_t n_endo, size_t n_exo,
                                               const std::vector<size_t> &zeta_fwrd_arg, const std::vector<size_t> &zeta_back_arg,
                                               const std::vector<size_t> &zeta_mixed_arg, const std::vector<size_t> &zeta_static_arg, const double qz_criterium,
                                               const std::vector<size_t> &varobs, double riccati_tol, double lyapunov_tol, int &INinfo) :
  startPenalty(-1e8), estiParDesc(INestiParDesc),
  kalmanFilter(dynamicDllFile, n_endo, n_exo, zeta_fwrd_arg, zeta_back_arg, zeta_mixed_arg, zeta_static_arg, qz_criterium,
               varobs, riccati_tol, lyapunov_tol, INinfo), eigQ(n_exo), eigH(varobs.size()), info(INinfo)
{
};

double
LogLikelihoodSubSample::compute(VectorView &steadyState, const MatrixConstView &dataView, const Vector &estParams, Vector &deepParams,
                                Matrix &Q, Matrix &H, VectorView &vll, MatrixView &detrendedDataView, int &info, size_t start, size_t period)
{
  penalty = startPenalty;
  logLikelihood = startPenalty;

  updateParams(estParams, deepParams, Q, H, period);
  if (info == 0)
    logLikelihood = kalmanFilter.compute(dataView, steadyState,  Q, H, deepParams, vll, detrendedDataView, start, period, penalty,  info);
  //  else
  //    logLikelihood+=penalty;

  return logLikelihood;

};

void
LogLikelihoodSubSample::updateParams(const Vector &estParams, Vector &deepParams,
                                     Matrix &Q, Matrix &H, size_t period)
{
  size_t i, k, k1, k2;
  int test;
  bool found;
  std::vector<size_t>::const_iterator it;
  info = 0;

  for (i = 0; i <  estParams.getSize(); ++i)
    {
      found = false;
      it = find(estiParDesc.estParams[i].subSampleIDs.begin(),
                estiParDesc.estParams[i].subSampleIDs.end(), period);
      if (it != estiParDesc.estParams[i].subSampleIDs.end())
        found = true;
      if (found)
        {
          switch (estiParDesc.estParams[i].ptype)
            {
            case EstimatedParameter::shock_SD:
              k = estiParDesc.estParams[i].ID1;
              Q(k, k) = estParams(i)*estParams(i);
              break;

            case EstimatedParameter::measureErr_SD:
              k = estiParDesc.estParams[i].ID1;
              H(k, k) = estParams(i)*estParams(i);
              break;

            case EstimatedParameter::shock_Corr:
              k1 = estiParDesc.estParams[i].ID1;
              k2 = estiParDesc.estParams[i].ID2;
              Q(k1, k2) = estParams(i)*sqrt(Q(k1, k1)*Q(k2, k2));
              Q(k2, k1) = Q(k1, k2);
              //   [CholQ,testQ] = chol(Q);
              test = lapack::choleskyDecomp(Q, "L");
              if (test > 0)
                {
                  mexPrintf("Caugth unhandled exception with cholesky of Q matrix: ");
                  logLikelihood = penalty;
                  info = 1;
                }
              else if (test < 0)
                {
                  // The variance-covariance matrix of the structural innovations is not definite positive.
                  // We have to compute the eigenvalues of this matrix in order to build the penalty.
                  double delta = 0;
                  eigQ.calculate(Q);  // get eigenvalues
                  //k = find(a < 0);
                  if (eigQ.hasConverged())
                    {
                      const Vector &evQ = eigQ.getD();
                      for (i = 0; i < evQ.getSize(); ++i)
                        if (evQ(i) < 0)
                          delta -= evQ(i);
                    }

                  logLikelihood = penalty+delta;
                  info = 43;
                } // if
              break;

            case EstimatedParameter::measureErr_Corr:
              k1 = estiParDesc.estParams[i].ID1;
              k2 = estiParDesc.estParams[i].ID2;
              //      H(k1,k2) = xparam1(i)*sqrt(H(k1,k1)*H(k2,k2));
              //      H(k2,k1) = H(k1,k2);
              H(k1, k2) = estParams(i)*sqrt(H(k1, k1)*H(k2, k2));
              H(k2, k1) = H(k1, k2);

              //[CholH,testH] = chol(H);
              test = lapack::choleskyDecomp(H, "L");
              if (test > 0)
                {
                  mexPrintf("Caugth unhandled exception with cholesky of Q matrix: ");
                  logLikelihood = penalty;
                  info = 1;
                }
              else if (test < 0)
                {
                  // The variance-covariance matrix of the measurement errors is not definite positive.
                  // We have to compute the eigenvalues of this matrix in order to build the penalty.
                  //a = diag(eig(H));
                  double delta = 0;
                  eigH.calculate(H);  // get eigenvalues
                  //k = find(a < 0);
                  if (eigH.hasConverged())
                    {
                      const Vector &evH = eigH.getD();
                      for (i = 0; i < evH.getSize(); ++i)
                        if (evH(i) < 0)
                          delta -= evH(i);
                    }
                  logLikelihood = penalty+delta;
                  info = 44;
                } //   end if
              break;

              //if estim_params_.np > 0  // i.e. num of deep parameters >0
            case EstimatedParameter::deepPar:
              k = estiParDesc.estParams[i].ID1;
              deepParams(k) = estParams(i);
              break;
            default:
              logLikelihood = penalty;
              info = 1;
            } // end switch
        } // end found
    } //end for
};

