/**
 * @file mirror/ct_string/size.hpp
 * @brief Implementation of the size and length meta-functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_SIZE_1105240825_HPP
#define MIRROR_CT_STRING_SIZE_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace cts {

template <typename Char, Char ... C>
struct size<basic_string<Char, C...> >
 : std::integral_constant<int, sizeof ... (C)>
{ };

template <typename Char, Char ... C>
struct length<basic_string<Char, C...> >
 : std::integral_constant<int, sizeof ... (C)>
{ };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

