/**
 * @file lagoon/reflection.hpp
 * @brief Implementation of the reflctions functions and macros
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_REFLECTION_1011291729_HPP
#define LAGOON_REFLECTION_1011291729_HPP

#include <lagoon/meta_objects.hpp>
#include <mirror/meta_type.hpp>
#include <mirror/meta_class.hpp>

LAGOON_NAMESPACE_BEGIN

/** @defgroup lagoon_reflection Lagoon - Reflection functions
 *
 *  Lagoon implements several reflection functions that can
 *  be used for obtaining run-time meta-objects reflecting
 *  namespaces, types, classes, etc.
 */

/// Function for reflecting types
/** This function returns a pointer to an meta_type reflecting
 *  the type passed as the template parameter @a Type
 *
 *  @tparam Type the type to be reflected
 *
 *  @see reflected_class
 *
 *  @ingroup lagoon_reflection
 */
template <typename Type>
inline shared<meta_type> reflected_type(void)
{
	return aux::get_meta_object<
		MIRRORED_TYPE(Type),
		meta_type
	>();
}

/// Function for reflecting classes
/** This function returns a pointer to an meta_class reflecting
 *  the class passed as the template parameter @a Class
 *
 *  @tparam Class the class to be reflected
 *
 *  @see reflected_type
 *
 *  @ingroup lagoon_reflection
 */
template <typename Class>
inline shared<meta_class> reflected_class(void)
{
	return aux::get_meta_object<
		MIRRORED_CLASS(Class),
		meta_class
	>();
}


/// Function reflecting the global scope
/** This function returns a pointer to a meta_namespace reflecting
 *  the global scope.
 *
 *  @see reflected_type
 *  @see reflected_class
 *
 *  @ingroup lagoon_reflection
 */
inline shared<meta_namespace> reflected_global_scope(void)
{
	return aux::get_meta_object<
		MIRRORED_GLOBAL_SCOPE(),
		meta_namespace
	>();
}


LAGOON_NAMESPACE_END

#endif //include guard

