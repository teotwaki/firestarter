/**
 * @file mirror/ct_string/find.hpp
 * @brief Implementation of the find meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_FIND_1105240825_HPP
#define MIRROR_CT_STRING_FIND_1105240825_HPP

#include <mirror/ct_string/default.hpp>
#include <mirror/ct_string/starts_with.hpp>
#include <mirror/ct_string/empty.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace cts {
namespace aux {

template <
	typename Temp,
	typename Searched,
	typename Found,
	typename Failed
> struct find_hlp;

template <typename Char>
struct find_hlp<
	basic_string<Char>,
	basic_string<Char>,
	std::true_type,
	std::true_type
>
{
	typedef basic_string<Char> type;
};

template <
	typename Char,
	Char ... CTn,
	Char ... CSn
> struct find_hlp<
	basic_string<Char, CTn...>,
	basic_string<Char, CSn...>,
	std::true_type,
	std::false_type
>
{
	typedef basic_string<Char, CTn...> type;
};

template <
	typename Char,
	Char ... CTn,
	Char ... CSn
> struct find_hlp<
	basic_string<Char, CTn...>,
	basic_string<Char, CSn...>,
	std::false_type,
	std::true_type
>
{
	typedef basic_string<Char> type;
};

template <
	typename Char,
	Char CT,
	Char ... CTn,
	Char ... CSn
> struct find_hlp<
	basic_string<Char, CT, CTn...>,
	basic_string<Char, CSn...>,
	std::false_type,
	std::false_type
> : find_hlp<
	basic_string<Char, CTn...>,
	basic_string<Char, CSn...>,
	typename starts_with<
		basic_string<Char, CTn...>,
		basic_string<Char, CSn...>
	>::type,
	typename empty<basic_string<Char, CTn...> >::type
>
{ };

} // namespace aux

template <typename Char, Char ... C1n, Char ... C2n>
struct find<
	basic_string<Char, C1n...>,
	basic_string<Char, C2n...>
> : aux::find_hlp<
	basic_string<Char, C1n...>,
	basic_string<Char, C2n...>,
	typename starts_with<
		basic_string<Char, C1n...>,
		basic_string<Char, C2n...>
	>::type,
	typename empty<basic_string<Char, C1n...> >::type
> { };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

