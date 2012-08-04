/**
 * .file mirror/utils/unicode/cp_to_utf8.hpp
 * .brief Conversion of alphanumeric unicode codepoints to utf-8 char sequences
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_UNICODE_CP_TO_UTF8_1011291729_HPP
#define MIRROR_UTILS_UNICODE_CP_TO_UTF8_1011291729_HPP

#include <mirror/config.hpp>

#include <type_traits>
#include <iostream>
#include <stdexcept>
#include <cassert>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class OutStream, size_t N>
void rawuccp2utf8(
	const unsigned char* oct,
	std::integral_constant<size_t, N>,
	OutStream& out
)
{
	throw std::runtime_error("Not implemented");
}

template <class OutStream>
void rawuccp2utf8(
	const unsigned char* oct,
	std::integral_constant<size_t, 3>,
	OutStream& out
)
{
	// 21 bits
	if((oct[0] & 0xE0) == 0x00)
	{
		out << char(
			0xF0 |
			((oct[0] >> 2) & 0x07)
		) << char(
			0x80 |
			((oct[0] << 4) & 0x30) |
			((oct[1] >> 4) & 0x0F)
		) << char(
			0x80 |
			((oct[1] << 2) & 0x3C) |
			((oct[2] >> 6) & 0x03)
		) << char(
			0x80 |
			((oct[2]) & 0x3F)
		);
	}
	else throw std::runtime_error("Not implemented");
}

template <class OutStream>
void rawuccp2utf8(
	const unsigned char* oct,
	std::integral_constant<size_t, 2>,
	OutStream& out
)
{
	// 11 bits
	if((oct[0] & 0xF8) == 0x00)
	{
		out << char(
			0xC0 |
			((oct[0] << 2) & 0x1C) |
			((oct[1] >> 6) & 0x03)
		) << char(
			0x80 |
			((oct[1]) & 0x3F)
		);
	}
	// 16 bits
	else
	{
		out << char(
			0xE0 |
			((oct[0] >> 4) & 0x0F)
		) << char(
			0x80 |
			((oct[0] << 2) & 0x3C) |
			((oct[1] >> 6) & 0x03)
		) << char(
			0x80 |
			((oct[1]) & 0x3F)
		);
	}
}

template <class OutStream>
void rawuccp2utf8(
	const unsigned char* oct,
	std::integral_constant<size_t, 1>,
	OutStream& out
)
{
	if((oct[0] & 0x80) == 0x80)
	{
		out 	<< char(0xC0 | ((oct[0] >> 6) & 0x1F))
			<< char(0x80 | ((oct[0]) & 0x3F));
	}
	else out << (char(oct[0]));
}

inline unsigned char uccphex2raw(const char c)
{
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'A' && c <= 'F') return 10 + c - 'A';
	if(c >= 'a' && c <= 'f') return 10 + c - 'a';
	throw std::runtime_error("Invalid hex digit");
	return 0;
}

template <typename Iterator, class OutStream, size_t N>
void uccp2utf8(
	Iterator cur,
	Iterator end,
	std::integral_constant<size_t, N> n,
	OutStream& out
)
{
	if(size_t(end - cur) > (2 * N))
		throw std::runtime_error("Code point too big");
	unsigned char raw[N];
	unsigned char* raw_cur = raw;
	if((end - cur) % 2 != 0)
	{
		*raw_cur = (uccphex2raw(*cur) & 0x0F);
		++raw_cur;
		++cur;
	}
	for(; raw_cur != raw + N; ++raw_cur)
	{
		assert(!"Internal error!" || cur != end);
		assert(!"Internal error!" || (cur + 1) != end);
		*raw_cur =
			((uccphex2raw(*cur) << 4) & 0xF0) |
			(uccphex2raw(*(cur+1)) & 0x0F);
		cur += 2;
	}
	assert(!"Internal error!" || cur == end);
	rawuccp2utf8(raw, n, out);
}

template <typename Iterator, class OutStream, size_t N>
inline void calluccp2utf8(
	Iterator begin,
	Iterator end,
	size_t n,
	std::integral_constant<size_t, N>,
	std::integral_constant<size_t, N>,
	OutStream& out
)
{
	if(n != N) throw std::runtime_error("Code point too big");
	uccp2utf8(
		begin,
		end,
		std::integral_constant<size_t, N>(),
		out
	);
}

template <typename Iterator, class OutStream, size_t Min, size_t Max>
inline void calluccp2utf8(
	Iterator begin,
	Iterator end,
	size_t n,
	std::integral_constant<size_t, Min> min,
	std::integral_constant<size_t, Max> max,
	OutStream& out
)
{
	typedef std::integral_constant<size_t, (Min + Max) / 2> Mid;
	if(n == Mid::value)
	{
		uccp2utf8(
			begin,
			end,
			Mid(),
			out
		);
	}
	else if(n < Mid::value)
		calluccp2utf8(
			begin,
			end,
			n,
			min,
			std::integral_constant<size_t, Mid::value - 1>(),
			out
		);
	else if(n >  Mid::value)
		calluccp2utf8(
			begin,
			end,
			n,
			std::integral_constant<size_t, Mid::value + 1>(),
			max,
			out
		);
}

template <typename Iterator, class OutStream, size_t N>
inline void calluccp2utf8(
	Iterator begin,
	Iterator end,
	std::integral_constant<size_t, N>,
	OutStream& out
)
{
	calluccp2utf8(
		begin,
		end,
		(end - begin + 1) / 2,
		std::integral_constant<size_t, 1>(),
		std::integral_constant<size_t, N>(),
		out
	);
}

template <typename Iterator, class OutStream>
OutStream& unicode_cp_to_utf8(
	Iterator begin,
	Iterator end,
	OutStream& out
)
{
	if(begin == end) return out;
	// skip leading zero pairs
	while(begin != end)
	{
		if(*begin == '0' && *(begin+1) == '0')
			begin += 2;
		else break;
	}
	if(begin == end) out << '\0';
	else
	{
		// call the output function
		calluccp2utf8(
			begin,
			end,
			std::integral_constant<size_t, 3>(),
			out
		);
	}
	return out;
}

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

