/**
 * @file mirror/auxiliary/constructors.hpp
 * @brief Internal implementation of constructors
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_CONSTRUCTORS_1011291729_HPP
#define MIRROR_AUX_CONSTRUCTORS_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>
#include <string>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <typename Type>
struct meta_ctr_base_base
{
private:
	typedef mirror::_type::_<Type> base_meta_type;
public:
	typedef base_meta_type scope;

	static inline const char* name(void)
	{
		typedef mirror::_type::_<Type> base_meta_type;
		return base_meta_type::name();
	}

	static inline size_t name_length(void)
	{
		typedef mirror::_type::_<Type> base_meta_type;
		return base_meta_type::name_length();
	}
};

template <typename Type, int Index>
class meta_constructor_base
 : public meta_ctr_base_base<Type>
{
private:
	typedef mirror::_type::_<Type> base_meta_type;
	typedef mirror::_class::_<Type> base_meta_class;

	typedef decltype(base_meta_class::ctr_params(
		std::integral_constant<int, Index>()
	)) ctr_params;
public:
	typedef base_meta_type scope;
};

template <class Class>
struct constructor_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_constructor<
				Class,
				Indices
			>...
		> type;
	};
};

template <typename Type, int ConstrIndex, int ParamIndex>
struct meta_ctr_param_base
{
private:
	typedef mirror::_class::_<Type> base_meta_type;
public:
	typedef meta_constructor_base<Type, ConstrIndex> scope;

	static const char* name(void)
	{
		typedef std::integral_constant<int, ConstrIndex>
			constr_index;
		typedef std::integral_constant<int, ParamIndex>
			param_index;
		typedef decltype(base_meta_type::ctr_params(
			constr_index()
		)) ctr_params;
		return ctr_params::name(param_index());
	}

	static size_t name_length(void)
	{
		typedef std::integral_constant<int, ConstrIndex>
			constr_index;
		typedef std::integral_constant<int, ParamIndex>
			param_index;
		typedef decltype(base_meta_type::ctr_params(
			constr_index()
		)) ctr_params;
		return ctr_params::name_length(param_index());
	}
};

template <typename Type>
struct meta_initlzr_param_base
{
	typedef meta_ctr_base_base<Type> scope;

	static const char* name(void)
	{
		return "";
	}

	static size_t name_length(void)
	{
		return 0;
	}
};

template <class Class, int ConstrIndex>
struct ctr_param_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_constructor_param<
				Class,
				ConstrIndex,
				Indices
			>...
		> type;
	};
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

