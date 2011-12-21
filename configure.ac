
AC_PREREQ([2.64])
AC_INIT([Firestarter], [0.1], [slauwers@gmail.com], [firestarter], [http://firestarter.example.com])
AM_INIT_AUTOMAKE([1.11 no-define subdir-objects color-tests parallel-tests])

LT_PREREQ([2.2])
LT_INIT([dlopen])

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for header files
AC_CHECK_HEADERS([stdlib.h ltdl.h])

# Checks for library functions.
AC_SEARCH_LIBS([lt_dlinit], [ltdl])

# Check if the boost libraries are available
BOOST_REQUIRE([1.42])
# you can check for specific parts of the boost library with:
# BOOST_ARRAY
BOOST_TEST

PKG_CHECK_MODULES([DEPS], [liblog4cxx >= 0.10 libconfig++ >= 1.3.2])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
