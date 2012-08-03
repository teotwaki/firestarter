/**
 * @file mirror/ct_string/skip_front.hpp
 * @brief Implementation of the skip_front meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_SKIP_FRONT_1105240825_HPP
#define MIRROR_CT_STRING_SKIP_FRONT_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/tail.hpp>
#include <mirror/ct_string/size.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

#ifndef MIRROR_DOCUMENTATION_ONLY

template <typename CTString, typename Length>
struct skip_front
 : tail<
	typename CTString::type,
	std::integral_constant<
		int,
		length<CTString>::value - Length::value
	>
> { };

template <typename CTString, int Count>
struct skip_front_c
 : skip_front<
	typename CTString::type,
	std::integral_constant<int, Count>
>{ };

#endif

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

