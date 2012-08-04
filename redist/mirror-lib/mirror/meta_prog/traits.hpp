/**
 *  @file mirror/meta_prog/traits.hpp
 *  @brief Meta-function template classes for getting various traits
 *  of types defined in the mp scope.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_TRAITS_1011291729_HPP
#define MIRROR_META_PROG_TRAITS_1011291729_HPP

#include <mirror/meta_prog/forward_decl.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This trait meta-function can be used to check if the passed type is a range
template <typename T>
struct is_range : BooleanConstant
{ };
#else
template <typename T>
struct is_range : std::false_type
{ };

template <typename ... P>
struct is_range<range<P...> > : std::true_type
{ };
#endif

#define MIRROR_ASSERT_IS_A_RANGE(EXPR, NAME) \
	static_assert( \
		is_range<EXPR>::value, \
		#NAME " is not a Range expression." \
	)

#define MIRROR_ASSERT_RETURNS_RANGE(X) \
	MIRROR_ASSERT_IS_A_RANGE(typename X::type, X)

#define MIRROR_ASSERT_IS_A_RANGE_OR_A_META(OBJECT, EXPR, NAME) \
	static_assert( \
		mirror::mp::is_a< \
			EXPR, \
			typename mirror::meta_##OBJECT##_tag \
		>::value || is_range<EXPR>::value, \
		#NAME " is not a Range nor a Meta" \
		MIRROR_META_OBJECT_NICE_NAME_STR(OBJECT) \
		" expression" \
	)

#define MIRROR_ASSERT_RETURNS_RANGE_OR_META(OBJECT, X) \
	MIRROR_ASSERT_IS_A_RANGE_OR_A_META(OBJECT, typename X::type, X)


#ifdef MIRROR_DOCUMENTATION_ONLY
/// This trait meta-function can be used to check if the passed type is optional
template <typename T>
struct is_optional : BooleanConstant
{ };
#else
template <typename T>
struct is_optional : std::false_type
{ };

template <typename T>
struct is_optional<optional<T> > : std::true_type
{ };
#endif

#define MIRROR_ASSERT_IS_AN_OPTIONAL(EXPR, NAME) \
	static_assert( \
		is_optional<EXPR>::value, \
		#NAME " is not an Optional expression." \
	)

#define MIRROR_ASSERT_RETURNS_OPTIONAL(X) \
	MIRROR_ASSERT_IS_AN_OPTIONAL(typename X::type, X)


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

