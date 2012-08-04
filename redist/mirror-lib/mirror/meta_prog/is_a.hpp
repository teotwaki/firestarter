/**
 * @file mirror/meta_prog/is_a.hpp
 * @brief Boolean meta-function checking if a MetaObject is a model
 * of a specified concept
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_IS_A_1011291729_HPP
#define MIRROR_META_PROG_IS_A_1011291729_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/not.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace mp {
namespace aux {

template <class X, class RealConcept, class QueriedConcept>
struct is_a_helper : public ::std::is_base_of<
	QueriedConcept,
	RealConcept
>{ };

template <class X, class Concept>
struct is_a_helper<X, Concept, Concept>
 : public std::true_type
{ };

template <class QueriedConcept>
struct is_a_helper<void*, non_meta_object_tag, QueriedConcept>
 : public std::false_type
{ };

void* is_a_hlpr_fn(...);

template <typename X>
void* is_a_hlpr_fn2(X*, X*);

template <typename X, typename Y>
typename X::type is_a_hlpr_fn2(X*, Y*);

template <typename X>
auto is_a_hlpr_fn(X* x, typename X::type* y = nullptr) ->
decltype(is_a_hlpr_fn2(x,y));


template <class X, class QueriedConcept>
struct is_a_helper<X, non_meta_object_tag, QueriedConcept>
 : public is_a_helper<
	decltype(is_a_hlpr_fn((X*)nullptr)),
	typename meta_object_category<
		decltype(is_a_hlpr_fn((X*)nullptr))
	>::type,
	QueriedConcept
>
{ };


} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a boolean constant based on whether the MetaObject is the Concept
/**
 *  @tparam MetaObject
 *  @tparam Concept
 *
 *  @ingroup meta_programming
 */
template <class MetaObject, class Concept>
struct is_a
{
	/// The @c std::true_type or the @c std::false_type
	typedef BooleanConstant type;
};

/// Returns a boolean constant based on whether the MetaObject isn't the Concept
/**
 *  @tparam MetaObject
 *  @tparam Concept
 *
 *  @ingroup meta_programming
 */
template <class MetaObject, class Concept>
struct is_not_a
{
	/// The @c std::true_type or the @c std::false_type
	typedef BooleanConstant type;
};
#else

// X is a concept
template <class X, class Concept>
struct is_a : aux::is_a_helper<
	X,
	typename meta_object_category<X>::type,
	typename eval_meta_object_tag<Concept>::type
>
{ };

// X isn't a concept
template <class X, class Concept>
struct is_not_a : not_<aux::is_a_helper<
	X,
	typename meta_object_category<X>::type,
	typename eval_meta_object_tag<Concept>::type
> >
{ };
#endif

#define MIRROR_ASSERT_IS_A_META(OBJECT, EXPR, NAME) \
static_assert( \
	mirror::mp::is_a< \
		EXPR, \
		typename mirror::meta_##OBJECT##_tag \
	>::value, \
	#NAME " is not a Meta" \
	MIRROR_META_OBJECT_NICE_NAME_STR(OBJECT) \
	" expression" \
)

#define MIRROR_ASSERT_RETURNS_META(OBJECT, X) \
	MIRROR_ASSERT_IS_A_META(OBJECT, typename X::type, X)

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

