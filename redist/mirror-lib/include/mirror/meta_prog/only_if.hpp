/**
 *  @file mirror/meta_prog/only_if.hpp
 *  @brief Meta-function returning a sub-range containing only
 *  elements of a range satisfying a predicate.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_ONLY_IF_1011291729_HPP
#define MIRROR_META_PROG_ONLY_IF_1011291729_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/if.hpp>
#include <mirror/meta_prog/prepend.hpp>
#include <mirror/meta_prog/lambda.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a sub-range containing only elements satisfying a predicate
/**
 *  @tparam Range the range to be filtered
 *  @tparam Predicate a unary lambda expression returning a boolean type
 *
 *  @ingroup meta_programming
 */
template <class Range, class Predicate>
struct only_if
{
	typedef Range type;
};
#endif

// specialization for empty ranges
template <class Predicate>
struct only_if<range<>, Predicate>
{
	typedef range<> type;
};

// specialization for ranges
template <typename T, typename ... P, class Predicate>
struct only_if<range<T, P...>, Predicate>
{
	typedef typename if_<
		typename apply<Predicate, T>::type,
		typename prepend<T, only_if<range<P...>, Predicate> >::type,
		typename only_if<range<P...>, Predicate>::type
	>::type type;
};

// specialization for range expressions
template <typename X, class Predicate>
struct only_if
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename only_if<typename X::type, Predicate>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

