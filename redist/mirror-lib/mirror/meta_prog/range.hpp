/**
 * @file mirror/meta_prog/range.hpp
 * @brief Forward type range
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_RANGE_1011291729_HPP
#define MIRROR_META_PROG_RANGE_1011291729_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/traits.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

/// Range of types supporting forward traversal
/**
 *  @tparam P the pack of elements in the range
 *  @see empty
 *  @see size
 *  @see front
 *  @see step_front
 *  @see at
 *  @see at_c
 *  @ingroup meta_programming
 */
template <typename ... P>
struct range
{
	typedef range type;
};

/// Convenience typedef for empty range
typedef range<> empty_range;

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Empty intrinsic meta-function for ranges
/**
 *  @tparam Range the range to be examined
 *  @see range
 *  @see size
 *  @ingroup meta_programming
 */
template <typename Range>
struct empty { };
#else
// The default implementation of the empty meta-function
template <typename X>
struct empty : empty<typename X::type>::type
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
};
#endif

template <>
struct empty<range<> >
 : std::true_type
{ };

template <typename ... P>
struct empty<range<P...> >
 : std::false_type
{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Size intrinsic meta-function for ranges
/**
 *  @tparam Range the range to be examined
 *  @see range
 *  @see empty
 *  @ingroup meta_programming
 */
template <typename Range>
struct size { };
#else
template <typename X>
struct size : size<typename X::type>::type
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
};
#endif

template <>
struct size<range<> >
 : std::integral_constant<int, 0>
{ };

template <typename T>
struct size<range<T> >
 : std::integral_constant<int, 1>
{ };

template <typename T, typename ... P>
struct size<range<T, P...> >
 : std::integral_constant<int, 1 + size<range<P...> >::value>
{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the front element of the range passed as argument
/**
 *  This operation may be invoked only on non-empty ranges.
 *
 *  @tparam Range the range front item of which is to be returned
 *  @see range
 *  @see empty
 *  @see step_front
 *  @ingroup meta_programming
 */
template <typename Range>
struct front
{
	/// The front type in the range passed as argument
	typedef unspecified_type type;
};
#else
template <typename X>
struct front
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename front<typename X::type>::type type;
};
#endif

template <typename T, typename ... P>
struct front<range<T, P...> >
{
	typedef T type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the last element of the range passed as argument
/**
 *  This operation may be invoked only on non-empty ranges.
 *
 *  @tparam BiDiRange the bi-directional range last item of which is to be returned
 *  @see range
 *  @see empty
 *  @see front
 *  @ingroup meta_programming
 */
template <typename BiDiRange>
struct back
{
	/// The front type in the range passed as argument
	typedef unspecified_type type;
};
#else
template <typename X>
struct back
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename back<typename X::type>::type type;
};
#endif
template <typename T>
struct back<range<T> >
{
	typedef T type;
};

template <typename T, typename ... P>
struct back<range<T, P...> >
 : back<range<P...> >
{ };

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the element of the range passed as argument at the given Index
/**
 *  This operation may be invoked only on non-empty ranges.
 *
 *  @tparam Range the range item of which is to be returned
 *  @tparam Index compile-time integral constant value index of the element to
 *  be retrieved
 *
 *  @see range
 *  @see empty
 *  @see front
 *  @see at
 *  @ingroup meta_programming
 */
template <typename Range, int Index>
struct at_c
{
	/// The type in the range passed as argument at the specified position
	typedef unspecified_type type;
};
#else
template <typename X, int Index>
struct at_c
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename at_c<typename X::type, Index>::type type;
};
#endif

template <typename T, typename ... P>
struct at_c<range<T, P...>, 0>
{
	typedef T type;
};

template <typename T, typename ... P, int Index>
struct at_c<range<T, P...>, Index>
{
	typedef typename at_c<range<P...>, Index-1>::type type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns the element of the range passed as argument at the given Index
/**
 *  This operation may be invoked only on non-empty ranges.
 *
 *  @tparam Range the range item of which is to be returned
 *  @tparam Index integral constant type index of the element to
 *  be retrieved
 *
 *  @see range
 *  @see empty
 *  @see front
 *  @see at_c
 *  @ingroup meta_programming
 */
template <typename Range, class Index>
struct at
{
	/// The type in the range passed as argument at the specified position
	typedef unspecified_type type;
};
#else
template <typename Range, class Index>
struct at : at_c<Range, Index::value>
{ };
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Range front traversal meta-function
/** This meta functions returns a range which contains
 *  the same elements as the range passed as argument
 *  with the exception of the first one.
 *
 *  This operation may be invoked only on non-empty ranges.
 *
 *  @tparam Range the range to be traversed
 *  @see range
 *  @see empty
 *  @see front
 *  @ingroup meta_programming
 */
template <typename Range>
struct step_front
{
	/// The range after steping front
	typedef unspecified_type type;
};
#else
template <typename X>
struct step_front
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename step_front<
		typename X::type
	>::type type;
};
#endif

template <typename T, typename ... P>
struct step_front<range<T, P...> >
{
	typedef range<P...> type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function appending the @a Item to the Range
/**
 *  @see range
 *  @ingroup meta_programming
 */
template <typename Range, typename Item>
struct push_back
{
	/// The resulting range
	typedef Range type;
};
#else
template <typename X, typename Item>
struct push_back
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename push_back<
		typename X::type,
		Item
	>::type type;
};
#endif

template <typename ... P, typename Item>
struct push_back<range<P...>, Item>
{
	typedef range<P..., Item> type;
};

namespace aux {

template <typename Expr, typename IsRange>
struct as_range;

template <typename Range>
struct as_range<Range, std::true_type>
{
	typedef Range type;
};

template <typename Expr>
struct as_range<Expr, std::false_type>
{
	MIRROR_ASSERT_RETURNS_RANGE(Expr);
	typedef typename Expr::type type;
};

} // namespace aux

template <typename Expr>
struct as_range : public aux::as_range<
	Expr,
	typename is_range<Expr>::type
>{ };


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

