/**
 * .file mirror/meta_prog/iter_info.hpp
 * .brief The iteration info helper class
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_ITER_INFO_1103161403_HPP
#define MIRROR_META_PROG_ITER_INFO_1103161403_HPP

MIRROR_NAMESPACE_BEGIN
namespace mp {
namespace aux {

// Helper structure containing info about range element iteration
template <
	typename T,
	typename IsFirst,
	typename IsLast
>
struct iteration_info
{
	// The current item in the tranversed range
	typedef T type;

	// compile time boolean constant indicating if
	// this is the first or the last item in the
	// traversed range
	typedef IsFirst is_first;
	typedef IsLast is_last;
};


} // namespace aux
} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

