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

# Target board selection
AC_ARG_ENABLE(rpi,
	AS_HELP_STRING([--enable-rpi], "Configure for original Raspberry Pi (BCM2835-based)"),
        [target_board=rpi,
	 AC_DEFINE([TARGET_BOARD_RPI], [1], [Target board is original RPi])
	])

AC_ARG_ENABLE(rpi2,
	AS_HELP_STRING([--enable-rpi2], "Configure for Raspberry Pi 2 (BCM2836-based)"),
	[target_board=rpi2,
	 AC_DEFINE([TARGET_BOARD_RPI2], [1], [Target board is RPi 2])
	])


AS_IF([test "x$target_board" != x],
         [],
         AC_MSG_FAILURE("Target board (RPi or RPi2) must be selected. Use --enable-xx option.")
     )

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
