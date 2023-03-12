#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(libhangul, 0.1.0, https://github.com/libhangul/libhangul)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([hangul/hangul.h])
AC_CONFIG_HEADER([config.h])

# Configure options
AC_ARG_ENABLE(external-keyboards,
    AS_HELP_STRING([--enable-external-keyboards],
                   [enable external keyboard xml file loading feature]),
    [enable_external_keyboards=$enableval],
    [enable_external_keyboards=yes]
)

# library version
LIBHANGUL_CURRENT=1
LIBHANGUL_REVISION=0
LIBHANGUL_AGE=0

AC_SUBST(LIBHANGUL_CURRENT)
AC_SUBST(LIBHANGUL_REVISION)
AC_SUBST(LIBHANGUL_AGE)

AM_MAINTAINER_MODE

AM_GNU_GETTEXT_VERSION([0.19.8])
AM_GNU_GETTEXT([external])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h limits.h])
AC_CHECK_HEADERS([langinfo.h])
AC_CHECK_HEADERS([glob.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
#AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([munmap])
AC_CHECK_FUNCS([strcasecmp])
AC_CHECK_FUNCS([nl_langinfo])

# Checks for gettext stuff
GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", gettext package name)
AM_ICONV
if test x$USE_NLS = xyes; then
    AC_DEFINE(ENABLE_NLS, 1, [Define to 1 if translation of program messages is requested.])
fi

# Checks for pkgconfig
PKG_PROG_PKG_CONFIG

# Checks for expat
if test x$enable_external_keyboards = xyes; then
    PKG_CHECK_MODULES(EXPAT, [expat])
    AC_DEFINE(ENABLE_EXTERNAL_KEYBOARDS, 1, [Define to 1 if you enabled to load external keyboards])
fi

# Checks for unit test framework
if test -n "$PKG_CONFIG"; then
    PKG_CHECK_EXISTS(check, [ PKG_CHECK_MODULES([CHECK], [check]) ])
fi

AC_CONFIG_FILES([
Makefile
data/Makefile
data/hanja/Makefile
data/keyboards/Makefile
doc/Doxyfile
doc/Makefile
hangul/Makefile
libhangul.pc
po/Makefile.in
test/Makefile
tools/Makefile
])

AC_CONFIG_LINKS([
data/keyboards/hangul-combination-default.xml:data/keyboards/hangul-combination-default.xml
data/keyboards/hangul-combination-full.xml:data/keyboards/hangul-combination-full.xml
])

AC_OUTPUT

# vim: et
