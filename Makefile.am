SUBDIRS = src
packextrasdir=@docdir@
EXTRA_DIST = 	COPYING INSTALL README.md NEWS ChangeLog AUTHORS	\
		example_output01.txt
packextras_DATA=$(EXTRA_DIST)

PACKAGE = @PACKAGE@
VERSION = @VERSION@

ACLOCAL_AMFLAGS = --install -I m4

distdir = $(PACKAGE)-$(VERSION)

all-local:
	cp $(top_builddir)/src/pokerhands $(top_builddir);

clean-local:
	if test -f $(top_builddir)/pokerhands; then	\
		rm $(top_builddir)/pokerhands;	\
	fi
