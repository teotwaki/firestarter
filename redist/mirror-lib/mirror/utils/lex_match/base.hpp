/**
 * .file mirror/utils/lex_match/base.hpp
 * .brief default implementation of the lexical matcher
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_MATCH_BASE_1011291729_HPP
#define MIRROR_UTILS_LEX_MATCH_BASE_1011291729_HPP

#include <boost/spirit/include/support_ascii.hpp>
#include <boost/spirit/include/qi_auto.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <sstream>

MIRROR_NAMESPACE_BEGIN

namespace aux {

template <typename T>
struct _lex_match
{
	template <typename Iterator>
	double match(Iterator& begin, Iterator end) const
	{
		double result = 0.0;
		if(boost::spirit::qi::parse(
			begin, end,
			boost::spirit::qi::create_parser<T>()
			[boost::phoenix::ref(result) = 1.0]
		) && (begin == end)) return result;
		else return 0.0;
	}
};

template <>
struct _lex_match<std::string>
{
	template <typename Iterator>
	double match(Iterator& begin, Iterator end) const
	{
		begin = end;
		return 1.0;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

