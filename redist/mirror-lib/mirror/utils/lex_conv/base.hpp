/**
 * .file mirror/utils/lex_conv/base.hpp
 * .brief default implementation of the lexical converter
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_CONV_BASE_1011291729_HPP
#define MIRROR_UTILS_LEX_CONV_BASE_1011291729_HPP

#include <boost/spirit/include/support_ascii.hpp>
#include <boost/spirit/include/qi_auto.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/karma_auto.hpp>
#include <boost/spirit/include/karma_format.hpp>
#include <boost/spirit/include/karma_numeric.hpp>
#include <sstream>
#include <stdexcept>

MIRROR_NAMESPACE_BEGIN

namespace aux {

template <typename Source, typename Target>
class _lex_conv;

template <typename Target>
class _lex_conv<std::string, Target>
{
public:
	template <typename Iterator>
	inline Target convert(Iterator& i, Iterator e) const
	{
		Target result = Target();
		if(!boost::spirit::qi::parse(i, e, result))
			throw std::bad_cast();
		return result;
	}
};

template <typename Source>
class _lex_conv<Source, std::string>
{
private:
	typedef Source source_param;
public:
	inline std::string operator()(source_param value) const
	{
		std::stringstream result;
		result << boost::spirit::karma::format(
			boost::spirit::karma::create_generator<Source>(),
			value
		);
		return result.str();
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

