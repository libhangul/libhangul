
libhangulincludedir = $(includedir)/hangul-1.0
libhangulinclude_HEADERS = hangul.h

lib_LTLIBRARIES = libhangul.la
noinst_HEADERS = \
	hangul-gettext.h \
	hangulkeyboard.h \
	hangulinternals.h \
	hanjacompatible.h

libhangul_la_SOURCES = \
	hangulctype.c \
	hangulinputcontext.c \
	hangulkeyboard.c \
	hanja.c

libhangul_la_CFLAGS = \
	-DLOCALEDIR=\"$(localedir)\" \
	-DLIBHANGUL_DEFAULT_HANJA_DIC=\"$(pkgdatadir)/hanja/hanja.txt\" \
	-DLIBHANGUL_DATA_DIR=\"$(pkgdatadir)\" \
	-DTOP_SRCDIR=\"$(abs_top_srcdir)\" \
	$(EXPAT_CFLAGS) \
	$(NULL)

libhangul_la_LDFLAGS = -version-info $(LIBHANGUL_CURRENT):$(LIBHANGUL_REVISION):$(LIBHANGUL_AGE)
libhangul_la_LIBADD = $(EXPAT_LIBS)
