/**
 * @file lagoon/categories.hpp
 * @brief Definitions of the meta-object categories
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_CATEGORIES_1011291729_HPP
#define LAGOON_CATEGORIES_1011291729_HPP

#include <lagoon/config.hpp>
#include <puddle/categories.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/preprocessor.hpp>

LAGOON_NAMESPACE_BEGIN
/** @defgroup lagoon_categories Lagoon - Meta-object categories
 *
 *  Lagoon defines an enumeration of tag values which can
 *  be used for the categorization of run-time meta-objects
 *  in a similar manner the tag types in Mirror are used for
 *  compile-time tag-dispatching.
 */

/// Enumeration of meta-object categorization tags
/** The meta_object and meta_meta_object interfaces
 *  define member functions for querying the category
 *  of the reflected construct or meta-object.
 *
 *  The enumerated tag value names match to the names of
 *  Mirror's tag type names, like meta_object_tag, ...,
 *  meta_type_tag, meta_class_tag, etc.
 *
 *  This enumeration is identical to the one defined in the Puddle
 *  layer.
 *
 *  @see meta_meta_object
 *  @see meta_meta_object::category
 *  @see meta_meta_object::is_a
 *  @see meta_object
 *  @see mirror::meta_object_tag
 *  @see puddle::meta_object_category;
 *
 *  @ingroup lagoon_categories
 */
#ifdef MIRROR_DOCUMENTATION_ONLY
enum meta_object_category;
#else
using puddle::meta_object_category;
#endif

#undef LAGOON_HLPR_DEFINE_MO_TAG_VALUE
#undef LAGOON_HLPR_DEFINE_MO_TAG

// Implement the categorize interface member function
#define LAGOON_HELPER_IMPL_CATEGORIZE_INTERFACE(OBJECT, I, X) \
inline meta_object_category \
categorize_interface(meta_ ## OBJECT *) \
{ \
	return meta_object_category:: \
		meta_ ## OBJECT ## _tag; \
}
MIRROR_FOR_EACH_META_OBJECT(LAGOON_HELPER_IMPL_CATEGORIZE_INTERFACE, _)
#undef LAGOON_HELPER_IMPL_CATEGORIZE_INTERFACE


LAGOON_NAMESPACE_END

#endif //include guard

