/**
 * @file mirror/ct_string/basic_string.hpp
 * @brief Declaration of the compile-time basic_string
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_BASIC_STRING_1105240825_HPP
#define MIRROR_CT_STRING_BASIC_STRING_1105240825_HPP

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

/// A compile-time basic-string template for various characted types
/**
 *  @see string
 *  @see empty
 *  @see size
 *  @see concat
 *  @see front
 *  @see at
 *  @see head
 *  @see tail
 *  @see slice
 *  @see starts_with
 *  @see ends_with
 *  @see contains
 *  @see find
 *  @see equal
 *  @see evaluate
 *
 *  @ingroup ct_string
 */
template <typename Char, Char ... C>
struct basic_string
{
	typedef basic_string<Char, C...> type;
};

/// A specialization of the basic compile-time string using @c char type
/**
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <char ... C>
struct string
 : basic_string<char, C...>
{ };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

