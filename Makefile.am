SUBDIRS = hangul data tools po
DIST_SUBDIRS = hangul data tools po test doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhangul.pc

EXTRA_DIST = \
    CMakeLists.txt \
    config.h.cmake.in \
    config.rpath \
    data/hanja/CMakeLists.txt \
    data/keyboards/CMakeLists.txt \
    doc/Doxyfile.in \
    doc/Makefile \
    doc/mainpage.dox \
    hangul-config.cmake.in \
    hangul/CMakeLists.txt \
    po/CMakeLists.txt \
    test/CMakeLists.txt \
    test/Makefile.am \
    test/Makefile.in \
    test/hangul.c \
    test/hanja.c \
    test/test.c \
    tools/CMakeLists.txt \
    $(NULL)

ACLOCAL_AMFLAGS = -I m4

dist-hook:
	if test -d $(top_srcdir)/.git; then \
		git log --name-status --date=iso > $(distdir)/ChangeLog ; \
	fi
