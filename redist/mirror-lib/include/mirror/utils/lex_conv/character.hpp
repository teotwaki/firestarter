/**
 * .file mirror/utils/lex_conv/character.hpp
 * .brief lexical converter for the character types
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_CONV_CHARACTER_1011291729_HPP
#define MIRROR_UTILS_LEX_CONV_CHARACTER_1011291729_HPP

#include <string>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <>
class _lex_conv<std::string, char>
{
public:
	template <typename Iterator>
	char convert(Iterator i, Iterator e) const
	{
		if(i + 1 == e) return *i;
		throw std::bad_cast();
	}
};

template <>
class _lex_conv<char, std::string>
{
public:
	inline std::string operator()(char value) const
	{
		const char tmp[2] = {value, '\0'};
		return std::string(tmp);
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

