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
AM_INIT_AUTOMAKE([1.11 -Wall foreign])

AC_PROG_CC
AC_PROG_CXX

AC_CANONICAL_HOST
case ${host_os} in
  *mingw32*)
    # On mingw32, we don't want dynamic libgcc
    # Note that static-libstdc++ is only supported since GCC 4.5 (but generates no error on older versions)
    LDFLAGS="$LDFLAGS -static-libgcc -static-libstdc++ -static-libgfortran"
    have_windows="yes"
    ;;
  *cygwin*)
    AC_MSG_WARN([You are compiling for the Cygwin target. This means that the preprocessor will])
    AC_MSG_WARN([not run from MATLAB unless you add the Cygwin DLL to the path.])
    AC_MSG_WARN([This is probably not what you want. Consider using a MinGW cross-compiler.])
    if test "x$F77" = "x"; then
      # On Cygwin 1.7, g77 comes has version 3, and is not compatible with default gcc/g++ which has version 4
      # And by default, the AC_PROG_F77 will pick up g77 if it is present (even if gfortran is also here)
      F77=gfortran
    fi
    have_windows="yes"
    ;;
esac


# Use C++ for testing headers
AC_LANG([C++])

CXXFLAGS="$CXXFLAGS -Wall -Wno-parentheses"

# Check for dlopen(), needed by tests for estimation DLL
AC_CHECK_LIB([dl], [dlopen], [LIBADD_DLOPEN="-ldl"], [])
AC_SUBST([LIBADD_DLOPEN])

# We need 1.36 because of unordered_{set,hash} used by Dynare++
AX_BOOST_BASE([1.36], [], [AC_MSG_ERROR([Can't find Boost >= 1.36])])

CPPFLAGS_SAVED="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
AC_CHECK_HEADERS([boost/graph/adjacency_list.hpp], [], [AC_MSG_ERROR([Can't find Boost Graph Library])])
AC_CHECK_HEADERS([boost/algorithm/string/trim.hpp], [], [AC_MSG_ERROR([Can't find Boost String Library])])
AC_CHECK_HEADERS([boost/algorithm/string/split.hpp], [], [AC_MSG_ERROR([Can't find Boost String Library])])
AC_CHECK_HEADERS([boost/lexical_cast.hpp], [], [AC_MSG_ERROR([Can't find Boost Lexical Cast Header])])
CPPFLAGS="$CPPFLAGS_SAVED"


AC_PROG_F77
AC_F77_LIBRARY_LDFLAGS
case ${host_os} in
  *mingw32*)
    # Hack to get static linking of libgfortran on MinGW
    # (-static-libgfortran doesn't act on gcc/g++)
    FLIBS=`echo $FLIBS | sed 's/-lgfortran/-Wl,-Bstatic -lgfortran -Wl,-Bdynamic/'`
    ;;
  *darwin*)
    # * OS X doesn't need to link against the Fortran libraries because
    #   everything is contained within the vecLib framework.
    FLIBS=''
    ;;
esac

if test "x$F77" != "x"; then
   AX_BLAS
   AX_LAPACK
fi
AM_CONDITIONAL([HAVE_BLAS], [test x"$ax_blas_ok" = "xyes"])
AM_CONDITIONAL([HAVE_LAPACK], [test x"$ax_lapack_ok" = "xyes"])

AC_CONFIG_FILES([Makefile
                 mex/sources/Makefile
                 mex/sources/estimation/Makefile
                 mex/sources/estimation/tests/Makefile
                 mex/sources/estimation/libmat/Makefile
                 mex/sources/estimation/libmat/tests/Makefile
])

AC_ARG_ENABLE([matlab], AS_HELP_STRING([--disable-matlab], [disable compilation of MEX files for MATLAB]), [], [enable_matlab=yes])
if test "x$enable_matlab" = "xyes"; then
  AC_CONFIG_SUBDIRS([mex/build/matlab])
  AX_MATLAB
fi
AM_CONDITIONAL([ENABLE_MATLAB], [test "x$enable_matlab" = "xyes"])
AM_CONDITIONAL([HAVE_CMD_LINE_MATLAB], [test "x$ax_enable_matlab" = "xyes" -a "x$have_windows" = "x"])

AC_ARG_ENABLE([octave], AS_HELP_STRING([--disable-octave], [disable compilation of MEX files for Octave]), [], [enable_octave=yes])
if test "x$enable_octave" = "xyes"; then
  AC_CONFIG_SUBDIRS([mex/build/octave])
  AC_CHECK_PROG([OCTAVE], [octave], [octave])
fi
AM_CONDITIONAL([ENABLE_OCTAVE], [test "x$enable_octave" = "xyes"])
AM_CONDITIONAL([HAVE_OCTAVE], [test "x$enable_octave" = "xyes" -a "x$OCTAVE" != "x"])

AC_OUTPUT
