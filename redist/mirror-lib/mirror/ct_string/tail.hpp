/**
 * @file mirror/ct_string/tail.hpp
 * @brief Implementation of the tail meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_TAIL_1105240825_HPP
#define MIRROR_CT_STRING_TAIL_1105240825_HPP

#include <mirror/ct_string/default.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {
namespace aux {

template <
	typename Count,
	typename Done,
	typename Head,
	typename Tail
> struct tail_hlp;

template <int N, typename Char, Char ... C>
struct tail_hlp<
	std::integral_constant<int, N>,
	std::false_type,
	basic_string<Char, C...>,
	basic_string<Char>
>
{
	typedef basic_string<Char> type;
};

template <int N, typename Char, Char ... Hn, Char ... Tn>
struct tail_hlp<
	std::integral_constant<int, N>,
	std::true_type,
	basic_string<Char, Hn...>,
	basic_string<Char, Tn...>
>
{
	typedef basic_string<Char, Tn...> type;
};

template <int N, typename Char, Char C, Char ... Hn, Char ... Tn>
struct tail_hlp<
	std::integral_constant<int, N>,
	std::false_type,
	basic_string<Char, Hn...>,
	basic_string<Char, C, Tn...>
> : tail_hlp<
	std::integral_constant<int, N - 1>,
	std::integral_constant<bool, N - 1 == 0>,
	basic_string<Char, Hn..., C>,
	basic_string<Char, Tn...>
> { };

} // namespace aux

template <typename Length, typename Char, Char ... C>
struct tail<
	basic_string<Char, C...>,
	Length
> : aux::tail_hlp<
	std::integral_constant<
		int,
		length<basic_string<Char, C...> >::value -
		Length::value
	>,
	std::integral_constant<
		bool,
		length<basic_string<Char, C...> >::value <=
		Length::value
	>,
	basic_string<Char>,
	basic_string<Char, C...>
> { };

template <typename CTString, int Length>
struct tail_c
 : tail<
	typename CTString::type,
	std::integral_constant<int, Length>
> { };

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

