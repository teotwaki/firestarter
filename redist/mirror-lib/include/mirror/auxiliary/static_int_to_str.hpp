/**
 *  @file mirror/auxiliary/static_int_to_str.hpp
 *  @brief Template that allows to calculate the "integral decadic
 *  logarithm" from the given compile-time integral constant.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_STATIC_INT_TO_STR_1011291729_HPP
#define MIRROR_AUX_STATIC_INT_TO_STR_1011291729_HPP

#include <mirror/auxiliary/static_log10.hpp>
#include <mirror/auxiliary/static_pow10.hpp>

#include <cassert>

MIRROR_NAMESPACE_BEGIN
namespace aux {

struct int2str_base
{
protected:
	static inline char* do_convert(
		char * const str,
		size_t max_len,
		size_t limit,
		huge_uint value
	)
	{
		// we need at least two characters int the dest string
		assert(max_len > 1);
		// get the most significant digit and put it
		// into the first char of the string ..
		*str = '0' + (value / limit);
		// if this was the last character
		// (one is remaining for the terminator)
		if(max_len == 2)
		{
			// terminate the string
			str[1] = '\0';
		}
		else
		{
			// otherwise chop off the most significant digit
			// and convert the rest of the number
			do_convert(
				str + 1,
				max_len - 1,
				limit / 10,
				value % limit
			);
		}
		return str;
	}
};

template <huge_uint Value>
struct int_to_str : int2str_base
{
	typedef int2str_base base_class;

	// The lenght of the string (without the terminating null)
	typedef std::integral_constant<
		huge_uint,
		1 + log10<Value>::value
	> length;

	static inline char* convert(char* const str, size_t max_len)
	{
		typedef typename pow10< log10<Value>::value >::type limit;
		return base_class::do_convert(
			str,
			max_len,
			limit::value,
			Value
		);
	}
};

// Specialization of int_to_str for zero
template <>
struct int_to_str<0>
{
	// The lenght of the string
	typedef std::integral_constant<huge_uint, 1> length;

	// Converts the compile time int into the given string
	static char* convert(char* str, size_t max_len)
	{
		assert(max_len > 1);
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
};

template <huge_uint Value>
struct int_to_str_getter : int_to_str<Value>
{
	typedef int_to_str<Value> base_class;

	// The length of the returned string
	// (without the terminating null)
	typedef typename base_class::length length;

	// Gets a string with the given compile time integral value
	static const char* get(void)
	{
		// the resulting string having the proper size
		static char result[length::value + 1] = {0};
		// if the result is not initialized yet
		if(!result[0])
		{
			// convert the string into the result
			return base_class::convert(
				result,
				length::value + 1
			);
		}
		// now just return the result
		return result;
	}
};

template <huge_uint Value, char ... prefix>
struct int_to_str_getter_prefix : int_to_str<Value>
{
	typedef int_to_str<Value> base_class;

	typedef std::integral_constant<
		huge_uint,
		sizeof...(prefix)
	> prefix_length;
	// The length of the returned string
	// (without the terminating null)
	typedef std::integral_constant<
		huge_uint,
		prefix_length::value + base_class::length::value
	> length;

	// Gets a string with the given compile time integral value
	static const char* get(void)
	{
		// the resulting string having the proper size
		static char result[length::value + 1] = {prefix..., 0};
		// if the result is not initialized yet
		if(!result[prefix_length::value])
		{
			// convert the string into the result
			base_class::convert(
				result + prefix_length::value,
				base_class::length::value + 1
			);
		}
		// now just return the result
		return result;
	}
};


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

