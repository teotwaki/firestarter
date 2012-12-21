
AC_PREREQ([2.64])
AC_INIT([Firestarter], [0.1], [firestarter-dev@lists.wedrop.it], [firestarter], [http://firestarter.wedrop.it])
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

AC_ARG_ENABLE([debugging], AS_HELP_STRING([--enable-debugging],
	[Enable debugging compilation options (e.g. CPU profiling, -ggdb3, -O0)]))

AC_ARG_ENABLE([profiling], AS_HELP_STRING([--with-profiling],
	[Enable linking against gperftools libprofiler]))

# Check whether debugging mode is enabled or not
AS_IF([test "x$enable_debugging" = "xyes"], [
	CXXFLAGS="-g -ggdb3 -O0"
	CPPFLAGS="$CPPFLAGS -DLTDL_DEBUG"
])

# Make sure the compile is capable of C++11. C++0x is not sufficient.
AX_CHECK_COMPILE_FLAG([-std=gnu++11], [CXXFLAGS="$CXXFLAGS -std=gnu++11"], [
	AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"], [
		AC_MSG_ERROR([Compiler needs to support either gnu++11 or c++11 (we hear clang++ works really well!)])
	])
])

# Check whether MAuReEn is correctly installed
AC_CHECK_PROG(maureen_present, maureen, yes)
AS_IF([test "x$maureen_present" != "xyes"], [
	AC_MSG_ERROR([Could not locate maureen. Please see README for more details.])], [
#else
	# Check whether doxygen is installed
	AC_CHECK_PROG(doxygen_present, doxygen, yes)
	# Check whether xsltproc is installed
	AC_CHECK_PROG(xsltproc_present, xsltproc, yes)
	AS_IF([test "x$doxygen_present" != "xyes"], [
		AC_MSG_ERROR([Could not locate doxygen. Doxygen is required for MAuReEn])
	])
	AS_IF([test "x$xsltproc_present" != "xyes"], [
		AC_MSG_ERROR([Could not locate xsltproc. xsltproc is required for MAuReEn])
	])
])

# If debugging is enabled, add libprofiler to the list of libraries
AS_IF([test "x$with_profiling" = "xyes"], [
	PKG_CHECK_MODULES([DEPS],
		[liblog4cxx >= 0.10 libconfig++ >= 1.3.2 libzmq >= 3.0 protobuf >= 2.3.0 libprofiler >= 2.0])
	], [
	PKG_CHECK_MODULES([DEPS], [liblog4cxx >= 0.10 libconfig++ >= 1.3.2 libzmq >= 3.0 protobuf >= 2.3.0])
	]
)
# Check whether the fastcgi connector and libctemplate are availble
PKG_CHECK_MODULES([WEBDEPS], [fastcgi++ >= 2.0 libctemplate >= 2.2])

# Check whether mysql_config is available
AC_CHECK_PROG(mysql_config_present, mysql_config, yes)
AS_IF([test "x$mysql_config_present" = "xyes"], [
	MYSQL_LIBS="`mysql_config --libs`"
	MYSQL_CFLAGS="`mysql_config --cflags`"
	AC_SUBST(MYSQL_LIBS)
	AC_SUBST(MYSQL_CFLAGS)
])

PKG_CHECK_MODULES([SQLITE], [sqlite3 >= 3.7],,
	[AC_MSG_WARN([Could not find the SQLite 3 library. Building without SQLite support.])])

PKG_CHECK_MODULES([PGSQL], [libpqxx >= 3.1],,
	[AC_MSG_WARN([Could not find the PostgreSQL 3.1 library. Building without PostgreSQL support.])])

AX_SOCI([3.1])

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

AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
