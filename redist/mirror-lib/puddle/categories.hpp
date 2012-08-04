/**
 * @file puddle/categories.hpp
 * @brief Definitions of the run-time meta-object categories
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_CATEGORIES_1011291729_HPP
#define PUDDLE_CATEGORIES_1011291729_HPP

#include <puddle/config.hpp>
#include <puddle/meta_object_tags.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/preprocessor.hpp>

PUDDLE_NAMESPACE_BEGIN
/** @defgroup puddle_categories Puddle - Meta-object categories
 *
 *  Puddle defines an enumeration of tag values which can
 *  be used for the categorization of run-time meta-objects
 *  in a similar manner the tag types in Mirror are used for
 *  compile-time tag-dispatching.
 */

#define PUDDLE_HLPR_DEFINE_MO_TAG_NAME(OBJECT, X) \
	meta_ ## OBJECT ## _tag |

#define PUDDLE_HLPR_DEFINE_MO_TAG(OBJECT, I, X) \
	meta_ ## OBJECT ## _tag = \
	MIRROR_FOR_EACH_META_OBJECT_BASE( \
		OBJECT, \
		PUDDLE_HLPR_DEFINE_MO_TAG_NAME, \
		X, \
		MIRROR_PP_EMPTY \
	) 1ULL << I,

/// Enumeration of meta-object categorization tags
/** The meta_object and meta_meta_object interfaces
 *  define member functions for querying the category
 *  of the reflected construct or meta-object.
 *
 *  The enumerated tag value names match to the names of
 *  Mirror's tag type names, like meta_object_tag, ...,
 *  meta_type_tag, meta_class_tag, etc.
 *
 *  @see meta_meta_object
 *  @see meta_meta_object::category
 *  @see meta_meta_object::is_a
 *  @see meta_object
 *  @see mirror::meta_object_tag
 *
 *  @ingroup puddle_categories
 */
#ifdef MIRROR_DOCUMENTATION_ONLY
enum meta_object_category
#else
enum class meta_object_category : unsigned long long
#endif
{
	MIRROR_FOR_EACH_META_OBJECT(
		PUDDLE_HLPR_DEFINE_MO_TAG,
		0
	)
	non_meta_meta_object_tag = 0
};

#undef PUDDLE_HLPR_DEFINE_MO_TAG_VALUE
#undef PUDDLE_HLPR_DEFINE_MO_TAG

#define PUDDLE_HELPER_MAKE_MO_GET_CATEGORY_FN(OBJECT, I, X) \
	inline meta_object_category get_category( \
		mirror::meta_ ## OBJECT ## _tag \
	) \
	{ \
		return meta_object_category::meta_ ## OBJECT ## _tag; \
	}

MIRROR_FOR_EACH_META_OBJECT(PUDDLE_HELPER_MAKE_MO_GET_CATEGORY_FN, _)
#undef PUDDLE_HELPER_MAKE_MO_GET_CATEGORY_FN


PUDDLE_NAMESPACE_END

#endif //include guard

