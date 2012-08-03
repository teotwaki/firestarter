/**
 * @file lagoon/range/utils.hpp
 * @brief Utilites shared by various range modifier implementations
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_UTILS_1011291729_HPP
#define LAGOON_RANGE_UTILS_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN
namespace aux {

class leaping_base
{
protected:
	template <typename Range>
	static inline size_t do_leap_front(Range& range, size_t leap)
	{
		size_t done = 0;
		while(done != leap)
		{
			if(range.empty()) break;
			range.step_front();
			++done;
		}
		return done;
	}
};

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

