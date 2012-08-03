/**
 * @file lagoon/range/until.hpp
 * @brief Range modification stopping the traversal when a predicate
 *  is satisfied.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_UNTIL_1011291729_HPP
#define LAGOON_RANGE_UNTIL_1011291729_HPP

#include <lagoon/range/utils.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN

namespace aux {

template <class Range, class Predicate>
struct preceding : public leaping_base
{
private:
	Range range;
	Predicate predicate;
	bool done;

	bool is_done(void) const
	{
		return range.empty() ? true : predicate(range.front());
	}
public:
	preceding(preceding&& other)
	 : range(std::move(other.range))
	 , predicate(std::move(other.predicate))
	 , done(other.done)
	{ }

	preceding(const Range& rng, const Predicate& pred)
	 : range(rng)
	 , predicate(pred)
	 , done(is_done())
	{ }

	preceding(Range&& rng, Predicate&& pred)
	 : range(std::forward(rng))
	 , predicate(std::forward(pred))
	 , done(is_done())
	{ }

	inline bool empty(void) const
	{
		return done || range.empty();
	}

	void step_front(void)
	{
		assert(!empty());
		range.step_front();
		done = is_done();
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
		const preceding& a,
		const preceding& b
	)
	{
		return same_position(a.range, b.range);
	}
};

} // namespace aux


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Adapts the @a range to end when a predicate is satisfied
/**
 *  @tparam Range The type of the underlying range
 *  @tparam Predicate The type of the terminating predicate
 *  @param range the underlying range
 *  @param predicate the predicate whose satisfying stops the traversal
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename Predicate>
UnspecifiedRange until(Range range, Predicate predicate);
#else
template <class Range, typename Predicate>
inline aux::preceding<Range, Predicate>
until(Range range, Predicate predicate)
{
	return aux::preceding<Range, Predicate>(range, predicate);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

