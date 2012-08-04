/**
 * @file mirror/meta_function.hpp
 * @brief Registering/Reflection of free functions
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_FUNCTION_1112121420_HPP
#define MIRROR_META_FUNCTION_1112121420_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/meta_namespace.hpp>
#include <mirror/auxiliary/default_spec_tags.hpp>
#include <mirror/auxiliary/free_functions.hpp>
#include <mirror/meta_prog/only_if.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

// implementation of the meta_free_function template
template <typename FunctionSelector, int Index>
class meta_free_function
 : public aux::scoped_named_impl<
	aux::meta_free_func_base<FunctionSelector, Index>
>
{
public:
	typedef typename FunctionSelector::ct_name static_name;

	// the scope of the function
	typedef typename mirror::aux::full_meta_object<
		typename FunctionSelector::scope
	>::type scope;

private:
	typedef std::integral_constant<int, Index> _idx;
	typedef decltype(FunctionSelector::res_type(_idx())) _res_type_idty;
public:
	// the result type of the function
	typedef typename aux::reflect_type<
		typename _res_type_idty::type
	>::type result_type;

	typedef typename aux::storage_class_tag_ff<
		decltype(FunctionSelector::ovld_linkage(_idx()))
	>::type linkage;
	typedef linkage storage_class;
};

template <typename FunctionSelector>
struct meta_ovlded_free_funcs
 : aux::scoped_named_impl< FunctionSelector >
{
	typedef typename FunctionSelector::ct_name static_name;

	typedef typename mirror::aux::full_meta_object<
		typename FunctionSelector::scope
	>::type scope;

	// NOTE: internal implementation detail do not use
	template <typename X>
	struct _by_name_typ
	{
		typedef typename FunctionSelector::template by_name_typ<X> type;
	};
};

// The implementation of free_functions for instantiation of meta_namespace
template <class X>
struct free_functions<meta_namespace<X> >
{
	typedef typename mp::only_if<
		typename members<meta_namespace<X> >::type,
		mp::is_a<mp::arg<1>, meta_overloaded_functions_tag>
	>::type type;
};

// The implementation of overloads for instantiations of meta_ovlded_free_funcs
template <class FunctionSelector>
struct overloads<meta_ovlded_free_funcs<FunctionSelector> >
{
	// A range of MetaFreeFunctions reflecting the individual overloaded functions
	typedef typename mp::apply_on_seq_pack<
		aux::free_func_helper<FunctionSelector>,
		typename FunctionSelector::ovld_count
	>::type type;
};

// Implementation of meta_free_func_param template
template <typename FunctionSelector, int OvldIndex, int ParamIndex>
struct meta_free_func_param
 : public aux::scoped_named_impl<
	aux::meta_ffn_param_base<
		FunctionSelector,
		OvldIndex,
		ParamIndex
	>
>
{
private:
	typedef decltype(
		FunctionSelector::ovld_params(
			std::integral_constant<int, OvldIndex>()
		).type(
			std::integral_constant<int, ParamIndex>()
		)
	) type_helper;
public:
	typedef decltype(
		FunctionSelector::ovld_params(
			std::integral_constant<int, OvldIndex>()
		).ct_name(
			std::integral_constant<int, ParamIndex>()
		)
	) static_name;

	typedef spec_auto_tag storage_class;

	typedef meta_free_function<FunctionSelector, OvldIndex> scope;

	typedef typename aux::reflect_type<
		typename type_helper::type
	>::type type;

	// the type of the constructor parameter
	typedef std::integral_constant<int, ParamIndex> position;
};

// Implementation of the parameters meta-function template class
template <typename FunctionSelector, int OvldIndex>
struct parameters<meta_free_function<FunctionSelector, OvldIndex> >
{
private:
	typedef decltype(
		FunctionSelector::ovld_params(
			std::integral_constant<int, OvldIndex>()
		)
	) size_helper;
public:
	// A range of MetaVariables reflecting
	// the parameters of a meta-function
	typedef typename mp::apply_on_seq_pack<
		aux::ffn_param_helper<FunctionSelector, OvldIndex>,
		typename size_helper::size
	>::type type;
};

// Implementation of the members meta-function for free-functions
template <typename FunctionSelector, int OvldIndex>
struct members<meta_free_function<FunctionSelector, OvldIndex> >
{
	typedef typename parameters<
		meta_free_function<FunctionSelector, OvldIndex>
	>::type type;
};

// common helper macro used in the other macros for registering
// global-scope or nested free functions
#define MIRROR_REG_FREE_OVLD_FUNC_COMMON_HLPR_CTS( \
	NAME, \
	SPELLED \
) \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
	template <typename X> \
	struct by_name_typ \
	{ \
		typedef X NAME; \
	}; \
	typedef std::integral_constant<int, 0>


#define MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN_CTS( \
		NAME, \
		SPELLED \
) \
namespace _function { \
template <typename Namespace> struct _ ## NAME; \
template <> struct _ ## NAME< mirror::_namespace::_ >\
{  \
	typedef mirror::_namespace::_ scope; \
	MIRROR_REG_FREE_OVLD_FUNC_COMMON_HLPR_CTS( \
		NAME, \
		SPELLED \
	)


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Ends the registering of a set of overloaded functions from the global scope
/**
 *  @param NAME the name of the overloaded function
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_END(NAME)
#else
#define MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_END(NAME) \
	ovld_count; \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace::_, \
	mirror::meta_ovlded_free_funcs< \
	mirror::_function::_ ## NAME< \
	mirror::_namespace::_ \
	> > \
)
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Registers a set of overloaded functions from the global scope with Mirror
/**
 *  @param NAME the name of the overloaded functions
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN(NAME)
#else
#define MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN(NAME) \
	MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN_CTS(NAME, ())
#endif



#define MIRROR_REG_FREE_OVLD_FUNC_BEGIN_CTS( \
	NAMESPACE, \
	NAME, \
	SPELLED \
) \
namespace _function { \
template <typename Namespace> struct _ ## NAME; \
template <> struct _ ## NAME< mirror::_namespace:: NAMESPACE ::_ >\
{  \
	typedef mirror::_namespace:: NAMESPACE ::_ scope; \
	MIRROR_REG_FREE_OVLD_FUNC_COMMON_HLPR_CTS( \
		NAME, \
		SPELLED \
	) \


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Ends the registering of a set of nested overloaded functions
/**
 *  @param NAMESPACE full name of the namespace where the functions are defined
 *  @param NAME the name of the overloaded function
 *
 *  @see MIRROR_REG_FREE_OVLD_FUNC_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_FREE_OVLD_FUNC_END(NAMESPACE, NAME)
#else
#define MIRROR_REG_FREE_OVLD_FUNC_END(NAMESPACE, NAME) \
	ovld_count; \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace:: NAMESPACE ::_, \
	mirror::meta_ovlded_free_funcs< \
	mirror::_function::_ ## NAME< \
	mirror::_namespace:: NAMESPACE ::_ \
	> > \
)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Starts the definition of one of function overloads
/**
 *  @param LINKAGE the linkage of the function
 *  @param RESULT_TYPE the result type of the function
 *  @param ID a unique id of the overload
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN
 *  @see MIRROR_REG_FREE_OVLD_FUNC_BEGIN
 *  @see MIRROR_REG_FREE_FUNCTION_END
 *
 *  @ingroup registering_macros
 */
#define MIRROR_REG_FREE_FUNCTION_BEGIN(LINKAGE, RESULT_TYPE, ID)
#else
#define MIRROR_REG_FREE_FUNCTION_BEGIN(LINKAGE, RESULT_TYPE, ID) \
	ovld_index_ ## ID; \
	static mirror::mp::identity<RESULT_TYPE> res_type(ovld_index_ ## ID); \
	static spec_ ## LINKAGE ## _tag ovld_linkage(ovld_index_ ## ID); \
	struct ovld_params_ ## ID \
	{ \
		typedef std::integral_constant<int, 0>
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Ends the definition of one of function overloads
/**
 *  @param ID a unique id of the overload
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN
 *  @see MIRROR_REG_FREE_OVLD_FUNC_BEGIN
 *  @see MIRROR_REG_FREE_FUNCTION_BEGIN
 *
 *  @ingroup registering_macros
 */
#define MIRROR_REG_FREE_FUNCTION_END(ID)
#else
#define MIRROR_REG_FREE_FUNCTION_END(ID) \
		size; \
	}; \
	static ovld_params_ ## ID ovld_params(ovld_index_ ## ID); \
	typedef std::integral_constant<int, ovld_index_ ## ID ::value + 1>
#endif


// registers the a function parameter
#define MIRROR_REG_FREE_FUNCTION_PARAM_CTS(TYPE, NAME, SPELLED) \
		param_idx_ ## NAME; \
		template <typename _That> \
		struct atr_typ_hlp_ ## NAME \
		{ \
			typedef decltype(attr_type((TYPE*)nullptr, &_That::NAME))\
				type; \
		}; \
		static mirror::mp::identity<TYPE> \
		type(param_idx_ ## NAME); \
		static const char* name( param_idx_ ## NAME) \
		{ \
			return #NAME; \
		} \
		static size_t name_length( param_idx_ ## NAME) \
		{ \
			return sizeof(#NAME) - 1; \
		} \
		static typename mirror::cts::string< \
			MIRROR_PP_EXPAND_ARGS SPELLED \
		>::type ct_name(param_idx_ ## NAME); \
		typedef std::integral_constant<int, param_idx_ ## NAME ::value + 1>


#ifdef MIRROR_DOCUMENTATION_ONLY
/**
 *  This macro must be used between the MIRROR_REG_FREE_FUNCTION_BEGIN
 *  and the MIRROR_REG_FREE_FUNCTION_END macros. The parameters
 *  must be registered in the same order as they appear in the function.
 *
 *  @param TYPE the type of the parameter.
 *  @param NAME the name of the parameter.
 *
 */
#define MIRROR_REG_FREE_FUNCTION_PARAM(TYPE, NAME)
#else
#define MIRROR_REG_FREE_FUNCTION_PARAM(TYPE, NAME) \
	MIRROR_REG_FREE_FUNCTION_PARAM_CTS(TYPE, NAME, ())
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects a set of functions named @a NAME defined in the @a NAMESPACE namespace
/**
 *  This macro expands into a type conforming to the MetaOverloadedFunctions
 *  concept, which provides meta-data about the given set of overloaded functions.
 *
 *  @see MIRRORED_GLOBAL_SCOPE_FUNCTIONS
 *  @see MIRROR_REG_FREE_OVLD_FUNC_BEGIN
 *  @see MIRROR_REG_FREE_OVLD_FUNC_END
 *  @see mirror::MetaOverloadedFunctions
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_FREE_FUNCTIONS(NAMESPACE, NAME) mirror::MetaOverloadedFunctions
#else
#define MIRRORED_FREE_FUNCTIONS(NAMESPACE, NAME) \
	mirror::meta_ovlded_free_funcs< \
	mirror::_function::_ ## NAME< \
	mirror::_namespace::NAMESPACE::_ \
	> >
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects a set of functions named @a NAME defined in the global scope
/**
 *  This macro expands into a type conforming to the MetaOverloadedFunctions
 *  concept, which provides meta-data about the given set of overloaded functions.
 *
 *  @see MIRRORED_FREE_FUNCTIONS
 *  @see MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_BEGIN
 *  @see MIRROR_REG_GLOBAL_SCOPE_OVLD_FUNC_END
 *  @see mirror::MetaOverloadedFunctions
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_GLOBAL_SCOPE_FUNCTIONS(NAME) mirror::MetaOverloadedFunctions
#else
#define MIRRORED_GLOBAL_SCOPE_FUNCTIONS(NAME) \
	mirror::meta_ovlded_free_funcs< \
	mirror::_function::_ ## NAME< \
	mirror::_namespace::_ \
	> >
#endif

MIRROR_NAMESPACE_END

#endif //include guard

