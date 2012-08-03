/**
 *  @file mirror/auxiliary/static_log10.hpp
 *  @brief Template that allows to calculate the "integral decadic
 *  logarithm" from the given compile-time integral constant.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_STATIC_LOG10_1011291729_HPP
#define MIRROR_AUX_STATIC_LOG10_1011291729_HPP

#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

typedef unsigned long long int huge_uint;

template <
	huge_uint Boundary,
	huge_uint Value,
	huge_uint Log,
	bool Found
> struct log10_helper;

template <
	huge_uint Boundary,
	huge_uint Value,
	huge_uint Log
> struct log10_helper<Boundary, Value, Log, true>
 : std::integral_constant< huge_uint, Log>
{ };

template <
	huge_uint Boundary,
	huge_uint Value,
	huge_uint Log
> struct log10_helper<Boundary, Value, Log, false>
 : log10_helper<
	Boundary * 10,
	Value,
	Log + 1,
	(Boundary > Value / 10)
> { };

template <huge_uint Boundary, huge_uint Value, huge_uint Log>
struct calc_log10
 : log10_helper<
	Boundary * 10,
	Value,
	Log,
	(Boundary > Value / 10)
>{ };


template <huge_uint Value>
struct log10
 : calc_log10< 1, Value, 0>
{ };


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

