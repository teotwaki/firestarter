/**
 * @file mirror/ct_string/concat.hpp
 * @brief Implementation of the concat meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_CONCAT_1105240825_HPP
#define MIRROR_CT_STRING_CONCAT_1105240825_HPP

#include <mirror/ct_string/default.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

template <typename Char, Char ... C>
struct concat<basic_string<Char, C...> >
{
	typedef basic_string<Char, C...> type;
};

template <typename Char, Char ... C1, Char ... C2>
struct concat<basic_string<Char, C1...>, basic_string<Char, C2...> >
{
	typedef basic_string<Char, C1..., C2...> type;
};

template <
	typename CTString1,
	typename CTString2,
	typename CTString3,
	typename ... CTStrings
> struct concat<
	CTString1,
	CTString2,
	CTString3,
	CTStrings...
>
{
	typedef typename concat<
		typename concat<CTString1, CTString2>::type,
		CTString3,
		CTStrings...
	>::type type;
};

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

