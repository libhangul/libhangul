SUBDIRS = hangul data test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhangul.pc

EXTRA_DIST = \
    doc/Makefile \
    doc/Doxyfile.in \
    doc/mainpage.dox

log:
	unset LC_ALL; \
	export LANG=C ; \
	export LC_CTYPE=ko_KR.UTF-8; \
	svn log -v > ChangeLog
