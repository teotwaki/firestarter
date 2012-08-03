/**
 * @file mirror/ct_string/head.hpp
 * @brief Implementation of the head meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_HEAD_1105240825_HPP
#define MIRROR_CT_STRING_HEAD_1105240825_HPP

#include <mirror/ct_string/default.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {
namespace aux {

template <
	typename Count,
	typename Done,
	typename Head,
	typename Tail
> struct head_hlp;

template <int N, typename Char, Char ... C>
struct head_hlp<
	std::integral_constant<int, N>,
	std::false_type,
	basic_string<Char, C...>,
	basic_string<Char>
>
{
	typedef basic_string<Char, C...> type;
};

template <typename Char, Char ... Hn, Char ... Tn>
struct head_hlp<
	std::integral_constant<int, 0>,
	std::true_type,
	basic_string<Char, Hn...>,
	basic_string<Char, Tn...>
>
{
	typedef basic_string<Char, Hn...> type;
};

template <int N, typename Char, Char C, Char ... Hn, Char ... Tn>
struct head_hlp<
	std::integral_constant<int, N>,
	std::false_type,
	basic_string<Char, Hn...>,
	basic_string<Char, C, Tn...>
> : head_hlp<
	std::integral_constant<int, N - 1>,
	std::integral_constant<bool, N - 1 == 0>,
	basic_string<Char, Hn..., C>,
	basic_string<Char, Tn...>
> { };

} // namespace aux

template <typename Length, typename Char, Char ... C>
struct head<
	basic_string<Char, C...>,
	Length
> : aux::head_hlp<
	std::integral_constant<int, Length::value>,
	std::integral_constant<bool, Length::value == 0>,
	basic_string<Char>,
	basic_string<Char, C...>
> { };

template <typename CTString, int Length>
struct head_c
 : head<
	typename CTString::type,
	std::integral_constant<int, Length>
> { };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

