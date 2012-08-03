/**
 * @file mirror/auxiliary/member_functions.hpp
 * @brief Internal implementation of member functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_MEMBER_FUNCTIONS_1011291729_HPP
#define MIRROR_AUX_MEMBER_FUNCTIONS_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/specifier_tags.hpp>
#include <string>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <typename Type, int FuncIndex>
class meta_mem_function_base
{
private:
	typedef mirror::_type::_<Type> _base_meta_type;
	typedef mirror::_class::_<Type> _base_meta_class;
public:
	typedef _base_meta_type scope;

	static inline const char* name(void)
	{
		std::integral_constant<int, FuncIndex> idx;
		return _base_meta_class::memfn_name(idx);
	}

	static inline size_t name_length(void)
	{
		std::integral_constant<int, FuncIndex> idx;
		return _base_meta_class::memfn_name_length(idx);
	}
};

template <class Class>
struct mem_ovlded_func_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_ovlded_mem_funcs<
				Class,
				Indices
			>...
		> type;
	};
};

template <class Class, int FuncIndex>
struct mem_function_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_member_function<
				Class,
				FuncIndex,
				Indices
			>...
		> type;
	};
};

// Helper template returning the possibly const qualified target type
// based on the constness of the tag passed as argument
template <typename ConstnessTag, typename T>
struct memfn_inst_type;

template <typename T>
struct memfn_inst_type<spec_non_const_tag, T>
{
	typedef T type;
};

template <typename T>
struct memfn_inst_type<spec___tag, T>
{
	typedef T type;
};

template <typename T>
struct memfn_inst_type<spec_const_tag, T>
{
	typedef const T type;
};

template <typename Type, int FuncIndex, int OvldIndex, int ParamIndex>
struct meta_memfn_param_base
{
private:
	typedef decltype(
		mirror::_class::_<Type>::mem_ovldfn(
			std::integral_constant<int, FuncIndex>()
		).memfn_params(
			std::integral_constant<int, OvldIndex>()
		)
	) _memfn_params;
public:
	typedef meta_mem_function_base<Type, FuncIndex> scope;

	static inline const char* name(void)
	{
		typedef std::integral_constant<int, ParamIndex> param_index;
		return _memfn_params::name(param_index());
	}

	static inline size_t name_length(void)
	{
		typedef std::integral_constant<int, ParamIndex> param_index;
		return _memfn_params::name_length(param_index());
	}
};

// Declaration of the mem_var_helper template (see below)
template <class Class, int FuncIndex, int OvldIndex>
struct memfn_param_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_mem_function_param<
				Class,
				FuncIndex,
				OvldIndex,
				Indices
			>...
		> type;
	};
};


template <typename Type, typename Result>
class meta_conv_op_base
{
private:
	typedef mirror::_type::_<Type> base_meta_type;
	typedef mirror::_class::_<Type> base_meta_class;
	typedef mirror::_type::_<Result> base_res_meta_type;

	static std::string operator_kw(void)
	{
		return
			std::string("operator ") +
			std::string(
				base_meta_type::name(),
				base_meta_type::name_length()
			);
	}
public:
	// type scope of the member function
	typedef base_meta_type scope;

	static inline const char* name(void)
	{
		return operator_kw().c_str();
	}

	static inline size_t name_length(void)
	{
		return operator_kw().size();
	}
};

template <typename Type, class Range>
struct conv_op_helper;

// helper template used in the implementation of the conversions meta-function
template <typename Type, typename ... Results>
struct conv_op_helper<Type, mp::range<Results...> >
{
	typedef mp::range<
		mirror::meta_conversion_operator<
			Type,
			Results
		> ...
	> type;
};


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

