/**
 * @file mirror/pre_registered/namespace/std.hpp
 * @brief Pre-registration of the ::std namespace with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_NAMESPACE_STD_1011291729_HPP
#define MIRROR_PRE_REGISTERED_NAMESPACE_STD_1011291729_HPP

#include <mirror/meta_namespace.hpp>

MIRROR_REG_BEGIN

// Pre register the ::std namespace with Mirror
MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE_CTS(std, ('s','t','d'))

MIRROR_REG_END

#endif //include guard

