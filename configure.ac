#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(libhangul, 0.0.3, http://kldp.net/projects/hangul/)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([hangul/hangul.h])
AC_CONFIG_HEADER([config.h])

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
