#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(libhangul, 0.0.1, http://kldp.net/projects/hangul/)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([hangul/hangul.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

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
test/Makefile
])
AC_OUTPUT
