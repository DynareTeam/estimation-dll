SUBDIRS = mex/sources

if ENABLE_MATLAB
SUBDIRS += mex/build/matlab
endif
if ENABLE_OCTAVE
SUBDIRS += mex/build/octave
endif

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	license.txt \
	README.md
