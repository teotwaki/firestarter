/**
 * @file lagoon/range/contains.hpp
 * @brief Checking if a range contains elements satisfying a predicate
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_CONTAINS_1011291729_HPP
#define LAGOON_RANGE_CONTAINS_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>

LAGOON_NAMESPACE_BEGIN

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Searches a range for an element satisfying a predicate
/** Traverses a @a range checking the result of a @a predicate
 *  on each element. Stops at the first element satisfying the predicate
 *  and returns true. If no such elements exists in the range returns false.
 *
 *  @param range the range to be searched
 *  @param predicate the predicate to be used on each element of the range
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename Predicate>
bool contains(Range range, Predicate predicate);
#else
template <class Range, typename Predicate>
bool contains(Range range, Predicate predicate)
{
	while(!range.empty())
	{
		if(predicate(range.front())) return true;
		else range.step_front();
	}
	return false;
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

