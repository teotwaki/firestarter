/**
 * @file mirror/meta_prog/find.hpp
 * @brief Meta-function returning a sub-range of the given range starting with
 * the first occurence of the searched type.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_FIND_1011291729_HPP
#define MIRROR_META_PROG_FIND_1011291729_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/traits.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a sub-range starting with the first occurence of the searched type
/**
 *  @tparam Range the range to search in.
 *  @tparam Type the type searched for
 *
 *  @ingroup meta_programming
 */
template <typename Range, typename Type>
struct find
{
	typedef Range type;
};
#endif

// specialization of find for other expressions
template <typename X, typename T>
struct find
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename find<typename X::type, T>::type type;
};

// specialization of find for empty ranges
template <typename T>
struct find<range<>, T>
{
	typedef range<> type;
};

// specialization of find for ranges starting with the type searched for
template <typename ... P, typename T>
struct find<range<T, P...>, T>
{
	typedef range<T, P...> type;
};

// specialization of find for ranges not starting with the type searched for
template <typename H, typename ... P, typename T>
struct find<range<H, P...>, T>
{
	typedef typename find<range<P...>, T>::type type;
};


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

