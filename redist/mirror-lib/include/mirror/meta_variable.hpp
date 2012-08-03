/**
 * @file mirror/meta_variable.hpp
 * @brief Registering/Reflection of free variables
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_VARIABLE_1103251718_HPP
#define MIRROR_META_VARIABLE_1103251718_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/meta_namespace.hpp>
#include <mirror/auxiliary/default_spec_tags.hpp>
#include <mirror/meta_prog/only_if.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

template <typename VariableSelector>
struct meta_free_variable
 : aux::scoped_named_impl< VariableSelector >
{
	typedef typename VariableSelector::ct_name static_name;

	typedef typename mirror::aux::storage_class_tag_fv<
		typename VariableSelector::storage_class
	>::type storage_class;

	typedef typename mirror::aux::full_meta_object<
		typename VariableSelector::scope
	>::type scope;

	typedef typename mirror::reflected<
		typename VariableSelector::type
	>::type type;

	static const typename VariableSelector::type& get(void)
	{
		return VariableSelector::get();
	}

	static void set(const typename VariableSelector::type& val)
	{
		VariableSelector::set(val);
	}
private:
	typedef decltype(VariableSelector::has_address()) has_address;

	friend struct meta_object_category<meta_free_variable>;
public:

	static auto address(void) -> decltype(VariableSelector::address())
	{
		return VariableSelector::address();
	}

	// NOTE: internal implementation detail do not use
	template <typename X>
	struct _by_name_typ
	{
		typedef typename VariableSelector::template by_name_typ<X> type;
	};

	// NOTE: internal implementation detail do not use
	template <typename X>
	struct _by_name_val
	{
		typedef typename VariableSelector::template by_name_val<X> type;
	};
};

//  Specialization of meta_object_category for meta_member_variables(s)
template <typename Selector>
struct meta_object_category<meta_free_variable<Selector> >
{
private:
	typedef meta_free_variable<Selector> mfv;

	static meta_free_variable_tag _get_type(std::false_type);

	static meta_plain_free_variable_tag _get_type(std::true_type);
public:
	typedef decltype(_get_type(typename mfv::has_address())) type;
};

// The implementation of free_variables for instantiation of meta_namespace
template <class X>
struct free_variables<meta_namespace<X> >
{
	typedef typename mp::only_if<
		typename members<meta_namespace<X> >::type,
		mp::is_a<mp::arg<1>, meta_free_variable_tag>
	>::type type;
};

#define MIRROR_REG_FREE_VARIABLE_COMMON_HLPR_CTS( \
	STORAGE_CLASS, \
	VARIABLE, \
	SPELLED \
) \
	typedef mirror::spec_ ## STORAGE_CLASS ## _tag storage_class; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#VARIABLE) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
	template <typename X> \
	struct by_name_typ \
	{ \
		typedef X VARIABLE; \
	}; \
	template <typename X> \
	struct by_name_val \
	{ \
		X VARIABLE; \
		by_name_val(void) = default; \
		template <class Parent, typename Param> \
		by_name_val(Parent& parent, Param param) \
		 : VARIABLE(parent, param) \
		{ } \
	};

#define MIRROR_REG_GLOBAL_SCOPE_VARIABLE_CTS( \
		STORAGE_CLASS, \
		VARIABLE, \
		SPELLED \
) \
namespace _variable { \
template <typename Namespace> struct _ ## VARIABLE ; \
template <> struct _ ## VARIABLE< mirror::_namespace::_ >\
{  \
	MIRROR_REG_FREE_VARIABLE_COMMON_HLPR_CTS( \
		STORAGE_CLASS, \
		VARIABLE, \
		SPELLED \
	) \
	typedef decltype(VARIABLE) type; \
	typedef mirror::_namespace::_ scope; \
	static inline const type& get(void) { return :: VARIABLE; } \
	static inline void set(type val) { :: VARIABLE = val; } \
	static inline type* address(void) { return & :: VARIABLE; } \
	static std::true_type has_address(void); \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace::_, \
	mirror::meta_free_variable< \
	mirror::_variable::_ ## VARIABLE< \
	mirror::_namespace::_ \
	> > \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a variable from the global scope with Mirror
/**
 *  @param STORAGE_CLASS the storage class of the variable. _ is the default.
 *  @param VARIABLE the variable name
 *
 *  @see MIRROR_REG_FREE_VARIABLE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_VARIABLE(STORAGE_CLASS, VARIABLE)
#else
#define MIRROR_REG_GLOBAL_SCOPE_VARIABLE(STORAGE_CLASS, VARIABLE) \
	MIRROR_REG_GLOBAL_SCOPE_VARIABLE_CTS(STORAGE_CLASS, VARIABLE, ())
#endif

#define MIRROR_REG_FREE_VARIABLE_CTS( \
	STORAGE_CLASS, \
	NAMESPACE, \
	VARIABLE, \
	SPELLED \
) \
namespace _variable { \
template <typename Namespace> struct _ ## VARIABLE ; \
template <> struct _ ## VARIABLE< mirror::_namespace:: NAMESPACE ::_ >\
{  \
	MIRROR_REG_FREE_VARIABLE_COMMON_HLPR_CTS( \
		STORAGE_CLASS, \
		VARIABLE, \
		SPELLED \
	) \
	typedef decltype(NAMESPACE :: VARIABLE) type; \
	typedef mirror::_namespace:: NAMESPACE ::_ scope; \
	static inline const type& get(void) { return :: NAMESPACE :: VARIABLE; } \
	static inline void set(type val) { :: NAMESPACE :: VARIABLE = val; } \
	static inline type* address(void) { return & :: NAMESPACE :: VARIABLE;}\
	static std::true_type has_address(void); \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace:: NAMESPACE ::_, \
	mirror::meta_free_variable< \
	mirror::_variable::_ ## VARIABLE< \
	mirror::_namespace:: NAMESPACE ::_ \
	> > \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a variable from the a namespace with Mirror
/**
 *  @param STORAGE_CLASS the storage class of the variable. _ is the default.
 *  @param NAMESPACE full name of the namespace where the variable is defined
 *  @param VARIABLE the variable name
 *
 *  @see MIRROR_REG_FREE_VARIABLE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_FREE_VARIABLE(STORAGE_CLASS, NAMESPACE, VARIABLE)
#else
#define MIRROR_REG_FREE_VARIABLE(STORAGE_CLASS, NAMESPACE, VARIABLE) \
	MIRROR_REG_FREE_VARIABLE_CTS(STORAGE_CLASS, NAMESPACE, VARIABLE, ())
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the variable @a VARIABLE defined in the @a NAMESPACE namespace
/**
 *  This macro expands into a type conforming to the MetaFreeVariable
 *  concept, which provides meta-data about the given variable.
 *
 *  @see MIRRORED_GLOBAL_SCOPE_VARIABLE
 *  @see MIRROR_REG_FREE_VARIABLE
 *  @see mirror::MetaFreeVariable
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_FREE_VARIABLE(NAMESPACE, VARIABLE) mirror::MetaFreeVariable
#else
#define MIRRORED_FREE_VARIABLE(NAMESPACE, VARIABLE) \
	mirror::meta_free_variable< \
	mirror::_variable::_ ## VARIABLE< \
	mirror::_namespace::NAMESPACE::_ \
	> >
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the variable @a VARIABLE defined in the global scope
/**
 *  This macro expands into a type conforming to the MetaFreeVariable
 *  concept, which provides meta-data about the given variable.
 *
 *  @see MIRRORED_FREE_VARIABLE
 *  @see MIRROR_REG_FREE_VARIABLE
 *  @see mirror::MetaFreeVariable
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_GLOBAL_SCOPE_VARIABLE(VARIABLE) mirror::MetaFreeVariable
#else
#define MIRRORED_GLOBAL_SCOPE_VARIABLE(VARIABLE) \
	mirror::meta_free_variable< \
	mirror::_variable::_ ## VARIABLE< \
	mirror::_namespace::_ \
	> >
#endif

namespace aux {

template <typename StorageClassTag>
struct meta_simple_var
{
	typedef mirror::meta_variable_tag category;
	typedef typename mirror::aux::storage_class_tag_fv<
		StorageClassTag
	>::type storage_class;
	typedef MIRRORED_UNSPECIFIED_SCOPE() scope;
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the meta_object passed as the @a META_OBJECT argument
/**
 *  This macro expands into a type conforming to the MetaMetaObject
 *  concept, which provides meta-data about the given Mirror's MetaObject.
 *  The type name passed as the @a META_TYPE argument
 *  must by a valid Mirror's meta-object.
 *
 *  @see mirror::Meta Variable
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_SIMPLE_VARIABLE(STORAGE_CLASS, VARIABLE) mirror::MetaVariable
#else
#define MIRRORED_SIMPLE_VARIABLE(STORAGE_CLASS, VARIABLE) \
struct _mirror_meta_simple_var_ ## VARIABLE \
 : mirror::aux::meta_simple_var< \
	mirror::spec_ ## STORAGE_CLASS ## _tag \
> \
{ \
	typedef mirror::reflected<decltype(VARIABLE)>::type type; \
	static std::string base_name(void) \
	{ \
		return std::string(#VARIABLE, sizeof(#VARIABLE) - 1); \
	} \
	static inline std::string full_name(void) { return base_name(); } \
	static inline std::string local_name(void) { return base_name(); } \
}
#endif


MIRROR_NAMESPACE_END

#endif //include guard

