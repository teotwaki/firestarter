/**
 * @file mirror/meta_prog/before.hpp
 * @brief Meta-function returning a sub-range of the given range ending before
 * the first occurence of the searched type.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_BEFORE_1107061248_HPP
#define MIRROR_META_PROG_BEFORE_1107061248_HPP


#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/traits.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a sub-range ending before the first occurence of the searched type
/**
 *  @tparam Range the range to search in.
 *  @tparam Type the type searched for
 *
 *  @ingroup meta_programming
 */
template <typename Range, typename Type>
struct before
{
	typedef Range type;
};
#endif

// specialization of before for other expressions
template <typename X, typename T>
struct before
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename before<typename X::type, T>::type type;
};

// specialization of before for empty ranges
template <typename T>
struct before<range<>, T>
{
	typedef range<> type;
};

namespace aux {

template <typename Result, typename Range, typename T>
struct bfr_hlpr;

template <typename ... R, typename ... P, typename T>
struct bfr_hlpr<
	mirror::mp::range<R...>,
	mirror::mp::range<T, P...>,
	T
>
{
	typedef mirror::mp::range<R...> type;
};

template <typename ... R, typename L, typename ... P, typename T>
struct bfr_hlpr<
	mirror::mp::range<R...>,
	mirror::mp::range<L, P...>,
	T
>
{
	typedef typename bfr_hlpr<
		mirror::mp::range<R..., L>,
		mirror::mp::range<P...>,
		T
	>::type type;
};

template <typename ... R, typename T>
struct bfr_hlpr<
	mirror::mp::range<R...>,
	mirror::mp::range<>,
	T
>
{
	typedef mirror::mp::range<R...> type;
};

} // namespace aux

template <typename H, typename ... P, typename T>
struct before<range<H, P...>, T>
{
	typedef typename mirror::mp::aux::bfr_hlpr<
		mirror::mp::range<>,
		mirror::mp::range<H, P...>,
		T
	>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

