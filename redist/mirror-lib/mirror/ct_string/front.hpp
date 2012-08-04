/**
 * @file mirror/ct_string/front.hpp
 * @brief Implementation of the front meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_FRONT_1105240825_HPP
#define MIRROR_CT_STRING_FRONT_1105240825_HPP

#include <mirror/ct_string/default.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

template <typename Char>
struct front<basic_string<Char> >
 : std::integral_constant<Char, 0>
{ };

template <typename Char, Char C, Char ... Cn>
struct front<basic_string<Char, C, Cn...> >
 : std::integral_constant<Char, C>
{ };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

