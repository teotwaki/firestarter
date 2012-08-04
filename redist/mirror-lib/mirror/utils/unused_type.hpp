/**
 * @file mirror/utils/unused_type.hpp
 * @brief A type for unused parameters
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_UNUSED_TYPE_1011291729_HPP
#define MIRROR_UTILS_UNUSED_TYPE_1011291729_HPP

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

struct unused_type
{
	unused_type(void)
	{ }

	template <typename T>
	unused_type(const T&)
	{ }

	template <typename T>
	const unused_type& operator = (const T&) const
	{
		return *this;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

