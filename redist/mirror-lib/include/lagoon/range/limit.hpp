/**
 * @file lagoon/range/limit.hpp
 * @brief Limitng the range to N elements
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_LIMIT_1011291729_HPP
#define LAGOON_RANGE_LIMIT_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN
namespace aux {

template <class Range>
struct limited
{
private:
	Range range;
	size_t remaining;

public:
	limited(limited&& other)
	 : range(std::move(other.range))
	 , remaining(other.remaining)
	{ }

	limited(const Range& rng, size_t limit)
	 : range(rng)
	 , remaining(limit)
	{ }

	limited(Range&& rng, size_t limit)
	 : range(std::forward(rng))
	 , remaining(limit)
	{ }

	inline bool empty(void) const
	{
		return (remaining == 0) || range.empty();
	}

	void step_front(void)
	{
		assert(remaining != 0);
		--remaining;
		range.step_front();
	}

	size_t leap_front(size_t amount)
	{
		if(amount > remaining)
			amount = remaining;
		return range.leap_front(amount);

	}

	inline auto front(void) const -> decltype(range.front())
	{
		assert(remaining != 0);
		return range.front();
	}

	friend inline bool same_position(
		const limited& a,
		const limited& b
	)
	{
		return same_position(a.range, b.range);
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Limits the range to contain maximum of n elements
/** Adapts the range so that it contains maximally @a count elements
 *  or less if the adapted range is shorter
 *
 *  @param range the range to be limited
 *  @param count the upper limit of the count of elements
 *
 *  @ingroup lagoon_ranges
 */
template <class Range>
UnspecifiedRange limit(Range range, size_t count);
#else
template <class Range>
inline aux::limited<Range> limit(Range range, size_t count)
{
	return aux::limited<Range>(range, count);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

