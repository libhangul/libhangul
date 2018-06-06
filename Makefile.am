SUBDIRS = hangul data tools po
DIST_SUBDIRS = hangul data tools po test doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhangul.pc

EXTRA_DIST = \
    config.rpath \
    doc/Makefile \
    doc/Doxyfile.in \
    doc/mainpage.dox \
    test/Makefile.am \
    test/Makefile.in \
    test/hangul.c \
    test/hanja.c \
    test/test.c

ACLOCAL_AMFLAGS = -I m4

dist-hook:
	if test -d .git; then \
		git log --name-status --date=iso > $(distdir)/ChangeLog ; \
	fi
