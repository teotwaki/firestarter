/**
 * @file mirror/ct_string/char_type.hpp
 * @brief Implementation of the char_type meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_CHAR_TYPE_1105240825_HPP
#define MIRROR_CT_STRING_CHAR_TYPE_1105240825_HPP

#include <mirror/ct_string/default.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

template <typename Char, Char ... C>
struct char_type<basic_string<Char, C...> >
{
	typedef Char type;
};

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

