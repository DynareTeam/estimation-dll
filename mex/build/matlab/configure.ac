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

dnl The following must occur before we modify CFLAGS/CXXFLAGS
AC_CANONICAL_HOST

AX_MATLAB

if test "x$ax_enable_matlab" = "xyes"; then
  AX_MATLAB_VERSION
  AX_DOT_MEXEXT
  AX_MATLAB_ARCH

  if test "x$ax_matlab_version_ok" = "xyes"; then
    AX_MEXOPTS
  fi

  CFLAGS="$MATLAB_CFLAGS"
  CXXFLAGS="$MATLAB_CXXFLAGS"
  FFLAGS="$MATLAB_FFLAGS"
fi

case ${host_os} in
  *cygwin*)
    AC_MSG_WARN([You are compiling for the Cygwin target. This means that the MEX files will])
    AC_MSG_WARN([not run from MATLAB, because it cannot load the Cygwin DLL.])
    AC_MSG_WARN([This is probably not what you want. Consider using a MinGW cross-compiler.])
    ;;
  *darwin13*)
    CXXFLAGS="$CXXFLAGS -std=c++11"
    ;;
esac

CFLAGS="$CFLAGS -Wall -Wno-parentheses"
FFLAGS="$FFLAGS -Wall"
CXXFLAGS="$CXXFLAGS -Wall -Wno-parentheses"

AC_PROG_CC
AC_PROG_CXX
AX_PROG_LN_S

case ${host_os} in
  *mingw32*)
    # Ensure that -lpthread is statically linked under MinGW
    PTHREAD_LIBS="-Wl,-Bstatic -lpthread -Wl,-Bdynamic"
    ;;
esac
AX_PTHREAD

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

AM_CONDITIONAL([DO_SOMETHING], [test "x$ax_enable_matlab" = "xyes" -a "x$ax_matlab_version_ok" = "xyes" -a "x$ax_mexopts_ok" = "xyes"])

if test "x$ax_enable_matlab" = "xyes" -a "x$ax_matlab_version_ok" = "xyes" -a "x$ax_mexopts_ok" = "xyes" -a "x$has_gsl" = "xyes"; then
   BUILD_ESTIMATION_MEX_MATLAB="yes"
else
   BUILD_ESTIMATION_MEX_MATLAB="no (missing GSL)"
fi

AC_MSG_NOTICE([

 Will build Estimation MEX for MATLAB: $BUILD_ESTIMATION_MEX_MATLAB

])

AC_CONFIG_FILES([Makefile
                 estimation/Makefile
])

AC_OUTPUT
