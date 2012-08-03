/**
 * @file mirror/ct_string/slice.hpp
 * @brief Implementation of the slice meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_SLICE_1105240825_HPP
#define MIRROR_CT_STRING_SLICE_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/head.hpp>
#include <mirror/ct_string/tail.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

#ifndef MIRROR_DOCUMENTATION_ONLY

template <typename CTString, typename Start, typename Length>
struct slice
 : tail<
	head<
		CTString,
		std::integral_constant<
			int,
			Start::value + Length::value
		>
	>,
	Length
> { };

template <typename CTString, int Start, int Length>
struct slice_c
 : slice<
	CTString,
	std::integral_constant<int, Start>,
	std::integral_constant<int, Length>
> { };

#endif

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

