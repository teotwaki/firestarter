/**
 * @file lagoon/range/fold.hpp
 * @brief Applying successively a binary functor on a state and every
 * element in a range.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_ACCUMULATE_1011291729_HPP
#define LAGOON_RANGE_ACCUMULATE_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>

LAGOON_NAMESPACE_BEGIN

/// Accumulating the results of calling a binary functor on the elements
/** Returns the result of successive application of a binary forward
 *  operation on the result of previous invocations and every element
 *  in the @a range.
 *
 *  @param range the range to be folded
 *  @param state the initial state used in the first invocation
 *  @param op the functor to be called on each element of the range
 *
 *  @ingroup lagoon_ranges
 */
template <typename State, class Range, typename BinaryOp>
State fold(Range range, State state, BinaryOp op)
{
	while(!range.empty())
	{
		state = op(state, range.front());
		range.step_front();
	}
	return state;
}

/// Accumulating the results of calling a binary functor on the elements
/** Returns the result of successive application of a binary forward
 *  operation on the result of previous invocations and every element
 *  in the @a range.
 *
 *  @param range the range to be folded
 *  @param state_ref reference to the state variable gradually modified by op
 *  @param op the functor to be called on each element of the range
 *
 *  @ingroup lagoon_ranges
 */
template <typename State, class Range, typename BinaryOp>
State& fold_ref(Range range, State& state_ref, BinaryOp op)
{
	while(!range.empty())
	{
		op(state_ref, range.front());
		range.step_front();
	}
	return state_ref;
}


/// Accumulating the results of calling a binary functor on the elements
/** Returns the result of successive application of a binary forward
 *  operation on the result of previous invocations and every element
 *  in the @a range. This version uses three different binary functors;
 *  one for the first element, one for the last element and one for the
 *  remaining elements. If the range has only one element the initial
 *  functor is called.
 *
 *  @param range the range to be folded
 *  @param state_ref reference to the state variable gradually modified by op
 *  @param initial the functor to be called on the first element of the range
 *  @param op the functor to be called on each element of the range except
 *    the first and the last
 *  @param final the functor to be called on the last element of the range
 *
 *  @ingroup lagoon_ranges
 */
template <
	typename State,
	class Range,
	typename InitialOp,
	typename DefaultOp,
	typename FinalOp
> State& fold_ref(
	Range range,
	State& state_ref,
	InitialOp initial,
	DefaultOp op,
	FinalOp final
)
{
	if(!range.empty())
	{
		initial(state_ref, range.front());
		range.step_front();
		bool done = range.empty();
		while(!done)
		{
			auto front = range.front();
			range.step_front();
			done = range.empty();
			if(done)
			{
				final(state_ref, front);
				break;
			}
			else op(state_ref, front);
		}
	}
	return state_ref;
}

LAGOON_NAMESPACE_END

#endif //include guard

