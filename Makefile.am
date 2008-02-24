SUBDIRS = hangul data tools test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhangul.pc

log:
	unset LC_ALL; \
	export LANG=C ; \
	export LC_CTYPE=ko_KR.UTF-8; \
	svn log -v > ChangeLog
