/**
 * @file lagoon/range/only_if.hpp
 * @brief Finding the first element satisfying a predicate
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_ONLY_IF_1011291729_HPP
#define LAGOON_RANGE_ONLY_IF_1011291729_HPP

#include <lagoon/range/utils.hpp>

LAGOON_NAMESPACE_BEGIN

namespace aux {

template <class Range, class Predicate>
struct satisfying_predicate : public leaping_base
{
private:
	Range range;
	Predicate predicate;

	// Skips those elements which do not satisfy the predicate
	void skip(void)
	{
		// while the range is not empty and the current element
		// does not satisfy the predicate step forward
		while(!range.empty() && !predicate(range.front()))
			range.step_front();
	}
public:
	satisfying_predicate(const satisfying_predicate& other)
	 : range(other.range)
	 , predicate(other.predicate)
	{
		skip();
	}

	satisfying_predicate(satisfying_predicate&& other)
	 : range(std::move(other.range))
	 , predicate(std::move(other.predicate))
	{
		skip();
	}

	satisfying_predicate(const Range& rng, const Predicate& pred)
	 : range(rng)
	 , predicate(pred)
	{
		skip();
	}

	satisfying_predicate(Range&& rng, Predicate&& pred)
	 : range(std::forward(rng))
	 , predicate(std::forward(pred))
	{
		skip();
	}

	inline bool empty(void) const
	{
		return range.empty();
	}

	void step_front(void)
	{
		range.step_front();
		skip();
	}

	size_t leap_front(size_t leap)
	{
		return leaping_base::do_leap_front(*this, leap);
	}

	inline auto front(void) const -> decltype(range.front())
	{
		return range.front();
	}

	friend inline bool same_position(
		const satisfying_predicate& a,
		const satisfying_predicate& b
	)
	{
		return same_position(a.range, b.range);
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Adapts the range to contain only elements satisfying a predicate
/** Traverses a @a range checking the result of a @a predicate
 *  on each element. Skips the elements not satisfying the predicate.
 *
 *  @param range the range to be searched
 *  @param predicate the predicate to be used to filter the range
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename Predicate>
UnspecifiedRange only_if(Range range, Predicate predicate);
#else
template <class Range, typename Predicate>
inline aux::satisfying_predicate<Range, Predicate>
only_if(Range range, Predicate predicate)
{
	return aux::satisfying_predicate<Range, Predicate>(range, predicate);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

