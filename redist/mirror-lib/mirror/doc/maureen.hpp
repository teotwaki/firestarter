/**
 *  file mirror/doc/maureen.hpp
 *  Documentation only header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_MAUREEN_1101182146_HPP
#define MIRROR_DOC_MAUREEN_1101182146_HPP

#ifdef MIRROR_DOCUMENTATION_ONLY

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @page maureen Mirror Auto-Reflection Engine
 *
 *  The @link mirror_registering manual registering process@endlink can
 *  be tedious and error-prone in some situations. Although the registering
 *  macros use auto-detection and many things do not have to be specified
 *  explicitly, some changes in the base-level classes, like adding or removing
 *  of a member variable, constructor or a whole class, etc. require changes
 *  in the registering code.
 *
 *  If no special tweaking in the reflection of the base-level constructs
 *  (like hiding certain members or constructors of a class or specifying
 *  of getter/setter functions for a member variable) is required, then
 *  support for automatic reflection is desirable. To provide this support
 *  is the aim of the @em MAuReEn (Mirror Auto-Reflection Engine) project.
 *
 *  This is only a short introduction and tutorial for the @c maureen utility.
 *  For more details consult its documentation
 *  and the command line help screen printed by the @c --help option.
 *
 *  @section maureen_hosting Hosting and source repository
 *
 *  @em MAuReEn's sources are currently managed by git, hosted on Gitorious:
 *  <A HREF="git://gitorious.org/maureen/maureen.git">git://gitorious.org/maureen/maureen.git</A>.
 *
 *  @section maureen_building Building
 *
 *  @em MAuReEn uses the cmake build system. To get information on how to
 *  build and install it you can consult the README and BUILDING files
 *  in the root directory of the source distribution.
 *
 *  @section maureen_intro Introduction
 *
 *  MAuReEn is invoked from shell by the @c maureen command with or without additional
 *  command-line options. Generally MAuReEn processes several input files
 *  containing base-level C++ declarations, typically header files and creates
 *  one or several output files containing C++ code for registering those
 *  base-level constructs with Mirror using the registering macros according
 *  to one or several pairs of input and output patterns.
 *  The patterns can be specified either by using command line options or read from
 *  special text files (referred to as @em MAREfiles, for Mirror Auto-Reflection
 *  Engine files).
 *
 *  Additional command line or MAREfile options may be used to specify things
 *  like pre-defined symbols for the C++ preprocessor, additional search paths
 *  for include files, etc.
 *
 *  The manual for the MAuReEn tool can be found
 *  <A HREF="/~chochlik/maureen/manual.pdf">here</A>.
 */

MIRROR_NAMESPACE_END

#endif // DOCUMENTATION_ONLY
#endif //include guard

