/**
 * .file mirror/utils/lex_match/bool.hpp
 * .brief default implementation of the lexical matcher
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_MATCH_BOOL_1011291729_HPP
#define MIRROR_UTILS_LEX_MATCH_BOOL_1011291729_HPP

#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_bool.hpp>
#include <boost/spirit/include/qi_char.hpp>

MIRROR_NAMESPACE_BEGIN

namespace aux {

template <>
struct _lex_match<bool>
{
	template <typename Iterator>
	double match(Iterator& begin, Iterator end) const
	{
		double result = 0.0;
		auto resref = boost::phoenix::ref(result);
		if(boost::spirit::qi::parse(
			begin, end,
			boost::spirit::qi::bool_[resref = 1.0] |
			boost::spirit::qi::uint_[resref = 0.5] |
			boost::spirit::qi::char_('0')[resref = 0.8] |
			boost::spirit::qi::char_('1')[resref = 0.8]
		) && (begin == end)) return result;
		return 0.0;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

