
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
AC_CHECK_HEADERS([stdlib.h ltdl.h Wt/WApplication Wt/WServer])

# Checks for library functions.
AC_SEARCH_LIBS([lt_dlinit], [ltdl])

# We need to check some advanced stuff, as there are no pkg-config definition files for Wt
AC_CACHE_CHECK([for the Wt library], [firestarter_cv_Wt_lib],
	[firestarter_cv_Wt_lib=no
	SAVED_LDFLAGS=$LDFLAGS
	LDFLAGS="$LDFLAGS -lwt"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM([#include <Wt/WText>],
		[Wt::WText dummy])],
		[WITTY_LIBS="$WITTY_LIBS -lwt"] [HAVE_WITTY=1] [firestarter_cv_Wt_lib=yes],
		[AC_MSG_ERROR([libwt is not installed.])])
	LDFLAGS=$SAVED_LDFLAGS])

AC_CACHE_CHECK([for the Wt Http library], [firestarter_cv_Wt_Http_lib],
	[firestarter_cv_Wt_Http_lib=no
	SAVED_LDFLAGS=$LDFLAGS
	LDFLAGS="$LDFLAGS -lwt -lwthttp"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM([#include <Wt/WServer>],
		[Wt::WServer dummy("dummy")])],
		[WITTY_LIBS="$WITTY_LIBS -lwthttp"] [firestarter_cv_Wt_Http_lib=yes],
		[AC_MSG_ERROR([libwthttp is not installed.])])
	LDFLAGS=$SAVED_LDFLAGS])

# Check if the boost libraries are available
BOOST_REQUIRE([1.42])
# you can check for specific parts of the boost library with:
# BOOST_ARRAY
BOOST_TEST
BOOST_FOREACH
BOOST_TUPLE
BOOST_GRAPH
BOOST_STRING_ALGO
BOOST_THREADS
BOOST_DATE_TIME

PKG_CHECK_MODULES([DEPS], [liblog4cxx >= 0.10 libconfig++ >= 1.3.2 libzmq >= 3.0 protobuf >= 2.3.0])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
