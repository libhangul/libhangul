#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(libhangul, 0.0.2, http://kldp.net/projects/hangul/)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([hangul/hangul.h])
AC_CONFIG_HEADER([config.h])

LIBHANGUL_MAJOR_VERSION=0
LIBHANGUL_MINOR_VERSION=0
LIBHANGUL_MICRO_VERSION=1

LIBHANGUL_VERSION=$LIBHANGUL_MAJOR_VERSION.$LIBHANGUL_MINOR_VERSION.$LIBHANGUL_MICRO_VERSION
AC_SUBST(LIBHANGUL_MAJOR_VERSION)
AC_SUBST(LIBHANGUL_MINOR_VERSION)
AC_SUBST(LIBHANGUL_MICRO_VERSION)
AC_SUBST(LIBHANGUL_VERSION)

# library version
LIBHANGUL_CURRENT=0
LIBHANGUL_REVISION=0
LIBHANGUL_AGE=0

AC_SUBST(LIBHANGUL_CURRENT)
AC_SUBST(LIBHANGUL_REVISION)
AC_SUBST(LIBHANGUL_AGE)

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdint.h stdlib.h string.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_CONFIG_FILES([
Makefile
hangul/Makefile
data/Makefile
data/hanja/Makefile
bindings/Makefile
test/Makefile
libhangul.pc
])

AC_OUTPUT
