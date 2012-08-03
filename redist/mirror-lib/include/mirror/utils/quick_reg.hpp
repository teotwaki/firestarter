/**
 * @file mirror/utils/quick_reg.hpp
 * @brief Boost.Preprocessor-based quick registering macros
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_QUICK_REG_1011291729_HPP
#define MIRROR_UTILS_QUICK_REG_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/meta_class.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>

MIRROR_NAMESPACE_BEGIN
/** @defgroup quick_registering_macros Mirror - Quick-registering macros
 *
 *  Even if flexible, the regular registering macros can be little cumbersome
 *  for some registering tasks. For example the registering of a POD class
 *  involves specifying all member variables and the default, copy and structure
 *  initializer constructors, each by a separate macro invocation. The quick
 *  registering macros simplify the registering of some special constructs
 *  like POD classes, enums, etc. at the cost of a dependency on an external
 *  library (Boost.Preprocessor).
 *
 *  More on registering can be found in the @ref mirror_registering section.
 */
MIRROR_NAMESPACE_END

#include <mirror/utils/quick_reg/pod_class.hpp>
#include <mirror/utils/quick_reg/class.hpp>
#include <mirror/utils/quick_reg/enum.hpp>
#include <mirror/utils/quick_reg/free_variable.hpp>

#endif //include guard

