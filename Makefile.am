## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

## Set the default command-line flags for the C preprocessor to the value
## obtained from pkg-config via PKG_CHECK_MODULES in configure.ac.  These
## flags are passed to the compiler for both C and C++, in addition to the
## language-specific options.
AM_CPPFLAGS = $(DEPS_CFLAGS) -I src/common
MODULES_CPPFLAGS = $(AM_CPPFLAGS) -I src/modules
TESTS_CPPFLAGS = $(AM_CPPFLAGS) -DIN_UNIT_TESTING
TESTS_LIBS = $(DEPS_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

## Define an executable target "firestarter", which will be installed into the
## directory named by the predefined variable $(bindir).
bin_PROGRAMS = firestarter

## Define the test executables that will provide unit testing.
TESTS = modulemanager_tests
check_PROGRAMS = $(TESTS)

## Define the different module libraries that will be compiled and installed
pkglib_LTLIBRARIES = dummy.la signalEmitter.la signalReceiver.la

## Define the list of source files for the "firestarter" target. The file extension
## .cpp is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. The
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project.
firestarter_SOURCES = src/fs/main.cpp src/fs/main.h \
                      src/fs/modulemanager.h src/fs/modulemanager.cpp \
                      src/common/log.h src/common/log.cpp

## Set the library dependencies for the "firestarter" target to the value obtained
## from pkg-config via PKG_CHECK_MODULES in configure.ac.  These libraries are
## passed to the linker in addition to the other linker flags.
firestarter_LDADD = $(DEPS_LIBS) \
                    -dlopen dummy.la \
                    -dlopen signalEmitter.la \
                    -dlopen signalReceiver.la

## Set the libtool flags
firestarter_LDFLAGS = -export-dynamic

## Define the list of source files for the "libdummy" library target. The file extension
## .cpp is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. THe
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project
dummy_la_SOURCES = src/modules/core/dummy/dummy.cpp \
                   src/modules/core/dummy/dummy.h

dummy_la_LDFLAGS = -module -avoid-version -export-dynamic

signalReceiver_la_SOURCES = src/modules/core/signalReceiver/signalreceiver.cpp \
                            src/modules/core/signalReceiver/signalreceiver.h

signalReceiver_la_LDFLAGS = -module -avoid-version -export-dynamic
signalReceiver_la_CPPFLAGS = $(MODULES_CPPFLAGS)

signalEmitter_la_SOURCES = src/modules/core/signalEmitter/signalemitter.cpp \
                           src/modules/core/signalEmitter/signalemitter.h

signalEmitter_la_LDFLAGS = -module -avoid-version -export-dynamic
signalEmitter_la_CPPFLAGS = $(MODULES_CPPFLAGS)

modulemanager_tests_SOURCES = src/fs/tests/modulemanager_tests.cpp \
                               src/fs/modulemanager.cpp src/fs/modulemanager.h
modulemanager_tests_LDADD = $(TESTS_LIBS)
modulemanager_tests_CPPFLAGS = $(TESTS_CPPFLAGS)

## Define an independent executable script for inclusion in the distribution
## archive. It will not be installed on an end user's system, however.
dist_noinst_SCRIPTS = autogen.sh

