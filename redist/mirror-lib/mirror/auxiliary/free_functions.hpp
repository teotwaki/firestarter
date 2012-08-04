/**
 * @file mirror/auxiliary/free_functions.hpp
 * @brief Internal implementation of free function reflection
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_FREE_FUNCTIONS_1112121709_HPP
#define MIRROR_AUX_FREE_FUNCTIONS_1112121709_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>
#include <string>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <typename FunctionSelector>
struct meta_ffn_base_base
{
public:
	static inline const char* name(void)
	{
		return FunctionSelector::name();
	}

	static inline size_t name_length(void)
	{
		return FunctionSelector::name_length();
	}

	typedef typename FunctionSelector::scope scope;
};

template <typename FunctionSelector, int Index>
class meta_free_func_base
 : public meta_ffn_base_base<FunctionSelector>
{
private:
	typedef decltype(FunctionSelector::ovld_params(
		std::integral_constant<int, Index>()
	)) ovld_params;
public:
};

template <class FunctionSelector>
struct free_func_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_free_function<
				FunctionSelector,
				Indices
			>...
		> type;
	};
};

template <typename FunctionSelector, int OvldIndex, int ParamIndex>
struct meta_ffn_param_base
{
private:
public:
	typedef meta_free_func_base<FunctionSelector, OvldIndex> scope;

	static const char* name(void)
	{
		typedef std::integral_constant<int, OvldIndex>
			ovld_index;
		typedef std::integral_constant<int, ParamIndex>
			param_index;
		typedef decltype(FunctionSelector::ovld_params(
			ovld_index()
		)) ovld_params;
		return ovld_params::name(param_index());
	}

	static size_t name_length(void)
	{
		typedef std::integral_constant<int, OvldIndex>
			ovld_index;
		typedef std::integral_constant<int, ParamIndex>
			param_index;
		typedef decltype(FunctionSelector::ovld_params(
			ovld_index()
		)) ovld_params;
		return ovld_params::name_length(param_index());
	}
};

template <class FunctionSelector, int OvldIndex>
struct ffn_param_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_free_func_param<
				FunctionSelector,
				OvldIndex,
				Indices
			>...
		> type;
	};
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

