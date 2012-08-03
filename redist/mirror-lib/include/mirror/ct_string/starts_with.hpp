/**
 * @file mirror/ct_string/starts_with.hpp
 * @brief Implementation of the starts_with meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_STARTS_WITH_1105240825_HPP
#define MIRROR_CT_STRING_STARTS_WITH_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/compare.hpp>
#include <mirror/ct_string/head.hpp>
#include <mirror/ct_string/size.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

#ifndef MIRROR_DOCUMENTATION_ONLY

template <typename CTString1, typename CTString2>
struct starts_with
 : equal<
	head<CTString1, length<CTString2> >,
	CTString2
> { };

#endif

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

