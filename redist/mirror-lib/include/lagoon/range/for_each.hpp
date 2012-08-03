/**
 * @file lagoon/range/for_each.hpp
 * @brief Executing a functor on all elements of a range
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_FOR_EACH_1011291729_HPP
#define LAGOON_RANGE_FOR_EACH_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>

LAGOON_NAMESPACE_BEGIN

/// Executes an unary functor on each element of a range
/** This function calls the @a func on each element of
 *  the @a range passed as the first argument
 *
 *  @param range the range to be traversed
 *  @param func the functor to be called on each element of the range
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename Functor>
Functor for_each(Range range, Functor func)
{
	while(!range.empty())
	{
		func(range.front());
		range.step_front();
	}
	return func;
}

LAGOON_NAMESPACE_END

#endif //include guard

