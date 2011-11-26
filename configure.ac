AC_INIT([Firestarter], [0.1], [slauwers@gmail.com], [firestarter], [http://firestarter.example.com])
AC_PREREQ([2.64])
AM_INIT_AUTOMAKE([1.11 -Wall no-define subdir-objects])

AC_CONFIG_HEADERS([config.h])

AC_PROG_LIBTOOL

AC_PROG_CXX

# Check if the boost libraries are available
BOOST_REQUIRE([1.42])
# you can check for specific parts of the boost library with:
# BOOST_ARRAY

PKG_CHECK_MODULES([DEPS], [liblog4cxx >= 0.10 libconfig++ >= 1.3.2])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
