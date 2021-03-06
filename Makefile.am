## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

## FS configuration directory
fsconfdir = ${sysconfdir}/firestarter

## FS shared libraries directory
fslibdir = ${libdir}/firestarter

## Modules configuration directory
modconfdir = ${localstatedir}/firestarter

## Set the default command-line flags for the C preprocessor to the value
## obtained from pkg-config via PKG_CHECK_MODULES in configure.ac.  These
## flags are passed to the compiler for both C and C++, in addition to the
## language-specific options.
AM_CPPFLAGS = $(DEPS_CFLAGS) -I src/common -I redist/mirror-lib -DSYSCONFDIR=\"$(fsconfdir)\" \
              -DMODCONFDIR=\"${modconfdir}\" -DLIBDIR=\"${fslibdir}\"
MODULES_CPPFLAGS = $(AM_CPPFLAGS) -I src/modules
TESTS_CPPFLAGS = $(AM_CPPFLAGS) -DIN_UNIT_TESTING
TESTS_LIBS = $(DEPS_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)
PERSISTENT_CFLAGS = $(SOCI_CFLAGS) $(MYSQL_CFLAGS) $(PGSQL_CFLAGS) $(SQLITE_CFLAGS)

## Define an executable target "firestarter", which will be installed into the
## directory named by the predefined variable $(bindir).
bin_PROGRAMS = firestarter

## Define the test executables that will provide unit testing.
TESTS = modulemanager_tests
check_PROGRAMS = $(TESTS)

## Define the files that will be generated by Google's Protocol Buffers compiler
BUILT_SOURCES = protobuf/module.pb.cc protobuf/module.pb.h

## Define the files that will be generated by MAuReEn
BUILT_SOURCES += src/modules/examples/persistance/person.meta.hpp

## Define the source files for the files above
EXTRA_DIST = protobuf/module.proto

## Define the different module libraries that will be compiled and installed
pkglib_LTLIBRARIES = webinterface.la persistance.la sqlite_backend.la mysql_backend.la postgresql_backend.la

## FS configuration files
fsconf_DATA = configuration/fs.cfg configuration/logging.cfg configuration/lighttpd.conf

## Modules configuration files
modconf_DATA = configuration/modules/webinterface.cfg configuration/modules/persistance.cfg \
			   configuration/modules/sqlite_backend.cfg

## WebInterface data directory
webinterfacedir = ${localstatedir}/firestarter/www
webinterface_DATA = redist/htmlkickstart/style.css
webinterfacecssdir = ${webinterfacedir}/css
webinterfacecss_DATA = redist/htmlkickstart/css/*.css
webinterfacebasefontsdir = ${webinterfacecssdir}/fonts/base
webinterfacebasefonts_DATA = redist/htmlkickstart/css/fonts/base/*
webinterfacesocialfontsdir = ${webinterfacecssdir}/fonts/social
webinterfacesocialfonts_DATA = redist/htmlkickstart/css/fonts/social/*
webinterfaceimgdir = ${webinterfacecssdir}/img
webinterfaceimg_DATA = redist/htmlkickstart/css/img/*.png redist/htmlkickstart/css/img/*.gif
webinterfacefancyboximgdir = ${webinterfaceimgdir}/fancybox
webinterfacefancyboximg_DATA = redist/htmlkickstart/css/img/fancybox/*
webinterfacerteimgdir = ${webinterfaceimgdir}/rte
webinterfacerteimg_DATA = redist/htmlkickstart/css/img/rte/*
webinterfacejsdir = ${webinterfacedir}/js
webinterfacejs_DATA = redist/htmlkickstart/js/*

## Define the list of source files for the "firestarter" target. The file extension
## .cpp is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. The
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project.
firestarter_SOURCES = src/fs/main.cpp src/fs/main.hpp \
                      src/fs/modulemanager.hpp src/fs/modulemanager.cpp \
                      src/fs/instancemanager.hpp src/fs/instancemanager.cpp \
                      src/fs/dependencygraph.hpp src/fs/dependencygraph.cpp \
                      src/common/log.hpp src/common/log.cpp src/common/simplecache.hpp \
                      protobuf/module.pb.cc protobuf/module.pb.h \
                      src/common/zmq/zmq.hpp src/common/zmq/zmqhelper.hpp \
                      src/common/zmq/zmqsocket.hpp src/common/zmq/zmqsocket.cpp \
                      src/common/module.hpp

## Set the library dependencies for the "firestarter" target to the value obtained
## from pkg-config via PKG_CHECK_MODULES in configure.ac.  These libraries are
## passed to the linker in addition to the other linker flags.
firestarter_LDADD = $(DEPS_LIBS) $(BOOST_THREAD_LIBS) \
                    -dlopen webinterface.la \
                    -dlopen persistance.la -dlopen sqlite_backend.la -dlopen mysql_backend.la \
                    -dlopen postgresql_backend.la

## Set the libtool flags
firestarter_LDFLAGS = -export-dynamic

## Define the list of source files for the "libdummy" library target. The file extension
## .cpp is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. THe
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project

MODULES_DEFAULT_SRC = src/common/module.hpp src/common/module.cpp \
                      protobuf/module.pb.cc protobuf/module.pb.h \
                      src/common/zmq/zmq.hpp src/common/zmq/zmqhelper.hpp \
                      src/common/zmq/zmqsocket.hpp src/common/zmq/zmqsocket.cpp \
                      src/common/clients/instancemanager.hpp

PERSISTENT_DEFAULT_SRC = src/common/persistent.hpp src/common/persistent/keywords.hpp \
                         src/common/persistent/lexer.hpp \
                         src/common/persistent/storage.hpp

webinterface_la_SOURCES = $(MODULES_DEFAULT_SRC) \
                          src/modules/core/webInterface/webinterface.cpp \
                          src/modules/core/webInterface/webinterface.hpp \
                          src/modules/core/webInterface/router.hpp \
                          src/modules/core/webInterface/router.cpp \
                          src/modules/core/webInterface/mainpage.hpp \
                          src/modules/core/webInterface/mainpage.cpp \
                          src/modules/core/webInterface/blankpage.hpp \
                          src/modules/core/webInterface/blankpage.cpp \
                          src/common/webwidgets/htmltemplates.hpp \
                          src/common/webwidgets/basepage.hpp \
                          src/common/webwidgets/basepage.cpp
webinterface_la_LDFLAGS = -module -avoid-version -export-dynamic
webinterface_la_CPPFLAGS = $(MODULES_CPPFLAGS) $(WEBDEPS_CFLAGS)
webinterface_la_LIBADD = $(WEBDEPS_LIBS)

persistance_la_SOURCES = $(MODULES_DEFAULT_SRC) $(PERSISTENT_DEFAULT_SRC) \
                         src/modules/examples/persistance/persistance.hpp \
                         src/modules/examples/persistance/persistance.cpp \
                         src/modules/examples/persistance/person.meta.hpp \
                         src/modules/examples/persistance/person.hpp
persistance_la_LDFLAGS = -module -avoid-version -export-dynamic
persistance_la_CPPFLAGS = $(MODULES_CPPFLAGS) $(PERSISTENT_CFLAGS)
persistance_la_LIBADD = $(SOCI_LIBS)

sqlite_backend_la_SOURCES = src/modules/vendor/sqlite/sqlite.hpp
sqlite_backend_la_LDFLAGS = -module -avoid-version -export-dynamic
sqlite_backend_la_CPPFLAGS = $(SQLITE_CFLAGS) $(SOCI_CFLAGS)
sqlite_backend_la_LIBADD = $(SOCI_SQLITE_LIBS)

mysql_backend_la_SOURCES = src/modules/vendor/mysql/mysql.hpp
mysql_backend_la_LDFLAGS = -module -avoid-version -export-dynamic
mysql_backend_la_CPPFLAGS = $(MYSQL_CFLAGS) $(SOCI_CFLAGS)
mysql_backend_la_LIBADD = $(SOCI_MYSQL_LIBS)

postgresql_backend_la_SOURCES = src/modules/vendor/postgresql/postgresql.hpp
postgresql_backend_la_LDFLAGS = -module -avoid-version -export-dynamic
postgresql_backend_la_CPPFLAGS = $(PGSQL_CFLAGS) $(SOCI_CFLAGS)
postgresql_backend_la_LIBADD = $(SOCI_PGSQL_LIBS)

modulemanager_tests_SOURCES = src/fs/tests/modulemanager_tests.cpp \
                              src/fs/modulemanager.cpp src/fs/modulemanager.hpp \
                              src/fs/dependencygraph.cpp src/fs/dependencygraph.hpp
modulemanager_tests_LDADD = $(TESTS_LIBS)
modulemanager_tests_CPPFLAGS = $(TESTS_CPPFLAGS)

# Protobuffer code generation rules (note the first rule has multiple targets). 
%.pb.cc %.pb.h: %.proto 
	@echo "  PROTO  $<"
	@protoc --cpp_out=. $< 

# The dummy rule is added to suppress a warning from automake
%.meta.hpp %.dummy.hpp: %.hpp 
	@basedir=`echo "$<" | awk -F/ '{ print $$1"/"$$2 }'`; \
	echo "  MAURN  $@"; file=`basename $< .hpp`; \
	maureen --with-cts true --input "$$basedir"'|%|'"$$file"'|hpp|%|~|%' \
		--output "$$basedir"'|$$(path)|$$(file)|meta.$$(suffix)';

## remove protobuf and maureen files
clean-local:
	-find src -name \*.meta.hpp -exec rm -f {} \;
	-find . -name \*.pb.h -exec rm -f {} \; -or -name \*.pb.cc -exec rm -f {} \;

## Define an independent executable script for inclusion in the distribution
## archive. It will not be installed on an end user's system, however.
dist_noinst_SCRIPTS = autogen.sh

