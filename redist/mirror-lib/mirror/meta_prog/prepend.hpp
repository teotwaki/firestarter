/**
 *  @file mirror/meta_prog/prepend.hpp
 *  @brief Meta-function returning a range having the passed item
 *  prepended.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_PREPEND_1011291729_HPP
#define MIRROR_META_PROG_PREPEND_1011291729_HPP


#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a range having the passed item prepended.
/**
 *  @tparam T the type to be prepended to the range
 *  @tparam Range the range to be modified
 *
 *  @ingroup meta_programming
 */
template <typename T, class RangeToModify>
struct prepend
{
	typedef Range type;
};
#endif

template <typename T, typename ... P>
struct prepend<T, range<P...> >
{
	typedef range<T, P...> type;
};

template <typename T, typename X>
struct prepend
{
	MIRROR_ASSERT_RETURNS_RANGE(X);
	typedef typename prepend<T, typename X::type>::type type;
};


} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

