ACLOCAL_AMFLAGS = -I ../../../m4

if DO_SOMETHING

if HAVE_GSL
if HAVE_MATIO
SUBDIRS = estimation
endif
endif

install-exec-local:
	$(MKDIR_P) $(DESTDIR)$(pkglibdir)/mex/octave

endif
