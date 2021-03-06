/**
 *  @file mirror/mirror.hpp
 *  @brief All-in-one inclusion of nearly everything in Mirror
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2006-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_MIRROR_1011291729_HPP
#define MIRROR_MIRROR_1011291729_HPP

// include everything except pre-registered items
#include <mirror/mirror_base.hpp>
// pre-registering of the default set of namespaces, types, classes, etc.
#include <mirror/pre_registered/default.hpp>

/** @defgroup registering_macros Mirror - Registering macros
 *
 *  Constructs like namespaces, types, typedefs, classes, class member
 *  variables, base classes, etc. need to be registered before they can
 *  be reflected or otherwise used. To simplify the registration process
 *  and to make it more convenient, the Mirror library provides several
 *  preprocessor macros described in greater detail in the references pages
 *  accessible through the links provided below.
 *
 *  More on registering can be found in the @ref mirror_registering section.
 */

/** @defgroup reflection_macros Mirror - Reflection macros
 *
 *  The reflection macros simplify the process of reflection and hide
 *  some of the internal details of Mirror's implementation.
 */

/** @defgroup utility_macros Mirror - Utility macros
 *
 *  Other macros
 */

/** @defgroup mirror_reflection_expressions Mirror - Reflection expressions
 *
 *  Using macros in client code can bring some problems, including trouble
 *  with debuggers, "uglyfication" of the code, etc. Therefore Mirror provides
 *  alternative means for the reflection of certain things like types which
 *  can be used instead of macros.
 */


#ifdef MIRROR_DOCUMENTATION_ONLY
/// All Mirror's types, classes, functions, etc. are defined in this namespace
namespace mirror
{
#else
MIRROR_NAMESPACE_BEGIN
#endif

/** @mainpage Mirror C++ reflection library
 *
 *  @image html  mirror.png
 *  @image latex mirror.png
 *
 *  Copyright &copy; 2006-2011 Matus Chochlik.
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @note Although the 'Boostified' version of Mirror uses the
 *  Boost C++ libraries Coding Guidelines
 *  and is implemented inside of the boost namespace, it IS NOT an officially
 *  reviewed and accepted Boost library. Mirror is being developed with the
 *  intention to be submitted for review for inclusion to the Boost C++ libraries.
 *
 *  @note The Mirror library is currently in development and so is this
 *  reference manual.
 *
 *  @section mirror_intro Introduction
 *
 *  This is a reference manual, generated by @em Doxygen, for the Mirror
 *  compile-time reflection library.
 *
 *  Other resources and documentation for Mirror and the related libraries and utilities
 *  can also be found on the project <A HREF="../../../index.html">home page</A>.
 *
 *  @subsection mirror_features Features
 *
 */

#ifdef MIRROR_DOCUMENTATION_ONLY
}
/* namespace mirror */
#else
MIRROR_NAMESPACE_END
#endif

#endif //include guard

