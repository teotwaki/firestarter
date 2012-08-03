/**
 * @file mirror/pre_registered/class/std/list.hpp
 * @brief Pre-registration of the standard list template class with Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_STD_LIST_1102040952_HPP
#define MIRROR_PRE_REGISTERED_CLASS_STD_LIST_1102040952_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/meta_std_container.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <mirror/pre_registered/type/std/allocator.hpp>
#include <mirror/pre_registered/type/std/initializer_list.hpp>
#include <list>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(typename Element),
	struct, std, list, (Element)
)
MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
	MIRROR_REG_INITLIST_CONSTRUCTOR(public, Element, values)
	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 1)
		MIRROR_REG_CONSTRUCTOR_PARAM(size_t, count)
		MIRROR_REG_CONSTRUCTOR_PARAM(Element, value)
	MIRROR_REG_CONSTRUCTOR_END(1)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN(
	(typename Element),
	std, list, (Element), Element
)
	MIRROR_REG_STD_REV_CONTAINER_TRAVERSALS

	typedef mirror::mp::range<
		MIRROR_REG_STD_CONTAINER_OP(locator, begin),
		MIRROR_REG_STD_CONTAINER_OP(locator, end)
	> locators;

	typedef mirror::mp::range<
		MIRROR_REG_STD_CONTAINER_OP(inserter, push_front),
		MIRROR_REG_STD_CONTAINER_OP(inserter, push_back),
		MIRROR_REG_STD_CONTAINER_OP(inserter, insert_pos),
		MIRROR_REG_STD_CONTAINER_OP(inserter, insert_pos_multi)
	> inserters;

	typedef  mirror::mp::range<
		MIRROR_REG_STD_CONTAINER_OP(eraser, clear),
		MIRROR_REG_STD_CONTAINER_OP(eraser, pop_front),
		MIRROR_REG_STD_CONTAINER_OP(eraser, pop_back),
		MIRROR_REG_STD_CONTAINER_OP(eraser, erase_pos),
		MIRROR_REG_STD_CONTAINER_OP(eraser, erase_range),
		MIRROR_REG_STD_CONTAINER_OP(eraser, remove_val)
	> erasers;
MIRROR_REG_STD_CONTAINER_END

MIRROR_REG_END

#endif //include guard

