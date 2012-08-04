/**
 * @file mirror/pre_registered/class/std/map.hpp
 * @brief Pre-registration of the standard set template class with Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_STD_MAP_1102040952_HPP
#define MIRROR_PRE_REGISTERED_CLASS_STD_MAP_1102040952_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/meta_std_container.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <mirror/pre_registered/type/std/allocator.hpp>
#include <mirror/pre_registered/type/std/functional.hpp>
#include <mirror/pre_registered/type/std/initializer_list.hpp>
#include <mirror/pre_registered/class/std/pair.hpp>
#include <map>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(typename Key, typename Value),
	struct, std, map, (Key, Value)
)
MIRROR_REG_TYPE_ALIAS((std::pair<Key,Value>), value_type)
MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
	MIRROR_REG_INITLIST_CONSTRUCTOR(public, value_type, values)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_END

#endif //include guard

