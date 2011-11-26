## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

## Set the default command-line flags for the C preprocessor to the value
## obtained from pkg-config via PKG_CHECK_MODULES in configure.ac.  These
## flags are passed to the compiler for both C and C++, in addition to the
## language-specific options.
AM_CPPFLAGS = $(DEPS_CFLAGS) -I src/common

## Define an executable target "hello", which will be installed into the
## directory named by the predefined variable $(bindir).
bin_PROGRAMS = firestarter

## Define the list of source files for the "firestarter" target. The file extension
## .cpp is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. The
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project.
firestarter_SOURCES = src/fs/main.cpp src/fs/main.h \
                      src/fs/modulemanager.h \
                      src/common/log.h src/common/log.cpp

## Set the library dependencies for the "example" target to the value obtained
## from pkg-config via PKG_CHECK_MODULES in configure.ac.  These libraries are
## passed to the linker in addition to the other linker flags.
firestarter_LDADD = $(DEPS_LIBS)

## Define an independent executable script for inclusion in the distribution
## archive. It will not be installed on an end user's system, however.
dist_noinst_SCRIPTS = autogen.sh
