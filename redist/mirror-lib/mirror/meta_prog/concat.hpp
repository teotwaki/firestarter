/**
 *  @file mirror/meta_prog/concat.hpp
 *  @brief Meta-function returning a range which is concatenation
 *  of other ranges.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_CONCAT_1011291729_HPP
#define MIRROR_META_PROG_CONCAT_1011291729_HPP


#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a range which is concatenation of other ranges.
/**
 *  @tparam Range the first range
 *  @tparam Ranges the pack of ranges to be concatenated.
 *
 *  @ingroup meta_programming
 */
template <class Range1, class ... Ranges>
struct concat
{
	typedef Range type;
};
#endif

template <>
struct concat<>
{
	typedef empty_range type;
};

// specialization for a single range
template <typename ... P>
struct concat<range<P...> >
{
	typedef range<P...> type;
};

// specialization for two ranges
template <typename ... P1, typename ... P2>
struct concat<range<P1...>, range<P2...> >
{
	typedef range<P1..., P2...> type;
};

// specialization for two range expressions
template <typename X1, typename X2>
struct concat<X1, X2>
{
	MIRROR_ASSERT_RETURNS_RANGE(X1);
	MIRROR_ASSERT_RETURNS_RANGE(X2);

	typedef typename concat<
		typename X1::type,
		typename X2::type
	>::type type;
};

template <typename X1, typename X2, typename ... Xs>
struct concat<X1, X2, Xs...>
{
	typedef typename concat<
		X1,
		typename concat<X2, Xs...>::type
	>::type type;
};


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

