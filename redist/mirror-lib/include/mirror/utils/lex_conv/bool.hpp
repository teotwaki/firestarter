/**
 * .file mirror/utils/lex_conv/bool.hpp
 * .brief lexical converter for the boolean type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_CONV_BOOL_1011291729_HPP
#define MIRROR_UTILS_LEX_CONV_BOOL_1011291729_HPP

#include <string>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <>
class _lex_conv<std::string, bool>
{
public:
	template <typename Iterator>
	bool convert(Iterator& i, Iterator e) const
	{
		if(i != e)
		{
			bool b;
			if(!boost::spirit::qi::parse(i, e, b))
			{
				int j;
				if(!boost::spirit::qi::parse(i, e, j))
				{
					double d;
					if(boost::spirit::qi::parse(i, e, d))
						return d != 0.0 ? true : false;

				}
				else return j != 0 ? true : false;
			}
			else return b;
		}
		throw std::bad_cast();
	}
};

template <>
class _lex_conv<bool, std::string>
{
public:
	inline std::string operator()(bool value) const
	{
		return value ? "true" : "false";
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

