SUBDIRS = hangul data test po

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhangul.pc

EXTRA_DIST = \
    config.rpath \
    m4/ChangeLog  \
    doc/Makefile \
    doc/Doxyfile.in \
    doc/mainpage.dox

log:
	unset LC_ALL; \
	export LANG=C ; \
	export LC_CTYPE=ko_KR.UTF-8; \
	svn log -v > ChangeLog

ACLOCAL_AMFLAGS = -I m4
