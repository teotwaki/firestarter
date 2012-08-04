/**
 * @file mirror/pre_registered/class/std/pair.hpp
 * @brief Pre-registration of the standard pair template class with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_STD_PAIR_1012140551_HPP
#define MIRROR_PRE_REGISTERED_CLASS_STD_PAIR_1012140551_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <utility>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(typename First, typename Second),
	struct, std, pair, (First, Second)
)
MIRROR_REG_CLASS_MEM_VARS_BEGIN
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, first)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, second)
MIRROR_REG_CLASS_MEM_VARS_END

MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 0)
		MIRROR_REG_CONSTRUCTOR_PARAM(_, first)
		MIRROR_REG_CONSTRUCTOR_PARAM(_, second)
	MIRROR_REG_CONSTRUCTOR_END(0)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_END

#endif //include guard

