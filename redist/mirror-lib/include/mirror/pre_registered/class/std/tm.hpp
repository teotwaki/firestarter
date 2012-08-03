/**
 * @file mirror/pre_registered/class/std/tm.hpp
 * @brief Pre-registration of the C++ tm struct with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_STD_TM_1012140553_HPP
#define MIRROR_PRE_REGISTERED_CLASS_STD_TM_1012140553_HPP

#include <mirror/meta_class.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <ctime>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_BEGIN(struct, std, tm)
MIRROR_REG_CLASS_MEM_VARS_BEGIN
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_sec)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_min)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_hour)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_mday)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_mon)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_year)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_wday)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_yday)
	MIRROR_REG_CLASS_MEM_VAR(_, _, _, tm_isdst)
MIRROR_REG_CLASS_MEM_VARS_END
MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_END

#endif //include guard

