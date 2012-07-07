
AC_PREREQ([2.64])
AC_INIT([Firestarter], [0.1], [sebastian.lauwers@gmail.com], [firestarter], [http://firestarter.example.com])
AM_INIT_AUTOMAKE([1.11 no-define subdir-objects color-tests parallel-tests])
AM_SILENT_RULES([yes])

LT_PREREQ([2.2])
LT_INIT([dlopen])
AC_LANG(C++)

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for header files
AC_CHECK_HEADERS([stdlib.h ltdl.h])

# Checks for library functions.
AC_SEARCH_LIBS([lt_dlinit], [ltdl])

AX_CHECK_COMPILE_FLAG([-std=gnu++11], [CXXFLAGS="$CXXFLAGS -std=gnu++11"], [
	AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="CXXFLAGS -std=c++11"], [
		AC_MSG_ERROR([Compiler needs to support either gnu++11 or c++11 (we hear clang++ works really well!)])
	])
])

# Check if the boost libraries are available
# Boost 1.48 is required as we're trying to compile with -std=gnu++11
BOOST_REQUIRE([1.48])
# you can check for specific parts of the boost library with:
# BOOST_ARRAY
BOOST_TEST
BOOST_TUPLE
BOOST_GRAPH
BOOST_STRING_ALGO
BOOST_THREADS
BOOST_DATE_TIME

PKG_CHECK_MODULES([DEPS], [liblog4cxx >= 0.10 libconfig++ >= 1.3.2 libzmq >= 3.0 protobuf >= 2.3.0])
PKG_CHECK_MODULES([WEBDEPS], [fastcgi++ >= 2.0 libctemplate >= 2.2])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
