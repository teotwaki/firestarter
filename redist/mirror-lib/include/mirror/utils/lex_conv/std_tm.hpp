/**
 * .file mirror/utils/lex_conv/std_tm.hpp
 * .brief lexical converter for the std::tm type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_CONV_STD_TM_1011291729_HPP
#define MIRROR_UTILS_LEX_CONV_STD_TM_1011291729_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <string>
#include <ctime>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <>
class _lex_conv<std::string, std::tm>
{
private:
	static void reinit(std::tm& res)
	{
		// see note in finish()
		res.tm_sec = 0;
		res.tm_min = 0;
		res.tm_hour = 0;
		res.tm_mday = 0;
		res.tm_mon = 0;
		res.tm_year = 0;
		res.tm_wday = 0;
		res.tm_yday = 0;
		res.tm_isdst = 0;
	}

	template <typename Iterator>
	static bool parse_iso8601(Iterator& i, Iterator e, std::tm& res)
	{
		using boost::spirit::qi::_1;
		using boost::spirit::qi::char_;
		using boost::spirit::qi::uint_;
		using boost::spirit::qi::uint_parser;
		using boost::spirit::qi::parse;
		using boost::phoenix::ref;
		//
		reinit(res);
		// 2,3,4-digit unsigned integer parsers
		auto qi_uint_2 = uint_parser<uint, 10, 2, 2>();
		//TODO:
		//auto qi_uint_3 = uint_parser<uint, 10, 3, 3>();
		auto qi_uint_4 = uint_parser<uint, 10, 4, 4>();
		// year, month and month day parsers with semantic actions
		auto iso8601year = qi_uint_4[ref(res.tm_year) = _1 - 1900];
		auto iso8601mon  = qi_uint_2[ref(res.tm_mon) = _1 - 1];
		auto iso8601mday = qi_uint_2[ref(res.tm_mday) = _1];
		// hour, minute and day parsers with semantic actions
		auto iso8601hour = qi_uint_2[ref(res.tm_hour) = _1];
		auto iso8601min  = qi_uint_2[ref(res.tm_min) = _1];
		auto iso8601sec  = (
			qi_uint_2[ref(res.tm_sec) = _1] >>
			-(char_('.') >> uint_)
		);
		// the date parser
		auto iso8601date = (
			(iso8601year >> iso8601mon >> iso8601mday) |
			(iso8601year >> '-' >> iso8601mon >> '-' >> iso8601mday)
		);
		// the time parser
		auto iso8601time = (
			(iso8601hour >> ':' >> iso8601min >> ':' >> iso8601sec)
		);
		// the time-zone parser (see note in finish())
		bool neg_diff = false;
		auto iso8601tz = (
			(char_('z') | 'Z') |
			(
				(char_('+') | char_('-')[ref(neg_diff) = true]) >>
				qi_uint_2[ref(res.tm_isdst) += _1 * 100] >> -(
					-char_(':') >>
					qi_uint_2[ref(res.tm_isdst) += _1]
				)
			)
		);
		auto iso8601time_tz = iso8601time >> -iso8601tz;
		// the iso8601 date-time string parsers
		auto iso8601 =
			iso8601date >> -((char_('T') | ' ') >> iso8601time_tz) ;
		// do the parsing
		if(parse(i, e, iso8601))
		{
			if(neg_diff) res.tm_isdst = -res.tm_isdst;
			return true;
		}
		return false;
	}

	static void finish(std::tm& res)
	{
		// NOTE: we are using the tm_isdst member here to pass
		// on timezone time difference
		int shift = (res.tm_isdst);
		res.tm_isdst = -1;
		res.tm_hour -= shift / 100;
		res.tm_min  -= shift % 100;
		// let mktime to adjust the resulting std::tm
		std::mktime(&res);
	}
public:
	template <typename Iterator>
	std::tm convert(Iterator& i, Iterator e) const
	{
		std::tm result = {0};
		if(!parse_iso8601(i, e, result))
			throw std::bad_cast();
		finish(result);
		return result;
	}
};

template <>
class _lex_conv<std::tm, std::string>
{
private:
	const char* format_str(void) const
	{
		// all std::tm instances are considered
		// to be in UTC
		return "%Y-%m-%dT%H:%M:%SZ";
		// TODO: switch to other formats ?
	}
public:
	inline std::string operator()(const std::tm& value) const
	{
		char buf[128];
		std::strftime(
			buf, sizeof(buf),
			format_str(),
			&value
		);
		return buf;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

