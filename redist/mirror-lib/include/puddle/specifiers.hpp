/**
 * @file puddle/specifiers.hpp
 * @brief Definitions of the specifiers and categorization tag values
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_SPECIFIERS_1011291729_HPP
#define PUDDLE_SPECIFIERS_1011291729_HPP

#include <mirror/specifier_tags.hpp>
#include <mirror/preprocessor.hpp>

PUDDLE_NAMESPACE_BEGIN

/** @defgroup puddle_specifiers Puddle - Specifiers
 *
 *  Puddle provides access to specifier meta-data through specifier
 *  wrapper classes and allows to do run-time categorization with specifier
 *  tags.
 */

#define PUDDLE_HLPR_DEFINE_SPEC_TAG_NAME(SPEC, X) \
	spec_ ## SPEC ## _tag |

#define PUDDLE_HLPR_DEFINE_SPEC_TAG(SPEC, KW, GRP, I, X) \
	spec_ ## SPEC ## _tag = \
	MIRROR_FOR_EACH_SPECIFIER_BASE( \
		SPEC, \
		PUDDLE_HLPR_DEFINE_SPEC_TAG_NAME, \
		X, \
		MIRROR_PP_EMPTY \
	) 1ULL << I,

/// Enumeration of tags categorizing various specifiers
/**
 *  The enumerated tag value names match to the names of
 *  Mirror's specifier tag type names, like spec_virtual_tag, ...,
 *  spec_public_tag, spec_class_tag, etc.
 *
 *  @ingroup puddle_specifiers
 */
#ifdef MIRROR_DOCUMENTATION_ONLY
enum specifier_category
#else
enum class specifier_category : unsigned long long
#endif
{
	MIRROR_FOR_EACH_SPECIFIER(
		PUDDLE_HLPR_DEFINE_SPEC_TAG,
		0
	)
	spec__tag = spec_none_tag
};

#undef PUDDLE_HLPR_DEFINE_SPEC_TAG_VALUE
#undef PUDDLE_HLPR_DEFINE_SPEC_TAG

#define PUDDLE_HELPER_MAKE_SPEC_GET_CATEGORY_FN(SPEC, KW, GRP, I, X) \
inline specifier_category get_category(mirror::spec_ ## SPEC ## _tag) \
{ \
	return specifier_category::spec_ ## SPEC ## _tag; \
}

MIRROR_FOR_EACH_SPECIFIER(PUDDLE_HELPER_MAKE_SPEC_GET_CATEGORY_FN, _)
#undef PUDDLE_HELPER_MAKE_SPEC_GET_CATEGORY_FN


PUDDLE_NAMESPACE_END


#endif //include guard

