/**
 * @file mirror/utils/lex_match.hpp
 * @brief lexical matcher for the native types defined in
 * mirror/utils/native_types.hpp
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_MATCH_1011291729_HPP
#define MIRROR_UTILS_LEX_MATCH_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/lex_match/base.hpp>
#include <mirror/utils/lex_match/bool.hpp>
#include <mirror/utils/lex_match/std_tm.hpp>

MIRROR_NAMESPACE_BEGIN

/// Lexical matcher
/** This unary functor returns a double value (0.0 - 1.0)
 *  estimating how a string input matches a type literal
 */
template <typename T>
struct lex_match : public aux::_lex_match<T>
{
	inline double operator()(const std::string& str) const
	{
		std::string::const_iterator i = str.begin();
		return this->match(i, str.end());
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

