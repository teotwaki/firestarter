/**
 * @file mirror/ct_string/at.hpp
 * @brief Implementation of the at meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_AT_1105240825_HPP
#define MIRROR_CT_STRING_AT_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/front.hpp>
#include <mirror/ct_string/skip_front.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

#ifndef MIRROR_DOCUMENTATION_ONLY

template <typename CTString, typename Position>
struct at
 : front<skip_front<CTString, Position> >
{ };

template <typename CTString, int Position>
struct at_c
 : front<skip_front_c<CTString, Position> >
{ };

#endif // !MIRROR_DOCUMENTATION_ONLY

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

