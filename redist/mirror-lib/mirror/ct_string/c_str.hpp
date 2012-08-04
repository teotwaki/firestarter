/**
 * @file mirror/ct_string/c_str.hpp
 * @brief Implementation of the c_str function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_C_STR_1105240825_HPP
#define MIRROR_CT_STRING_C_STR_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/evaluate.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {
namespace aux {

template <typename Char, Char ... C>
const Char* c_str(const basic_string<Char, C...>&)
{
	static const Char res[] = { C ... , Char('\0')};
	return res;
}

} // namespace aux

/// A function returning a C-null-terminated-string for a CT string
/** This function returns a null-terminated C-string for the compile-time
 *  string, passed as the @a CTString template parameter.
 *
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename CTString>
auto c_str(void) ->
decltype(aux::c_str(typename evaluate<CTString>::type()))
{
	return aux::c_str(typename evaluate<CTString>::type());
}

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

