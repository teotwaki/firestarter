/**
 * .file puddle/auxiliary/ct_string.hpp
 * .brief The Mirror compile-time string wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_CT_STRING_1106061232_HPP
#define PUDDLE_AUXILIARY_CT_STRING_1106061232_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/base_wrap.hpp>
#include <mirror/ct_string.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

#define PUDDLE_HLPR_IMPL_SIMPLE_CT_STRING_MEM_FN(WRAPPER, NAME) \
	static WRAPPER< \
		typename mirror::cts::NAME<CTString>::type \
	> NAME(void) \
	{ \
		return WRAPPER< \
			typename mirror::cts::NAME< \
				CTString \
			>::type \
		>(); \
	}

#define PUDDLE_HLPR_IMPL_IT_PAR_CT_STRING_MEM_FN(WRAPPER, NAME) \
	template <typename Int, Int Index> \
	static WRAPPER< \
		typename mirror::cts::NAME< \
			CTString, \
			std::integral_constant<Int, Index> \
		>::type \
	> NAME(integer<std::integral_constant<Int, Index> >) \
	{ \
		return integer< \
			typename mirror::cts::NAME< \
				CTString, \
				std::integral_constant<Int, Index> \
			>::type \
		>(); \
	}


#define PUDDLE_HLPR_IMPL_IC_PAR_CT_STRING_MEM_FN(WRAPPER, NAME) \
	template <int Index> \
	static WRAPPER< \
		typename mirror::cts::NAME<CTString, Index>::type \
	> NAME(void) \
	{ \
		return WRAPPER< \
			typename mirror::cts::NAME< \
				CTString, \
				Index \
			>::type \
		>(); \
	}

#define PUDDLE_HLPR_IMPL_2S_PAR_CT_STRING_MEM_FN(WRAPPER, NAME) \
	template <typename CTString_> \
	static WRAPPER< \
		typename mirror::cts::NAME<CTString, CTString_>::type \
	> NAME(ct_string_<CTString_>) \
	{ \
		return WRAPPER< \
			typename mirror::cts::NAME< \
				CTString, \
				CTString_ \
			>::type \
		>(); \
	}

template <typename CTString>
struct ct_string_
{
	static const char* c_str(void)
	{
		return mirror::cts::c_str<CTString>();
	}

	template <typename CTString_>
	friend auto operator == (ct_string_, ct_string_<CTString_>) ->
	integer<
		typename mirror::cts::equal<
			CTString,
			CTString_
		>::type
	>
	{
		return integer<
			typename mirror::cts::equal<
				CTString,
				CTString_
			>::type
		>();
	}

	template <typename CTString_>
	friend auto operator != (ct_string_, ct_string_<CTString_>) ->
	integer<
		typename mirror::cts::nonequal<
			CTString,
			CTString_
		>::type
	>
	{
		return integer<
			typename mirror::cts::nonequal<
				CTString,
				CTString_
			>::type
		>();
	}

	template <typename CTString_>
	friend ct_string_<
		typename mirror::cts::concat<
			CTString,
			CTString_
		>::type
	> operator + (ct_string_, ct_string_<CTString_>)
	{
		return ct_string_<
			typename mirror::cts::concat<
				CTString,
				CTString_
			>::type
		>();
	}

	PUDDLE_HLPR_IMPL_SIMPLE_CT_STRING_MEM_FN(integer, empty)
	PUDDLE_HLPR_IMPL_SIMPLE_CT_STRING_MEM_FN(integer, size)
	PUDDLE_HLPR_IMPL_SIMPLE_CT_STRING_MEM_FN(integer, front)
	PUDDLE_HLPR_IMPL_IT_PAR_CT_STRING_MEM_FN(integer, at)
	PUDDLE_HLPR_IMPL_IC_PAR_CT_STRING_MEM_FN(integer, at_c)

	PUDDLE_HLPR_IMPL_2S_PAR_CT_STRING_MEM_FN(integer, starts_with)
	PUDDLE_HLPR_IMPL_2S_PAR_CT_STRING_MEM_FN(integer, ends_with)
	PUDDLE_HLPR_IMPL_2S_PAR_CT_STRING_MEM_FN(integer, contains)
	PUDDLE_HLPR_IMPL_SIMPLE_CT_STRING_MEM_FN(ct_string_, step_front)
	PUDDLE_HLPR_IMPL_IT_PAR_CT_STRING_MEM_FN(ct_string_, skip_front)
	PUDDLE_HLPR_IMPL_IC_PAR_CT_STRING_MEM_FN(ct_string_, skip_front_c)
	PUDDLE_HLPR_IMPL_IT_PAR_CT_STRING_MEM_FN(ct_string_, head)
	PUDDLE_HLPR_IMPL_IC_PAR_CT_STRING_MEM_FN(ct_string_, head_c)
	PUDDLE_HLPR_IMPL_IT_PAR_CT_STRING_MEM_FN(ct_string_, tail)
	PUDDLE_HLPR_IMPL_IC_PAR_CT_STRING_MEM_FN(ct_string_, tail_c)
	PUDDLE_HLPR_IMPL_2S_PAR_CT_STRING_MEM_FN(ct_string_, find)
};

#undef PUDDLE_HLPR_IMPL_SIMPLE_CT_STRING_MEM_FN
#undef PUDDLE_HLPR_IMPL_IC_PAR_CT_STRING_MEM_FN
#undef PUDDLE_HLPR_IMPL_IT_PAR_CT_STRING_MEM_FN
#undef PUDDLE_HLPR_IMPL_2S_PAR_CT_STRING_MEM_FN

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Function for constructing compile-time strings
/**
 *  @see CompileTimeString
 */
template <char ... Chars>
CompileTimeString ct_string(void);
#else
template <char ... Chars>
aux::ct_string_<mirror::cts::string<Chars...> >
ct_string(void)
{
	return aux::ct_string_<mirror::cts::string<Chars...> >();
}
#endif

PUDDLE_NAMESPACE_END

#endif //include guard
