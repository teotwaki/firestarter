/**
 * @file mirror/ct_string.hpp
 * @brief All-in-one inclusion of compile-time strings and algorithms
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_1105241212_HPP
#define MIRROR_CT_STRING_1105241212_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/evaluate.hpp>
#include <mirror/ct_string/char_type.hpp>
#include <mirror/ct_string/empty.hpp>
#include <mirror/ct_string/size.hpp>
#include <mirror/ct_string/compare.hpp>
#include <mirror/ct_string/head.hpp>
#include <mirror/ct_string/tail.hpp>
#include <mirror/ct_string/slice.hpp>
#include <mirror/ct_string/front.hpp>
#include <mirror/ct_string/step_front.hpp>
#include <mirror/ct_string/skip_front.hpp>
#include <mirror/ct_string/at.hpp>
#include <mirror/ct_string/contains.hpp>
#include <mirror/ct_string/find.hpp>
#include <mirror/ct_string/concat.hpp>
#include <mirror/ct_string/starts_with.hpp>
#include <mirror/ct_string/ends_with.hpp>
//
#include <mirror/ct_string/c_str.hpp>


MIRROR_NAMESPACE_BEGIN

/** @defgroup ct_string Mirror - Compile-time strings
 *
 *  Mirror optionally provides the identifier names also as compile-time
 *  strings which can be used in compile-time meta-programs.
 */

/// Contains the compile-time string-related code
namespace cts {

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

