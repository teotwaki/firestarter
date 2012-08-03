/**
 * @file mirror/utils/lex_conv.hpp
 * @brief lexical converter for the native types defined in
 * mirror/utils/native_types.hpp
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_CONV_1011291729_HPP
#define MIRROR_UTILS_LEX_CONV_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/lex_conv/base.hpp>
#include <mirror/utils/lex_conv/bool.hpp>
#include <mirror/utils/lex_conv/character.hpp>
#include <mirror/utils/lex_conv/std_tm.hpp>

MIRROR_NAMESPACE_BEGIN

/// Lexical converter
/** This unary functor does lexical conversion between the Source
 *  and the Target type.
 */
template <typename Source, typename Target>
struct lex_conv;

template <typename Source, typename Target>
struct lex_conv : public aux::_lex_conv<Source, Target>
{
};

template <typename Target>
struct lex_conv<std::string, Target>
 : public aux::_lex_conv<std::string, Target>
{
	inline Target operator()(const std::string& value) const
	{
		std::string::const_iterator i = value.begin();
		return this->convert(i, value.end());
	}
};

template <typename T>
struct lex_conv<T, T>
{
	inline T operator()(T value) const
	{
		return value;
	}
};

template <>
struct lex_conv<std::string, std::string>
{
	template <typename Iterator>
	inline std::string convert(Iterator b, Iterator e) const
	{
		return std::string(b, e);
	}

	inline std::string operator()(std::string value) const
	{
		return value;
	}
};


MIRROR_NAMESPACE_END

#endif //include guard

