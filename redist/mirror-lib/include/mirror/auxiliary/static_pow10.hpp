/**
 *  @file mirror/auxiliary/static_pow10.hpp
 *  @brief Template that allows to calculate the "integral power of 10"
 *  from the given compile-time integral constant.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_STATIC_POW10_1011291729_HPP
#define MIRROR_AUX_STATIC_POW10_1011291729_HPP

#include <mirror/auxiliary/static_log10.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <
	huge_uint Boundary,
	huge_uint Value,
	huge_uint Pow,
	bool Found
> struct pow10_helper;

template <
	huge_uint Boundary,
	huge_uint Value,
	huge_uint Pow
> struct pow10_helper<Boundary, Value, Pow, true>
 : std::integral_constant< huge_uint, Pow>
{ };

template <
	huge_uint Boundary,
	huge_uint Value,
	huge_uint Pow
> struct pow10_helper<Boundary, Value, Pow, false>
 : pow10_helper<
	Boundary + 1,
	Value,
	Pow * 10,
	Boundary == Value
> { };

template <huge_uint Boundary, huge_uint Value, huge_uint Pow>
struct calc_pow10
 : pow10_helper<
	Boundary + 1,
	Value,
	Pow,
	Boundary == Value
>{ };


template <huge_uint Value>
struct pow10
 : calc_pow10< 0, Value, 1>
{ };

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

