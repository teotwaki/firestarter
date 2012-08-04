/**
 * @file mirror/ct_string/empty.hpp
 * @brief Implementation of the empty and nonempty meta-functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_EMPTY_1105240825_HPP
#define MIRROR_CT_STRING_EMPTY_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace cts {

template <typename Char, Char ... C>
struct empty<basic_string<Char, C...> >
 : std::integral_constant<bool, sizeof ... (C) == 0>
{ };

template <typename Char, Char ... C>
struct nonempty<basic_string<Char, C...> >
 : std::integral_constant<bool, sizeof ... (C) != 0>
{ };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

