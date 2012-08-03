/**
 * @file mirror/ct_string/compare.hpp
 * @brief Implementation of compile-time string comparison meta-functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_COMPARE_1105240825_HPP
#define MIRROR_CT_STRING_COMPARE_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace cts {

template <typename Char, Char ... C>
struct equal<basic_string<Char, C...>, basic_string<Char, C...> >
 : std::true_type
{ };

template <typename Char, Char ... C1, Char ... C2>
struct equal<basic_string<Char, C1...>, basic_string<Char, C2...> >
 : std::false_type
{ };

template <typename Char, Char ... C>
struct nonequal<basic_string<Char, C...>, basic_string<Char, C...> >
 : std::false_type
{ };

template <typename Char, Char ... C1, Char ... C2>
struct nonequal<basic_string<Char, C1...>, basic_string<Char, C2...> >
 : std::true_type
{ };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

