/**
 * .file mirror/utils/string/unescape.hpp
 * .brief String unescaper for backslash escaped characters
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_STRING_UNESCAPE_1011291729_HPP
#define MIRROR_UTILS_STRING_UNESCAPE_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/unicode/cp_to_utf8.hpp>

#include <string>
#include <sstream>
#include <stdexcept>

MIRROR_NAMESPACE_BEGIN
namespace aux {

char unescape_singlebyte(char c)
{
	switch(c)
	{
		case '"':  return '"';
		case '/':  return '/';
		case '\\': return '\\';
		case 'b':  return '\b';
		case 'f':  return '\f';
		case 'n':  return '\n';
		case 'r':  return '\r';
		case 't':  return '\t';
	}
	throw std::runtime_error("Unknown escape character");
	return '\0';
}

template <typename Iterator>
std::string unescape(Iterator cur, Iterator end)
{
	std::stringstream output;
	bool had_escape = false;
	while(cur != end)
	{
		char chr = *cur;
		if(had_escape)
		{
			had_escape = false;
			switch(chr)
			{
				case '\\':
				case '"':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					output << unescape_singlebyte(chr);
					++cur;
					break;
				case 'u':
				{
					Iterator tmp = ++cur;
					for(int x=0; x!=4; ++x)
					{
						if(cur != end) ++cur;
						else break;
					}
					unicode_cp_to_utf8(
						tmp,
						cur,
						output
					);
					if(cur == end) return output.str();
					else break;
				}
				throw std::runtime_error("Unknown escape char");
			}
		}
		else
		{
			if(chr != '\\') output << chr;
			else had_escape = true;
			++cur;
		}
	}
	return output.str();
}

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

