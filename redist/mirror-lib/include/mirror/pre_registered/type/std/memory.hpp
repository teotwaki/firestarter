/**
 * @file mirror/pre_registered/type/std/memory.hpp
 * @brief Pre-registration of the standard smart pointers with Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_STD_MEMORY_1106021348_HPP
#define MIRROR_PRE_REGISTERED_TYPE_STD_MEMORY_1106021348_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <memory>

MIRROR_REG_BEGIN

MIRROR_REG_TYPE_TEMPLATE(std, shared_ptr)
MIRROR_REG_TYPE_TEMPLATE(std, unique_ptr)

MIRROR_REG_END

#endif //include guard

