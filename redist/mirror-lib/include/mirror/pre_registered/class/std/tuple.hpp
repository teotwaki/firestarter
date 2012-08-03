/**
 * @file mirror/pre_registered/class/std/tuple.hpp
 * @brief Pre-registration of the standard tuple template class with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_STD_TUPLE_1101111212_HPP
#define MIRROR_PRE_REGISTERED_CLASS_STD_TUPLE_1101111212_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/pre_registered/namespace/std.hpp>
#include <mirror/auxiliary/static_int_to_str.hpp>
#include <utility>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(typename ...Types),
	struct, std, tuple, (Types...)
)
	typedef mp::range<Types...> attr_range;
	typedef typename mp::size<attr_range>::type attr_count;

	template <int I>
	static spec_public_tag member_access(std::integral_constant<int, I>);

	template <int I>
	static member<spec_public_tag, spec___tag, mp::at_c<attr_range, I> >
	attr_traits (std::integral_constant<int, I>);

	template <int I>
	static const char* attr_name(std::integral_constant<int, I>)
	{
		return aux::int_to_str_getter_prefix<I, '_'>::get();
	}

	template <int I>
	static size_t attr_name_length(std::integral_constant<int, I>)
	{
		return aux::int_to_str_getter_prefix<I, '_'>::length::value;
	}

	template <int I>
	static auto get(std::integral_constant<int, I>, const _this& t)
	 -> decltype(std::get<I>(t))
	{
		return std::get<I>(t);
	}

	template <int I>
	static void set(
		std::integral_constant<int, I>,
		_this& t,
		const typename mp::at_c<attr_range, I>::type& val
	)
	{
		std::get<I>(t) = val;
	}

	template <int I>
	static std::true_type has_address(std::integral_constant<int, I>);

	template <int I>
	static auto address(std::integral_constant<int, I>, _this& t)
	 -> decltype(&std::get<I>(t))
	{
		return &std::get<I>(t);
	}

	template <int I>
	static auto address(std::integral_constant<int, I>, const _this& t)
	 -> decltype(&std::get<I>(t))
	{
		return &std::get<I>(t);
	}

	//TODO:
	template <int I>
	static void* mem_ptr(std::integral_constant<int, I>)
	{
		return nullptr;
	}

MIRROR_REG_CONSTRUCTORS_BEGIN
	ctr_index_init;
	struct ctr_params_init
	{
		template <int I>
		static mp::at_c<attr_range, I>
		type(std::integral_constant<int, I>);

		template <int I>
		static const char* name(std::integral_constant<int, I> idx)
		{
			return attr_name(idx);
		}

		template <int I>
		static size_t name_length(std::integral_constant<int, I> idx)
		{
			return attr_name_length(idx);
		}

		typedef attr_count size;
	};
	static ctr_params_init ctr_params(ctr_index_init);
	static spec_public_tag ctr_access(ctr_index_init);
	typedef std::integral_constant<int, ctr_index_init::value + 1>
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

MIRROR_REG_END

#endif //include guard

