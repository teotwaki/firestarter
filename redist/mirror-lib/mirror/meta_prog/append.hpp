/**
 *  @file mirror/meta_prog/append.hpp
 *  @brief Meta-function returning a range having the passed item
 *  appended.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_APPEND_1011291729_HPP
#define MIRROR_META_PROG_APPEND_1011291729_HPP


#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a range having the passed item appended.
/**
 *  @tparam Range the range to be modified
 *  @tparam T the type to be appended to the range
 *
 *  @ingroup meta_programming
 */
template <class RangeToModify, typename T>
struct append
{
	typedef Range type;
};
#endif

template <typename ... P, typename T>
struct append<range<P...>, T>
{
	typedef range<P..., T> type;
};

template <typename X, typename T>
struct append
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename append<typename X::type, T>::type type;
};


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

