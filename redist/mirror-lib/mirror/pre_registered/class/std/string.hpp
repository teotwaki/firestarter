/**
 * @file mirror/pre_registered/class/std/string.hpp
 * @brief Pre-registration of the native C++ string classes with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_STD_STRING_1012140552_HPP
#define MIRROR_PRE_REGISTERED_CLASS_STD_STRING_1012140552_HPP

#include <mirror/meta_class.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <string>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_BEGIN(class, std, string)
MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)

	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 0)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::string, s)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::size_t, pos)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::size_t, n)
	MIRROR_REG_CONSTRUCTOR_END(0)

	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 1)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::size_t, n)
		MIRROR_REG_CONSTRUCTOR_PARAM(char, c)
	MIRROR_REG_CONSTRUCTOR_END(1)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_CLASS_BEGIN(class, std, wstring)
MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)

	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 0)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::wstring, s)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::size_t, pos)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::size_t, n)
	MIRROR_REG_CONSTRUCTOR_END(0)

	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 1)
		MIRROR_REG_CONSTRUCTOR_PARAM(::std::size_t, n)
		MIRROR_REG_CONSTRUCTOR_PARAM(wchar_t, c)
	MIRROR_REG_CONSTRUCTOR_END(1)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_END

#endif //include guard

