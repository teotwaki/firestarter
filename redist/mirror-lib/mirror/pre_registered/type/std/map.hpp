/**
 * @file mirror/pre_registered/type/std/map.hpp
 * @brief Pre-registration of the standard map container with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_STD_MAP_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_STD_MAP_1011291729_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <mirror/pre_registered/type/std/allocator.hpp>
#include <map>

MIRROR_REG_BEGIN

MIRROR_REG_TYPE_TEMPLATE(std, map)

MIRROR_REG_END

#endif //include guard

