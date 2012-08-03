/**
 * @file lagoon/range/transform.hpp
 * @brief Transforming the elements of the range by a functor
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_TRANSFORM_1011291729_HPP
#define LAGOON_RANGE_TRANSFORM_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN

namespace aux {

template <class Range, typename Transform>
struct transformed
{
private:
	Range range;
	Transform transform;
public:
	transformed(transformed&& other)
	 : range(std::move(other.range))
	 , transform(std::move(other.transform))
	{ }

	transformed(const transformed& other)
	 : range(other.range)
	 , transform(other.transform)
	{ }

	transformed(const Range& rng, const Transform& transf)
	 : range(rng)
	 , transform(transf)
	{ }

	transformed(Range&& rng, Transform&& transf)
	 : range(std::forward(rng))
	 , transform(std::forward(transf))
	{ }

	inline bool empty(void) const
	{
		return range.empty();
	}

	inline void step_front(void)
	{
		range.step_front();
	}

	inline size_t leap_front(size_t amount)
	{
		return range.leap_front(amount);

	}

	inline auto front(void) const -> decltype(transform(range.front()))
	{
		return transform(range.front());
	}

	friend inline bool same_position(
		const transformed& a,
		const transformed& b
	)
	{
		return same_position(a.range, b.range);
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Transforms the elements in the range by a functor
/** Returns a range containing the elements of the original range
 *  transformed by the unary functor.
 *
 *  @param range the range to be transformed
 *  @param transf the functor transforming the elements
 *
 *  @ingroup lagoon_ranges
 */
template <class Range, typename Transform>
UnspecifiedRange transform(Range range, Transform transf);
#else
template <class Range, typename Transform>
inline aux::transformed<Range, Transform>
transform(Range range, Transform transf)
{
	return aux::transformed<Range, Transform>(range, transf);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

