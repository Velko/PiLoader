#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([hello.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_NO_EXECUTABLES
AC_PROG_CC
AC_CHECK_TOOL(AS, as, as)
AC_CHECK_TOOL(LD, ld, ld)
AC_CHECK_TOOL(OBJCOPY, objcopy, objcopy)
AC_CHECK_PROG(RM, rm, rm)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
