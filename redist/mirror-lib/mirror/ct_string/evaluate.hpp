/**
 * @file mirror/ct_string/evaluate.hpp
 * @brief Implementation of the evaluate meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_EVALUATE_1105240825_HPP
#define MIRROR_CT_STRING_EVALUATE_1105240825_HPP

#include <mirror/ct_string/basic_string.hpp>
#include <mirror/meta_prog/nil_type.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

/// Meta-function evaluates a compile-time string expression
/**
 *  @see basic_string
 *
 *  @ingroup ct_string
 */
template <typename CTString>
struct evaluate
#ifndef MIRROR_DOCUMENTATION_ONLY
 : evaluate<typename CTString::type>
{ };
#else
{
	/// The compile-time string that is the result of the expression
	typedef basic_string<Char, ...> type;
};
#endif

template <typename Char, Char ... C>
struct evaluate<basic_string<Char, C...> >
{
	typedef basic_string<Char, C...> type;
};

template <>
struct evaluate<mirror::mp::nil_t>
{
	typedef basic_string<char> type;
};

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

