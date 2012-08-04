/**
 * @file lagoon/range/link.hpp
 * @brief Range modification linking several ranges together
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_LINK_1011291729_HPP
#define LAGOON_RANGE_LINK_1011291729_HPP

#include <lagoon/range/utils.hpp>
#include <cassert>
#include <list>

LAGOON_NAMESPACE_BEGIN

namespace aux {

template <class Range>
class linked : public leaping_base
{
private:
	// the linked list of ranges
	std::list<Range> ranges;

	void skip_empty(void)
	{
		while(!ranges.empty())
		{
			if(ranges.front().empty())
				ranges.pop_front();
			else break;
		}
	}
public:
	linked(void){ }

	/// Constructor taking the pack of ranges to be linked
	linked(const std::initializer_list<Range>& rngs)
	 : ranges(rngs)
	{
		skip_empty();
	}

	/// Returns true if the range is empty
	bool empty(void) const
	{
		return ranges.empty();
	}

	/// Moves the front of the range one step forward
	void step_front(void)
	{
		assert(!empty());
		ranges.front().step_front();
		skip_empty();
	}

	size_t leap_front(size_t leap)
	{
		return leaping_base::do_leap_front(*this, leap);
	}

	/// Returns the value at the front of the range
	auto front(void) const -> decltype(ranges.front().front())
	{
		assert(!empty());
		return ranges.front().front();
	}

	/// Appends the passed range to the end of this range
	void append(const Range& rng)
	{
		if(!rng.empty()) ranges.push_back(rng);
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Links the homogenous @a ranges into a single range
/** Returns a single range that contains a sequence of elements
 *  from the provided @a ranges. The ranges must have the same type.
 *
 *  @tparam Range the type the of ranges to be linked
 *  @param ranges the list of ranges to be linked
 *
 *  @ingroup lagoon_ranges
 */
template <class Range>
UnspecifiedRange link(const std::initializer_list<Range>& ranges);
#else

// link ranges
template <class Range>
aux::linked<Range> link(const std::initializer_list<Range>& ranges)
{
	return aux::linked<Range>(ranges);
}

#endif

LAGOON_NAMESPACE_END

#endif //include guard

