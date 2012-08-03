/**
 * @file lagoon/range/find_if.hpp
 * @brief Finding the first element satisfying a predicate
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_FIND_IF_1011291729_HPP
#define LAGOON_RANGE_FIND_IF_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>

LAGOON_NAMESPACE_BEGIN

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Searches a range for an element satisfying a predicate
/** Traverses a @a range checking the result of a @a predicate
 *  on each element. Stops at the first element satisfying the predicate
 *  and returns the resulting range.
 *
 *  @param range the range to be searched
 *  @param predicate the predicate to be used on each element of the range
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename Predicate>
Range find_if(Range range, Predicate predicate);
#else
template <class Range, typename Predicate>
Range find_if(Range range, Predicate predicate)
{
	while(!range.empty())
	{
		if(predicate(range.front())) break;
		else range.step_front();
	}
	return range;
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

