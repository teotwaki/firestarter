/**
 * @file lagoon/range/offset.hpp
 * @brief Offsetting the range by N elements
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_OFFSET_1011291729_HPP
#define LAGOON_RANGE_OFFSET_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Offsets the beginning of the range by N elements
/** Adapts the range so that the front is moved by @a count elements
 *  or less if the adapted range is shorter. In the latter case
 *  the resulting range will be empty.
 *
 *  @param range the range to be offset
 *  @param amount the numer of elements to offset the range by
 *
 *  @ingroup lagoon_ranges
 */
template <class Range>
inline Range offset(Range range, size_t amount);
#else
template <class Range>
inline Range offset(Range range, size_t amount)
{
	range.leap_front(amount);
	return range;
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

