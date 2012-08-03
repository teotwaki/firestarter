/**
 * @file mirror/pre_registered/type/std/string.hpp
 * @brief Pre-registration of the native C++ string types with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_STD_STRING_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_STD_STRING_1011291729_HPP

#include <mirror/meta_type.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <string>

MIRROR_REG_BEGIN

MIRROR_REG_TYPE_CTS(std, string, ('s','t','r','i','n','g'))
MIRROR_REG_TYPE_CTS(std, wstring, ('w','s','t','r','i','n','g'))

namespace _class {
template <> struct _<std::string>
 : _defaults_native<std::string, mirror::spec_type_tag > { };
template <> struct _<std::wstring>
 : _defaults_native<std::wstring, mirror::spec_type_tag > { };
} // namespace _class

MIRROR_REG_END

#endif //include guard

