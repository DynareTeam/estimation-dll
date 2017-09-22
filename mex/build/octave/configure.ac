dnl Process this file with autoconf to produce a configure script.

dnl Copyright (C) 2017 Dynare Team
dnl
dnl This file is part of Dynare.
dnl
dnl Dynare is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl Dynare is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with Dynare.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.62])
AC_INIT([estimation], [1.0])
AC_CONFIG_SRCDIR([configure.ac])
AM_INIT_AUTOMAKE([-Wall -Wno-portability foreign])

AC_CHECK_PROG([MKOCTFILE], [mkoctfile], [mkoctfile])

if test "x$MKOCTFILE" != "x"; then
  CC=`$MKOCTFILE -p CC`
  CXX=`$MKOCTFILE -p CXX`
  CFLAGS=`$MKOCTFILE -p CFLAGS`
  FFLAGS=`$MKOCTFILE -p FFLAGS`
  CXXFLAGS=`$MKOCTFILE -p CXXFLAGS`
  LDFLAGS="`$MKOCTFILE -p LFLAGS` `$MKOCTFILE -p LDFLAGS`"
  OCTAVE_VERSION=`$MKOCTFILE -v 2>&1 | sed 's/mkoctfile, version //'`
  AX_COMPARE_VERSION([$OCTAVE_VERSION], [lt], [3.6], [AC_MSG_ERROR([Your Octave is too old, please upgrade to version 3.6 at least.])])
  AX_COMPARE_VERSION([$OCTAVE_VERSION], [ge], [3.8], [OCTAVE38=yes])
  AX_COMPARE_VERSION([$OCTAVE_VERSION], [ge], [4.0], [OCTAVE40=yes])
fi

CFLAGS="$CFLAGS -Wall -Wno-parentheses"
FFLAGS="$FFLAGS -Wall"
CXXFLAGS="$CXXFLAGS -Wall -Wno-parentheses"

AC_PROG_CC
AC_PROG_CXX
AX_PROG_LN_S

# Check for boost libraries used in estimation DLL
AC_LANG_PUSH([C++])
AX_BOOST_BASE([1.36], [], [AC_MSG_ERROR([Can't find Boost >= 1.36])])
CPPFLAGS_SAVED="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
AC_CHECK_HEADER([boost/random/linear_congruential.hpp], [], [AC_MSG_ERROR([Can't find Boost Random Library])])
AC_CHECK_HEADER([boost/math/distributions/beta.hpp], [], [AC_MSG_ERROR([Can't find Boost Math Library])])
CPPFLAGS="$CPPFLAGS_SAVED"
AC_LANG_POP([C++])

AC_CHECK_LIB([dl], [dlopen], [LIBADD_DLOPEN="-ldl"], [])
AC_SUBST([LIBADD_DLOPEN])

AX_GSL
AM_CONDITIONAL([HAVE_GSL], [test "x$has_gsl" = "xyes"])

AX_MATIO
AM_CONDITIONAL([HAVE_MATIO], [test "x$has_matio" = "xyes"])

AM_CONDITIONAL([DO_SOMETHING], [test "x$MKOCTFILE" != "x"])

if test "x$MKOCTFILE" != "x" -a "x$has_matio" = "xyes" -a "x$has_gsl" = "xyes" -a "x$has_matio" = "xyes"; then
  BUILD_ESTIMATION_MEX_OCTAVE="yes"
else
  BUILD_ESTIMATION_MEX_OCTAVE="no (missing MatIO library)"
fi

AC_MSG_NOTICE([

 Will build Estimation MEX for Octave: $BUILD_ESTIMATION_MEX_OCTAVE

])

AC_CONFIG_FILES([Makefile
                 estimation/Makefile
])

AC_OUTPUT
