/**
 * .file mirror/utils/script_factory/default_traits.hpp
 * .brief Default traits for the script parser-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_FACTORY_DEFAULT_TRAITS_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_FACTORY_DEFAULT_TRAITS_1011291729_HPP

#include <mirror/utils/lex_match.hpp>
#include <mirror/utils/lex_conv.hpp>
#include <mirror/utils/string/unescape.hpp>
#include <string>
#include <cctype>
#include <set>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <typename Iterator>
class script_fact_matcher
{
private:
	struct deref
	{
		template <typename Iter>
		inline char operator()(Iter& i, Iter e) const
		{
			assert(i != e);
			return *i;
		}
	};

	struct skip_space_deref
	{
		template <typename Iter>
		inline char operator()(Iter& i, Iter e) const
		{
			assert(i != e);
			skip_space(i, e);
			if(i == e) return '\0';
			return *i;
		}
	};

	template <typename Iter1, typename Iter2, typename Transform>
	static bool do_match(
		Iter1& ai, Iter1 ae,
		Iter2  bi, Iter2 be,
		Transform transf
	)
	{
		Iter1 tmp = ai;
		while((ai != ae) && (bi != be))
		{
			if(transf(ai, ae) != transf(bi, be))
			{
				ai = tmp;
				return false;
			}
			++ai;
			++bi;
		}
		if(bi != be)
		{
			ai = tmp;
			return false;
		}
		return true;
	}

	static bool match_quotmark(Iterator& i, Iterator e)
	{
		return match(i, e, "'") || match(i, e, "\"");
	}

	static void skip_lit_char(Iterator& i, Iterator e)
	{
		while(i != e)
		{
			match(i, e, ".") ||
			match(i, e, "-") ||
			match(i, e, "+");
			if(std::isalnum(*i)) ++i;
			else break;
		}
	}

	static bool skip_char(Iterator& i, Iterator e)
	{
		// TODO: handle utf-8 multi-byte characters
		if(i == e) return false;
		++i;
		return true;
	}
public:
	typedef Iterator iterator;

	template <typename Iter>
	static void skip_space(Iter& i, Iter e)
	{
		while((i != e) && std::isspace(*i)) ++i;
	}

	static bool match(Iterator& i, Iterator e, const std::string& s)
	{
		return do_match(i, e, s.begin(), s.end(), deref());
	}

	template <size_t N>
	static bool match(Iterator& i, Iterator e, const char s[N])
	{
		return do_match(i, e, s, s+N-1, deref());
	}

	static bool match_skip_space(
		Iterator& i,
		Iterator e,
		const std::string& s
	)
	{
		return do_match(
			i, e,
			s.begin(), s.end(),
			skip_space_deref()
		);
	}

	template <typename T>
	static double match_lit(
		Iterator& i,
		Iterator e,
		Iterator& lb,
		Iterator& le,
		T*
	)
	{
		// remember the start position
		lb = i;
		le = i;
		skip_lit_char(le, e);
		lex_match<T> matcher;
		double result = matcher.match(i, le);
		return result;
	}

	template <typename T>
	static double match_quoted_lit(
		Iterator& i,
		Iterator e,
		Iterator& lb,
		Iterator& le,
		T*
	)
	{
		// remember the start position
		Iterator tmp = i;
		// try to match a quotation mark
		if(!match_quotmark(i, e)) return 0.0;
		// this is the begin of the literal
		lb = i;
		Iterator li = lb;
		while(1)
		{
			// check for escape character
			bool escape = match(i, e, "\\");
			// this is a potential end of the literal
			le = i;
			// check for the closing quotation mark
			bool quotmark = match_quotmark(i, e);
			// if there was an unescaped quotation mark
			if(quotmark && !escape)
			{
				// try to match the actual literal
				lex_match<T> matcher;
				double res = matcher.match(li, le);
				// if there is a complete match
				// return the result
				if((res > 0.0) && (li == le))
					return res;
				else break;
			}
			if(!skip_char(i, e)) break;
		}
		i = tmp;
		return 0.0;
	}

	static double match_lit(
		Iterator& begin,
		Iterator end,
		Iterator& lit_begin,
		Iterator& lit_end,
		char* x
	)
	{
		return match_quoted_lit(begin, end, lit_begin, lit_end, x);
	}

	static double match_lit(
		Iterator& begin,
		Iterator end,
		Iterator& lit_begin,
		Iterator& lit_end,
		std::string* x
	)
	{
		return match_quoted_lit(begin, end, lit_begin, lit_end, x);
	}

	static double match_lit(
		Iterator& begin,
		Iterator end,
		Iterator& lit_begin,
		Iterator& lit_end,
		std::tm* x
	)
	{
		return match_quoted_lit(begin, end, lit_begin, lit_end, x);
	}

	template <typename T>
	static T convert(Iterator begin, Iterator end, T*)
	{
		lex_conv<std::string, T> conv;
		return conv.convert(begin, end);
	}

	static std::string convert(Iterator begin, Iterator end, std::string*)
	{
		return unescape(begin, end);
	}
};

template <typename Iterator>
class script_fact_ext_matcher
{
protected:
	template <class Matcher, typename T>
	static double match_expr(
		Matcher&,
		Iterator& i,
		Iterator e,
		Iterator& eb,
		Iterator& ee,
		int& expr_id,
		T*
	)
	{
		expr_id = -1;
		return 0.0;
	}
public:
	template <typename T>
	static T get(Iterator begin, Iterator end, int expr_id, T*)
	{
		assert(!"Never should get here!");
		return *((T*)nullptr);
	}
};

class script_fact_strcache
{
public:
	class shared_data
	{
	private:
		std::set<std::string> shared_strings;
	public:
		std::set<std::string>& strings(void)
		{
			return shared_strings;
		}

		std::set<std::string>::const_iterator share(
			const std::string& str
		)
		{
			return shared_strings.insert(str).first;
		}
	};

	typedef typename std::set<std::string>::const_iterator
		shared_string_proxy;

	static inline const std::string& get_shared_string(
		shared_data& data,
		shared_string_proxy proxy
	)
	{
		return *proxy;
	}

	static inline shared_string_proxy share_string(
		shared_data& data,
		const std::string& str
	)
	{
		return data.share(str);
	}
};

class script_fact_translator
{
public:
	struct shared_data
	{
	};

	static inline std::string translate(
		shared_data&,
		std::string str
	)
	{
		return str;
	}
};

} // namespace aux

// The general traits for the script factory plugin
template <
	class Matcher,
	class ExternalMatcher,
	class StringCache,
	class Translator
>
class script_fact_traits
 : public Matcher
 , public ExternalMatcher
 , public StringCache
 , public Translator
{
public:
	class shared_data
	 : public StringCache::shared_data
	 , public Translator::shared_data
	{
	};

	template <typename T>
	double match_expr(
		typename Matcher::iterator& i,
		typename Matcher::iterator  e,
		typename Matcher::iterator& eb,
		typename Matcher::iterator& ee,
		int& expr_id,
		T* selector
	) const
	{
		return ExternalMatcher::match_expr(
			(Matcher&)*this,
			i, e,
			eb, ee,
			expr_id,
			selector
		);
	}
};

template <typename Iterator>
struct script_fact_def_traits
 : public script_fact_traits<
	aux::script_fact_matcher<Iterator>,
	aux::script_fact_ext_matcher<Iterator>,
	aux::script_fact_strcache,
	aux::script_fact_translator
> { };

template <typename Iterator, template <class> class ExternalMatcher>
struct script_fact_custom_traits
 : public script_fact_traits<
	aux::script_fact_matcher<Iterator>,
	ExternalMatcher<Iterator>,
	aux::script_fact_strcache,
	aux::script_fact_translator
> { };

MIRROR_NAMESPACE_END

#endif //include guard

