/**
 * @file mirror/pre_registered/type/std/tm.hpp
 * @brief Pre-registration of the native C++ string types with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_STD_TM_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_STD_TM_1011291729_HPP

#include <mirror/meta_type.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <ctime>

MIRROR_REG_BEGIN

MIRROR_REG_TYPE_CTS(std, tm, ('t', 'm'))
namespace _class {
template <> struct _<std::tm>
 : _defaults_native<std::tm, mirror::spec_struct_tag > { };
} // namespace _class

MIRROR_REG_END

#endif //include guard

