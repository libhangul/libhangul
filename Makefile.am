SUBDIRS = hangul data bindings test

DISTCLEANFILES = hangulconfig.h

hangulconfdir = $(includedir)/hangul-1.0
hangulconf_DATA = hangulconfig.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhangul.pc

hangulconfig.h: config.status
	$(top_builddir)/config.status hangulconfig.h

log:
	svn log -v > ChangeLog
