/**
 * @file mirror/meta_class.hpp
 * @brief Implementation of class registering and reflection
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_CLASS_1011291729_HPP
#define MIRROR_META_CLASS_1011291729_HPP

#include <mirror/meta_namespace.hpp>
#include <mirror/meta_type.hpp>
#include <mirror/meta_typedef.hpp>
#include <mirror/specifier_tags.hpp>
#include <mirror/constructor_tags.hpp>
#include <mirror/auxiliary/default_spec_tags.hpp>
#include <mirror/auxiliary/class_layout.hpp>
#include <mirror/auxiliary/base_classes.hpp>
#include <mirror/auxiliary/member_variables.hpp>
#include <mirror/auxiliary/constructors.hpp>
#include <mirror/auxiliary/member_functions.hpp>
#include <mirror/meta_prog/identity.hpp>
#include <mirror/meta_prog/lambda.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/concat.hpp>
#include <mirror/meta_prog/unique.hpp>
#include <mirror/meta_prog/fold.hpp>
#include <mirror/meta_prog/apply_on_seq_pack.hpp>
#include <initializer_list>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace _class {

// Helper structure for storing information about class inheritance
template <
	typename InheritanceType,
	typename AccessSpecifier,
	typename BaseClass
>
struct inheritance
{
	typedef InheritanceType inherit;
	typedef AccessSpecifier access;
	typedef BaseClass base;
};

// helper structure for storing information about member variable
template <
	typename AccessType,
	typename StorageClass,
	typename TypeSelector
>
struct member
{
	typedef AccessType access_type;
	typedef StorageClass storage_class;
	typedef TypeSelector type_sel;
	typedef TypeSelector real_type;
};

// specialization of member for members with typedefined typs
template <
	typename AccessType,
	typename StorageClass,
	typename BaseTypedef
>
struct member<
	AccessType,
	StorageClass,
	mirror::_typedef::_selector<BaseTypedef>
>
{
	typedef AccessType access_type;
	typedef StorageClass storage_class;
	typedef mirror::_typedef::_selector<BaseTypedef>
		type_sel;
	typedef typename BaseTypedef::source_type real_type;
};

struct _defs_base
{
	typedef mirror::mp::range<> base_classes;
	typedef std::integral_constant<int, 0> attr_count;

	typedef std::integral_constant<int, 0> ctr_count;

	typedef std::integral_constant<int, 0> mem_ovldfn_count;

	template <int Idx>
	static mirror::custom_constructor_tag
	ctr_kind(std::integral_constant<int, Idx>);

	typedef mp::range<> conv_ops;
};

template <typename Class, typename TypeKind>
struct _defaults
 : _defs_base
 , mirror::_type::_<Class>
{
	typedef Class _this;
	typedef TypeKind kind;
	typedef meta_class_tag category;
	//
	typedef typename ::std::has_trivial_default_constructor<
		Class
	>::type	has_default_ctr;

	typedef typename ::std::has_trivial_copy_constructor<
		Class
	>::type	has_copy_ctr;
};

template <typename Class, typename TypeKind>
struct _defaults_native : _defs_base
{
	typedef Class _this;
	typedef TypeKind kind;

	typedef std::true_type has_default_ctr;
	typedef std::true_type has_copy_ctr;

	// default constructor
	struct ctr_params_default
	{
		typedef std::integral_constant<int, 0> size;
	};
	static ctr_params_default ctr_params(std::integral_constant<int, 0>);
	static spec_public_tag ctr_access(std::integral_constant<int, 0>);

	// copy-constructor / initializer
	struct ctr_params_copy
	{
		typedef std::integral_constant<int, 0> param_idx_source;
		static mp::identity<_this> type(param_idx_source);
		static const char* name( param_idx_source )
		{
			return "source";
		}
		static size_t name_length( param_idx_source )
		{
			return sizeof("source") - 1;
		}
		static typename mirror::cts::string<
			's', 'o', 'u', 'r', 'c', 'e'
		>::type ct_name( param_idx_source );
		typedef std::integral_constant<int, 1> size;
	};
	static ctr_params_copy ctr_params(std::integral_constant<int, 1>);
	static spec_public_tag ctr_access(std::integral_constant<int, 1>);

	typedef std::integral_constant<int, 2> ctr_count;
};

template <typename Class>
struct _defaults<Class, mirror::spec_enum_tag>
 : public _defaults_native<Class, mirror::spec_enum_tag>
{
	// TODO: conversion of enum values to ints
	typedef mp::range<> conv_ops;
};

// Default implementation of the _ template providing base info
template <typename Class>
struct _ : _defaults<Class, typename mirror::_type::_<Class>::kind>
{ };

} // namespace _class

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Begins the registering of an elaborate type from the global scope
/**
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *  @param CLASS the base class name.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_CLASS_BEGIN(ELABORATED_TYPE, CLASS)
#else
#define MIRROR_REG_GLOBAL_SCOPE_CLASS_BEGIN(ELABORATED_TYPE, CLASS) \
MIRROR_REG_GLOBAL_SCOPE_TYPE_BASICS(ELABORATED_TYPE, CLASS, ()) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace :: _, \
	mirror::meta_class< CLASS > \
) \
namespace _class { \
template <> \
struct _< CLASS > \
 : _defaults< CLASS, spec_ ## ELABORATED_TYPE ## _tag >\
{
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that begins the registering of an elaborate type
/**
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *  @param NAMESPACE the full namespace name inside of which the class is
 *  nested.
 *  @param CLASS the base class name.
 *
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_BEGIN(ELABORATED_TYPE, NAMESPACE, CLASS)
#else
#define MIRROR_REG_CLASS_BEGIN(ELABORATED_TYPE, NAMESPACE, CLASS) \
MIRROR_REG_TYPE_BASICS(class, NAMESPACE, CLASS, ()) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace :: NAMESPACE :: _, \
	mirror::meta_class< :: NAMESPACE :: CLASS> \
) \
namespace _class { \
template <> \
struct _< :: NAMESPACE :: CLASS > \
 : _defaults< :: NAMESPACE :: CLASS, spec_ ## ELABORATED_TYPE ## _tag >\
{
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that begins the registering of an elaborate type nested in a class
/**
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *  @param PARENT_CLASS the full class name inside of which the class is
 *  nested.
 *  @param CLASS the base class name.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_NESTED_CLASS_BEGIN(ELABORATED_TYPE, PARENT_CLASS, CLASS)
#else
#define MIRROR_REG_NESTED_CLASS_BEGIN(ELABORATED_TYPE, PARENT_CLASS, CLASS) \
MIRROR_REG_NESTED_TYPE_BASICS(class, PARENT_CLASS, CLASS, ()) \
namespace _class { \
template <> \
struct _< :: PARENT_CLASS :: CLASS > \
 : _defaults< :: PARENT_CLASS :: CLASS, spec_ ## ELABORATED_TYPE ## _tag >\
{ \
	typedef spec_ ## ELABORATED_TYPE ## _tag kind;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of an elaborate type
/**
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_END
#else
#define MIRROR_REG_CLASS_END \
}; /* struct _class::_< :: NAMESPACE :: CLASS > */ \
} /* namespace _class */
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that creates an alias for a type
/** This macro can be used to create an alias for a type inside
 *  of the class registering expression, basically to do a @c typedef.
 *
 *  Doing this is necessary with templated types whose name contains
 *  a comma. TODO
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_TYPE_ALIAS(TYPENAME, ALIAS)
#else
#define MIRROR_REG_TYPE_ALIAS(TYPENAME, ALIAS) \
	typedef MIRROR_PP_EXPAND_ARGS TYPENAME ALIAS;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that begins the registering of the base classes of a class
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_BASE_CLASSES_BEGIN
#else
#define MIRROR_REG_BASE_CLASSES_BEGIN \
	typedef mirror::mp::range<
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of the base classes of a class
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros and after the
 *  MIRROR_REG_BASE_CLASSES_BEGIN macro.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_BASE_CLASSES_END
#else
#define MIRROR_REG_BASE_CLASSES_END \
	> base_classes;
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that registers one base class inheritance
/**
 *  This macro must be used between the MIRROR_REG_BASE_CLASSES_BEGIN
 *  and the MIRROR_REG_BASE_CLASSES_END macros and must be comma
 *  separated from the other invocations of this macro between the
 *  MIRROR_REG_BASE_CLASSES_BEGIN and MIRROR_REG_BASE_CLASSES_END
 *  macros.
 *
 *  @param INHERITANCE_TYPE the inheritance type specifier. This argument must
 *  have one of these values (virtual, non_virtual or _ for default
 *  (non-virtual)).
 *
 *  @param ACCESS_TYPE the member access type specifier. This argument must
 *  have one of these values (private, protected, public or _ for default
 *  (public for struct(s) and private for class(es))).
 *
 *  @param BASE_CLASS the full base class type name.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_BASE_CLASSES_BEGIN
 *  @see MIRROR_REG_BASE_CLASSES_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_BASE_CLASS(INHERITANCE_TYPE, ACCESS_TYPE, BASE_CLASS)
#else
#define MIRROR_REG_BASE_CLASS(INHERITANCE_TYPE, ACCESS_TYPE, BASE_CLASS)\
	mirror::_class::inheritance< \
		spec_ ## INHERITANCE_TYPE ## _tag, \
		spec_ ## ACCESS_TYPE ## _tag, \
		BASE_CLASS \
	>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that begins the registering of the class' member variables
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_CLASS_ATTR_END
 *  @see MIRROR_REG_CLASS_ATTR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_MEM_VARS_BEGIN
#else
#define MIRROR_REG_CLASS_MEM_VARS_BEGIN \
	template <typename T, typename P> static T attr_type(T*, P); \
	template <typename T> static T attr_type(_*, T _this::*); \
	template <typename T> static T attr_type(_*, T*); \
	template <typename T> static T real_attr_type(T*); \
	template <typename BTD> static typename BTD::source_type \
	real_attr_type(mirror::_typedef::_selector<BTD>*); \
	static spec___tag member_access(std::integral_constant<int, -1>); \
	typedef std::integral_constant<int, 0>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of the class' member variables
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_CLASS_MEM_VARS_BEGIN
 *  @see MIRROR_REG_CLASS_MEM_VAR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_MEM_VARS_END
#else
#define MIRROR_REG_CLASS_MEM_VARS_END \
	attr_count;
#endif

#define MIRROR_REG_CLASS_MEM_VAR_BEGIN_CTS( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, \
	SPELLED, \
	MEM_PTR  \
) \
	attr_idx_ ## NAME; \
	typedef decltype(attr_type((TYPE*)nullptr, MEM_PTR)) \
		attr_type_ ## NAME; \
	typedef decltype(real_attr_type((attr_type_ ## NAME*)nullptr)) \
		real_attr_type_ ## NAME; \
	static typename mirror::aux::resof_member_access< \
		_, \
		spec_ ## ACCESS_TYPE ## _tag, \
		attr_idx_ ## NAME ::value \
	>::type member_access(attr_idx_ ## NAME); \
	typedef  member< \
		decltype(member_access(attr_idx_ ## NAME())), \
		spec_ ## STORAGE_CLASS ## _tag, \
		attr_type_ ## NAME \
	> mem_var_ ## NAME; \
	static mem_var_ ## NAME attr_traits(attr_idx_ ## NAME); \
	static const char* attr_name( attr_idx_ ## NAME ) \
	{ \
		return #NAME; \
	} \
	static size_t attr_name_length( attr_idx_ ## NAME ) \
	{ \
		return sizeof(#NAME) - 1; \
	} \
	static typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type attr_ct_name( attr_idx_ ## NAME ); \
	template <typename X> \
	struct by_name_typ_ ## NAME \
	{ \
		typedef X NAME; \
	}; \
	template <typename X> \
	static by_name_typ_ ## NAME <X> by_name_typ(attr_idx_ ## NAME, X*); \
	template <typename X> \
	struct by_name_val_ ## NAME \
	{ \
		X NAME; \
		by_name_val_ ## NAME(void) = default; \
		template <class Parent, typename Param> \
		by_name_val_ ## NAME(Parent& parent, Param param) \
		 : NAME(parent, param) \
		{ } \
	}; \
	template <typename X> \
	static by_name_val_ ## NAME <X> by_name_val(attr_idx_ ## NAME, X*); \
	typedef typename mem_var_ ## NAME :: real_type real_type_ ## NAME; \
	typedef const real_type_ ## NAME & r_value_ ## NAME;

#define MIRROR_REG_CLASS_MEM_VAR_END( \
	NAME \
) typedef std::integral_constant<int, attr_idx_ ## NAME::value + 1>


#define MIRROR_REG_CLASS_MEM_VAR_GET_SET_HLPR( \
	NAME, \
	GET_EXPRESSION, \
	SET_EXPRESSION  \
) \
	static auto get( \
		attr_idx_ ## NAME, \
		const _this& _ \
	) -> decltype(GET_EXPRESSION) { return GET_EXPRESSION ;} \
	static void set( \
		attr_idx_ ## NAME, \
		_this& _, \
		r_value_ ## NAME _ ## NAME \
	) { SET_EXPRESSION ; }

#define MIRROR_REG_CLASS_MEM_VAR_GET_SET_CTS( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, \
	SPELLED, \
	GET_EXPRESSION, \
	SET_EXPRESSION  \
) \
	MIRROR_REG_CLASS_MEM_VAR_BEGIN_CTS( \
		ACCESS_TYPE, \
		STORAGE_CLASS, \
		TYPE, \
		NAME, \
		SPELLED, \
		(TYPE*)nullptr \
	) \
	MIRROR_REG_CLASS_MEM_VAR_GET_SET_HLPR( \
		NAME, \
		GET_EXPRESSION, \
		SET_EXPRESSION \
	) \
	static std::false_type has_address(attr_idx_ ## NAME); \
	static void* address( attr_idx_ ## NAME, const _this& _ ); \
	static void* mem_ptr(attr_idx_ ## NAME); \
	MIRROR_REG_CLASS_MEM_VAR_END(NAME)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for registering a single class member variable
/** This registering macro registers a single member variable of
 *  a class and allows to specify custom expressions for getting
 *  and setting the value of the member variable in case it is necessary.
 *  This macro must be used between the MIRROR_REG_CLASS_MEM_VARS_BEGIN
 *  and the MIRROR_REG_CLASS_MEM_VARS_END macros.
 *  In case the member variable is a regular non-const and publically
 *  accessible member variable, the MIRROR_REG_CLASS_MEM_VAR macro
 *  can be used.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|publici|_)
 *  In case the special '_' specifier is used, then the access type is either
 *  determined from the elaborated type specifier of the parent class or from
 *  the explicitly specified access of a previous member (if any).
 *
 *  @param STORAGE_CLASS the storage class specifier for the member variable
 *  (static|mutable|auto|...|_) if the special '_' specifier is used,then auto
 *  is assumed.
 *
 *  @param TYPE the type of the member variable. If the special '_' specifier
 *  is used, then the type if automatically detected, if it is possible.
 *
 *  @param NAME the name (i.e. identifier not a string) of the member variable.
 *
 *  @param GET_EXPRESSION the expression which returns the value of the member
 *  variable from a const reference to an instance of the parent class of the
 *  member var. The reference to the parent class instance can be accessed by
 *  the pre-defined _ variable.
 *
 *  @param SET_EXPRESSION the expression which sets the value of the member
 *  variable to a reference to an instance of the parent class of the member
 *  variable.
 *  The reference to the parent class instance can be accessed by
 *  the pre-defined _ variable, the value to be set is stored in a variable
 *  having the same name as the registered member variable prefixed with
 *  a single underscore.
 *
 *  @see MIRROR_REG_CLASS_MEM_VARS_BEGIN
 *  @see MIRROR_REG_CLASS_MEM_VARS_END
 *  @see MIRROR_REG_CLASS_MEM_VAR
 *  @see MIRROR_FRIENDLY_CLASS
 *
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_MEM_VAR_GET_SET( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, \
	GET_EXPRESSION, \
	SET_EXPRESSION  \
)
#else
#define MIRROR_REG_CLASS_MEM_VAR_GET_SET( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, \
	GET_EXPRESSION, \
	SET_EXPRESSION  \
) MIRROR_REG_CLASS_MEM_VAR_GET_SET_CTS( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, (), \
	GET_EXPRESSION, \
	SET_EXPRESSION  \
)
#endif

#define MIRROR_REG_CLASS_MEM_VAR_CTS( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, \
	SPELLED \
) MIRROR_REG_CLASS_MEM_VAR_BEGIN_CTS( \
		ACCESS_TYPE, \
		STORAGE_CLASS, \
		TYPE, \
		NAME, \
		SPELLED, \
		&_this::NAME \
	) \
	static const real_attr_type_ ## NAME& get( \
		attr_idx_ ## NAME, \
		const _this& _ \
	){ return _.NAME; } \
	static void set( \
		attr_idx_ ## NAME, \
		_this& _, \
		r_value_ ## NAME _ ## NAME \
	) { _.NAME = _ ## NAME; } \
	static std::true_type has_address(attr_idx_ ## NAME); \
	static auto address( attr_idx_ ## NAME, _this& _ ) \
	 -> decltype(&_.NAME) { return &_.NAME; } \
	static auto address( attr_idx_ ## NAME, const _this& _ ) \
	 -> decltype(&_.NAME) { return &_.NAME; } \
	static auto mem_ptr(attr_idx_ ## NAME) \
	 -> decltype(&_this::NAME) { return &_this::NAME; } \
	MIRROR_REG_CLASS_MEM_VAR_END(NAME)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for registering a single class member variable
/** This registering macro registers a single member variable of
 *  a class, in case the member variable is a regular non-const and publically
 *  accessible member variable or its parent class definition contains the
 *  MIRROR_FRIENDLY_CLASS macro.
 *
 *  This macro must be used between the MIRROR_REG_CLASS_MEM_VARS_BEGIN
 *  and the MIRROR_REG_CLASS_MEM_VARS_END macros.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|publici|_)
 *  In case the special '_' specifier is used, then the access type is either
 *  determined from the elaborated type specifier of the parent class or from
 *  the explicitly specified access of a previous member (if any).
 *
 *  @param STORAGE_CLASS the storage class specifier for the member variable
 *  (static|mutable|auto|...|_) if the special '_' specifier is used,then auto
 *  is assumed.
 *
 *  @param TYPE the type of the member variable. If the special '_' specifier
 *  is used, then the type if automatically detected, if it is possible.
 *
 *  @param NAME the name (i.e. identifier not a string) of the member variable.
 *
 *  @see MIRROR_REG_CLASS_MEM_VARS_BEGIN
 *  @see MIRROR_REG_CLASS_MEM_VARS_END
 *  @see MIRROR_REG_CLASS_MEM_VAR_GET_SET
 *  @see MIRROR_FRIENDLY_CLASS
 *
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_MEM_VAR( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME \
)
#else
#define MIRROR_REG_CLASS_MEM_VAR( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME \
) MIRROR_REG_CLASS_MEM_VAR_CTS( \
	ACCESS_TYPE, \
	STORAGE_CLASS, \
	TYPE, \
	NAME, () \
)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that begins the registering of the class' constructors
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @see MIRROR_REG_STRUCT_INITIALIZER
 *  @see MIRROR_REG_DEFAULT_CONSTRUCTOR
 *  @see MIRROR_REG_COPY_CONSTRUCTOR
 *  @see MIRROR_REG_INITLIST_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CONSTRUCTORS_BEGIN
#else
#define MIRROR_REG_CONSTRUCTORS_BEGIN \
	typedef std::integral_constant<int, 0>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of the class' constructors
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_STRUCT_INITIALIZER
 *  @see MIRROR_REG_DEFAULT_CONSTRUCTOR
 *  @see MIRROR_REG_COPY_CONSTRUCTOR
 *  @see MIRROR_REG_INITLIST_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CONSTRUCTORS_END
#else
#define MIRROR_REG_CONSTRUCTORS_END \
	ctr_count;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that registers the class' default constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTORS_BEGIN
 *  and the MIRROR_REG_CONSTRUCTORS_END macros.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|publici|_)
 *  In case the special '_' specifier is used, then the access type is
 *  determined from the elaborated type specifier of the parent class
 *
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @see MIRROR_REG_COPY_CONSTRUCTOR
 *  @see MIRROR_REG_STRUCT_INITIALIZER
 *  @see MIRROR_REG_INITLIST_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_DEFAULT_CONSTRUCTOR(ACCESS_TYPE)
#else
#define MIRROR_REG_DEFAULT_CONSTRUCTOR(ACCESS_TYPE) \
	ctr_index_default; \
	typedef std::true_type has_default_ctr; \
	struct ctr_params_default \
	{ \
		typedef std::integral_constant<int, 0> size; \
	};\
	static ctr_params_default ctr_params(ctr_index_default); \
	static spec_ ## ACCESS_TYPE ## _tag ctr_access(ctr_index_default); \
	static default_constructor_tag ctr_kind(ctr_index_default); \
	typedef std::integral_constant<int, ctr_index_default::value + 1>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that registers the class' copy constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTORS_BEGIN
 *  and the MIRROR_REG_CONSTRUCTORS_END macros.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|publici|_)
 *  In case the special '_' specifier is used, then the access type is
 *  determined from the elaborated type specifier of the parent class
 *
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @see MIRROR_REG_DEFAULT_CONSTRUCTOR
 *  @see MIRROR_REG_STRUCT_INITIALIZER
 *  @see MIRROR_REG_INITLIST_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_COPY_CONSTRUCTOR(ACCESS_TYPE)
#else
#define MIRROR_REG_COPY_CONSTRUCTOR(ACCESS_TYPE) \
	ctr_index_copy; \
	typedef std::true_type has_copy_ctr; \
	struct ctr_params_copy \
	{ \
		typedef std::integral_constant<int, 0> param_idx_source; \
		static mp::identity<_this> type(param_idx_source); \
		static const char* name( param_idx_source ) \
		{ \
			return "source"; \
		} \
		static size_t name_length( param_idx_source ) \
		{ \
			return sizeof("source") - 1; \
		} \
		static typename mirror::cts::string< \
			's', 'o', 'u', 'r', 'c', 'e' \
		>::type ct_name( param_idx_source ); \
		typedef std::integral_constant<int, 1> size; \
	}; \
	static ctr_params_copy ctr_params(ctr_index_copy); \
	static spec_ ## ACCESS_TYPE ## _tag ctr_access(ctr_index_copy); \
	static copy_constructor_tag ctr_kind(ctr_index_copy); \
	typedef std::integral_constant<int, ctr_index_copy::value + 1>
#endif

#define MIRROR_REG_INITLIST_CONSTRUCTOR_CTS( \
	ACCESS_TYPE, \
	ELEMENT_TYPE, \
	NAME, \
	SPELLED \
) \
	ctr_index_initlist; \
	typedef std::true_type has_initlist_ctr; \
	struct ctr_params_initlist \
	{ \
		typedef std::integral_constant<int, 0> param_idx_source; \
		static mp::identity<std::initializer_list< ELEMENT_TYPE > > \
		type(param_idx_source); \
		static const char* name( param_idx_source ) \
		{ \
			return #NAME; \
		} \
		static size_t name_length( param_idx_source ) \
		{ \
			return sizeof(#NAME) - 1; \
		} \
		static typename mirror::cts::string< \
			MIRROR_PP_EXPAND_ARGS SPELLED \
		>::type ct_name( param_idx_source ); \
		typedef std::integral_constant<int, 1> size; \
	}; \
	static ctr_params_initlist ctr_params(ctr_index_initlist); \
	static spec_ ## ACCESS_TYPE ## _tag ctr_access(ctr_index_initlist); \
	static initializer_list_constructor_tag ctr_kind(ctr_index_initlist);\
	typedef std::integral_constant<int, ctr_index_initlist::value + 1>

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that registers the class' initializer_list constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTORS_BEGIN
 *  and the MIRROR_REG_CONSTRUCTORS_END macros.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|publici|_)
 *  In case the special '_' specifier is used, then the access type is
 *  determined from the elaborated type specifier of the parent class
 *
 *  @param ELEMENT_TYPE the type of the elements in the initializer list
 *
 *  @param NAME the name of the initializer-list parameter
 *
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @see MIRROR_REG_DEFAULT_CONSTRUCTOR
 *  @see MIRROR_REG_STRUCT_INITIALIZER
 *  @see MIRROR_REG_COPY_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_INITLIST_CONSTRUCTOR(ACCESS_TYPE, ELEMENT_TYPE, NAME)
#else
#define MIRROR_REG_INITLIST_CONSTRUCTOR(ACCESS_TYPE, ELEMENT_TYPE, NAME) \
	MIRROR_REG_INITLIST_CONSTRUCTOR_CTS( \
		ACCESS_TYPE, \
		ELEMENT_TYPE, \
		NAME, () \
	)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that registers the POD class' initializer as constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTORS_BEGIN
 *  and the MIRROR_REG_CONSTRUCTORS_END macros.
 *
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @see MIRROR_REG_DEFAULT_CONSTRUCTOR
 *  @see MIRROR_REG_COPY_CONSTRUCTOR
 *  @see MIRROR_REG_INITLIST_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_STRUCT_INITIALIZER()
#else
#define MIRROR_REG_STRUCT_INITIALIZER() \
	ctr_index_structinit; \
	struct ctr_params_structinit \
	{ \
		typedef attr_count size; \
		template <typename AttrTraits> \
		static auto type_hlpr(AttrTraits)-> \
		typename AttrTraits::real_type; \
		template <int Idx> \
		static auto type(std::integral_constant<int, Idx> idx) -> \
		mp::identity<decltype(type_hlpr(attr_traits(idx)))>; \
		template <int Idx> \
		static const char* name(std::integral_constant<int, Idx> idx) \
		{ \
			return attr_name(idx); \
		} \
		template <int Idx> \
		static size_t name_length(std::integral_constant<int, Idx> idx)\
		{ \
			return attr_name_length(idx); \
		} \
		template <int Idx> \
		static auto ct_name(std::integral_constant<int, Idx> idx) -> \
		decltype(attr_ct_name(idx)); \
	}; \
	static ctr_params_structinit ctr_params(ctr_index_structinit); \
	static spec_public_tag ctr_access(ctr_index_structinit); \
	static std::true_type is_strinit(ctr_index_structinit); \
	static struct_initializer_tag ctr_kind(ctr_index_structinit); \
	typedef std::integral_constant<int, ctr_index_structinit::value + 1>
#endif


#define MIRROR_REG_MEM_FUNCTION_PARAM_CTS(TYPE, NAME, SPELLED) \
	param_idx_ ## NAME; \
	template <typename _That> \
	struct atr_typ_hlp_ ## NAME \
	{ \
		typedef decltype(attr_type((TYPE*)nullptr, &_That::NAME))\
			type; \
	}; \
	template <typename _Type> \
	static mirror::mp::identity<_Type> \
	get_type(_Type*, param_idx_ ## NAME); \
	static atr_typ_hlp_ ## NAME <_this> \
	get_type(_*, param_idx_ ## NAME);\
	static auto type(param_idx_ ## NAME pi) -> \
		decltype(get_type((TYPE*)nullptr, pi)); \
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
/// Macro that ends the registering of a single class' constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTORS_BEGIN
 *  and the MIRROR_REG_CONSTRUCTORS_END macros.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|public|_)
 *  In case the special '_' specifier is used, then the access type is
 *  determined from the elaborated type specifier of the parent class
 *
 *  @param ID a unique identifier of the constructor within the class.
 *  Same rules as for C++ identifiers apply except it is possible
 *  for the identifier to start with a digit. i.e. 1 is a valid constructor
 *  identifier.
 *
 *  @see MIRROR_REG_CONSTRUCTOR_END
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @see MIRROR_REG_STRUCT_INITIALIZER
 *  @see MIRROR_REG_DEFAULT_CONSTRUCTOR
 *  @see MIRROR_REG_COPY_CONSTRUCTOR
 *  @see MIRROR_REG_INITLIST_CONSTRUCTOR
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CONSTRUCTOR_BEGIN(ACCESS_TYPE, ID)
#else
#define MIRROR_REG_CONSTRUCTOR_BEGIN(ACCESS_TYPE, ID) \
	ctr_index_ ## ID; \
	static spec_ ## ACCESS_TYPE ## _tag ctr_access(ctr_index_ ## ID); \
	struct ctr_params_ ## ID \
	{ \
		typedef std::integral_constant<int, 0>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of a single class' constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTORS_BEGIN
 *  and the MIRROR_REG_CONSTRUCTORS_END macros.
 *
 *  @param ID a unique identifier of the constructor within the class.
 *  Same rules as for C++ identifiers apply except it is possible
 *  for the identifier to start with a digit. i.e. 1 is a valid constructor
 *  identifier.
 *
 *  @see MIRROR_REG_CONSTRUCTOR_BEGIN
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CONSTRUCTOR_END(ID)
#else
#define MIRROR_REG_CONSTRUCTOR_END(ID) \
		size; \
	}; \
	static ctr_params_ ## ID ctr_params(ctr_index_ ## ID); \
	static custom_constructor_tag ctr_kind(ctr_index_ ## ID); \
	typedef std::integral_constant<int, ctr_index_ ## ID ::value + 1>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of a single class' constructor
/// Macro that registers a single parameter of a member function
/**
 *  This macro must be used between the MIRROR_REG_MEM_FUNCTION_BEGIN
 *  and the MIRROR_REG_MEM_FUNCTION_END macros. The parameters
 *  must be registered in the same order as they appear in the function.
 *
 *  @param TYPE the type of the parameter. If the class that this member
 *  function belongs to has a member variable with the same name as @a NAME
 *  you can use the special value '_'. If @c _ is used then the type of this
 *  function parameter is deduced from the type of the member variable @a NAME.
 *
 *  @param NAME the name of the parameter (an identifier not a string).
 *
 *  @see MIRROR_REG_MEM_FUNCTION_BEGIN
 *  @see MIRROR_REG_MEM_FUNCTION_END
 *  @see MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  @see MIRROR_REG_MEM_FUNCTIONS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_FUNCTION_PARAM(TYPE, NAME)
#else
#define MIRROR_REG_MEM_FUNCTION_PARAM(TYPE, NAME) \
	MIRROR_REG_MEM_FUNCTION_PARAM_CTS(TYPE, NAME, ())
#endif

#define MIRROR_REG_CONSTRUCTOR_PARAM_CTS(TYPE, NAME, SPELLED) \
	MIRROR_REG_MEM_FUNCTION_PARAM_CTS(TYPE, NAME, SPELLED)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that registers a single parameter of a constructor
/**
 *  This macro must be used between the MIRROR_REG_CONSTRUCTOR_BEGIN
 *  and the MIRROR_REG_CONSTRUCTOR_END macros. The parameters
 *  must be registered in the same order as they appear in the constructor.
 *
 *  @param TYPE the type of the parameter. If the class that this constructor
 *  belongs to has a member variable with the same name as @a NAME
 *  you can use the special value '_'. If @c _ is used then the type of this
 *  constructor parameter is deduced from the type of the member variable
 *  @a NAME.
 *
 *  @param NAME the name of the parameter (an identifier not a string).
 *
 *  @see MIRROR_REG_CONSTRUCTOR_BEGIN
 *  @see MIRROR_REG_CONSTRUCTOR_END
 *  @see MIRROR_REG_CONSTRUCTORS_BEGIN
 *  @see MIRROR_REG_CONSTRUCTORS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CONSTRUCTOR_PARAM(TYPE, NAME)
#else
#define MIRROR_REG_CONSTRUCTOR_PARAM(TYPE, NAME) \
	MIRROR_REG_MEM_FUNCTION_PARAM(TYPE, NAME)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that begins the registering of the class' member functions
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_MEM_FUNCTIONS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_FUNCTIONS_BEGIN
#else
#define MIRROR_REG_MEM_FUNCTIONS_BEGIN \
	typedef std::integral_constant<int, 0>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of the class' member functions
/**
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_FUNCTIONS_END
#else
#define MIRROR_REG_MEM_FUNCTIONS_END \
	mem_ovldfn_count;
#endif

#define MIRROR_REG_MEM_OVLD_FUNC_BEGIN_CTS(NAME, SPELLED) \
	mem_ovldfn_idx_ ## NAME ; \
	static const char* memfn_name( mem_ovldfn_idx_ ## NAME) \
	{ \
		return #NAME; \
	} \
	static size_t memfn_name_length( mem_ovldfn_idx_ ## NAME) \
	{ \
		return sizeof(#NAME) - 1; \
	} \
	static typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type memfn_ct_name( mem_ovldfn_idx_ ## NAME); \
	struct mem_ovldfn_ ## NAME { \
		typedef std::integral_constant<int, 0>


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that starts the registering of a (overloaded) member function
/**
 *  This macro must be used between the MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  and the MIRROR_REG_MEM_FUNCTIONS_END macros. This macro only registers
 *  the things common to all functions with the same name. The individual
 *  overloads are registered by the MIRROR_REG_MEM_FUNCTION_BEGIN and
 *  MIRROR_REG_MEM_FUNCTION_END macros.
 *
 *  @see MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  @see MIRROR_REG_MEM_FUNCTIONS_END
 *  @see MIRROR_REG_MEM_OVLD_FUNC_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_OVLD_FUNC_BEGIN(NAME)
#else
#define MIRROR_REG_MEM_OVLD_FUNC_BEGIN(NAME) \
	MIRROR_REG_MEM_OVLD_FUNC_BEGIN_CTS(NAME, ())
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of a (overloaded) member function
/**
 *  This macro must be used between the MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  and the MIRROR_REG_MEM_FUNCTIONS_END macros. This macro only registers
 *  the things common to all functions with the same name. The individual
 *  overloads are registered by the MIRROR_REG_MEM_FUNCTION_BEGIN and
 *  MIRROR_REG_MEM_FUNCTION_END macros.
 *
 *  @see MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  @see MIRROR_REG_MEM_FUNCTIONS_END
 *  @see MIRROR_REG_MEM_OVLD_FUNC_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_OVLD_FUNC_END(NAME)
#else
#define MIRROR_REG_MEM_OVLD_FUNC_END(NAME) \
		memfn_count; \
	}; \
	static mem_ovldfn_ ## NAME mem_ovldfn( mem_ovldfn_idx_ ## NAME ); \
	typedef std::integral_constant<int, mem_ovldfn_idx_ ## NAME ::value + 1>
#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of a single class' member function
/**
 *  This macro must be used between the MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  and the MIRROR_REG_MEM_FUNCTIONS_END macros.
 *
 *  @param ACCESS_TYPE the access type specifier (private|protected|public|_)
 *  In case the special '_' specifier is used, then the access type is
 *  determined from the elaborated type specifier of the parent class
 *
 *  @param LINKAGE_TYPE the linkage type or storage class specifier
 *  for the member function (static|_).
 *
 *  @param RESULT_TYPE the type of the result of the member function.
 *
 *  @param NAME the name of the member function.
 *
 *  @param ID a unique identifier of the member function within
 *  the overloads with the same name.
 *  Same rules as for C++ identifiers apply except it is possible
 *  for the identifier to start with a digit. i.e. 1 is a valid member
 *  function identifier.
 *
 *  @see MIRROR_REG_MEM_FUNCTION_END
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  @see MIRROR_REG_MEM_FUNCTIONS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_FUNCTION_BEGIN( \
	ACCESS_TYPE, \
	LINKAGE_TYPE, \
	RESULT_TYPE, \
	NAME, \
	ID \
)
#else
#define MIRROR_REG_MEM_FUNCTION_BEGIN( \
	ACCESS_TYPE, \
	LINKAGE_TYPE, \
	RESULT_TYPE, \
	NAME, \
	ID \
) \
	memfn_idx_ ## ID; \
	typedef  member< \
		spec_ ## ACCESS_TYPE ## _tag, \
		spec_ ## LINKAGE_TYPE ## _tag, \
		RESULT_TYPE \
	> memfn_ ## ID; \
	static memfn_ ## ID memfn_traits(memfn_idx_ ## ID); \
	template <typename _That> \
	struct memfn_wrapper_ ## ID \
	{ \
		_That& instance; \
		memfn_wrapper_ ## ID(_That& inst) \
		 : instance(inst) \
		{ } \
		\
		template <typename ... _Params > \
		RESULT_TYPE operator()(_Params ... params) const \
		{ \
			return instance.NAME(params ... ); \
		} \
	};\
	struct memfn_params_ ## ID \
	{ \
		typedef std::integral_constant<int, 0>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro that ends the registering of a single class' member function
/**
 *  This macro must be used between the MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  and the MIRROR_REG_MEM_FUNCTIONS_END macros.
 *
 *  @param NAME the name of the parameter (an identifier not a string).
 *
 *  @param ID a unique identifier of the member function within
 *  the overloads with the same name.
 *  Same rules as for C++ identifiers apply except it is possible
 *  for the identifier to start with a digit. i.e. 1 is a valid member
 *  function identifier.
 *
 *  @param CONST the constness specifier. Can be either @c non_const or @c _
 *  for non-const member functions or @c const for const member functions.
 *
 *  @see MIRROR_REG_MEM_FUNCTION_BEGIN
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_MEM_FUNCTIONS_BEGIN
 *  @see MIRROR_REG_MEM_FUNCTIONS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_MEM_FUNCTION_END(ID, CONST)
#else
#define MIRROR_REG_MEM_FUNCTION_END(ID, CONST) \
		size; \
	}; \
	static memfn_params_ ## ID memfn_params( \
		memfn_idx_ ## ID \
	); \
	static spec_ ## CONST ## _tag memfn_constness(memfn_idx_ ## ID);\
	static memfn_wrapper_ ## ID < \
		aux::memfn_inst_type<spec_ ## CONST ## _tag, _this>::type  \
	> memfn_wrapper( \
		memfn_idx_ ## ID, \
		aux::memfn_inst_type<spec_ ## CONST ## _tag, _this>::type& inst\
	) \
	{ \
		return memfn_wrapper_ ## ID < \
			aux::memfn_inst_type<spec_ ## CONST ## _tag, _this>::type\
		>(inst); \
	} \
	typedef std::integral_constant<int, memfn_idx_ ## ID ::value + 1>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for the registering of types the class is convertible to
/** This macro allows to register the other types that the currently
 *  registered class is convertible into, i.e. has the apropriate
 *  conversion operators.
 *
 *  This macro must be used between the MIRROR_REG_CLASS_BEGIN
 *  and the MIRROR_REG_CLASS_END macros.
 *
 *  @param TYPELIST the comma separated list of fully qualified types
 *  enclosed in parentheses.
 *
 *  @code
 *  MIRROR_REG_CLASS_BEGIN(class, myproject, X)
 *    MIRROR_REG_CLASS_CONVERSIONS((::myproject::Y, ::myproject::Z))
 *  MIRROR_REG_CLASS_END
 *  @endcode
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_CONVERSIONS(TYPELIST)
#else
#define MIRROR_REG_CLASS_CONVERSIONS(TYPELIST) \
	typedef mp::range< \
		MIRROR_PP_EXPAND_ARGS TYPELIST \
	> conv_ops;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Grants access to private members of a class to Mirror
/** This macro is used to grant access to private and protected
 *  members of the class @a CLASS to Mirror's meta-objects.
 *  It should be used inside of class declaration and basically
 *  translates into one or several 'friend' declarations.
 *
 *  This macro is necessary if you want to reflect non-public
 *  member variables of a class directly i.e. without using
 *  public getter / setter member functions.
 *
 *  @see MIRROR_REG_CLASS_MEM_VARS_BEGIN
 *  @see MIRROR_REG_CLASS_MEM_VARS_END
 *  @see MIRROR_REG_CLASS_MEM_VAR_GET_SET
 *  @see MIRROR_REG_CLASS_MEM_VAR
 *
 *  @ingroup registering_macros
 */
#define MIRROR_FRIENDLY_CLASS(CLASS)
#else
#define MIRROR_FRIENDLY_CLASS(CLASS) \
	friend class mirror::_class::_< CLASS >
#endif

template <class Class, class BaseMetaInheritance>
struct meta_inheritance
{
	typedef meta_class<Class> derived_class;

	typedef typename aux::inheritance_type_tag<
		typename BaseMetaInheritance::inherit
	>::type inheritance_type;

	typedef typename aux::access_type_tag<
		typename _class::_<Class>::kind,
		typename BaseMetaInheritance::access
	>::type access_type;

	typedef meta_class<typename BaseMetaInheritance::base> base_class;
};


template <class Class, int Index>
struct meta_member_variable
{
private:
	typedef std::integral_constant<int, Index> _idx;
	// Get the base traits of the type from the registered function
	typedef decltype(
		mirror::_class::_<Class>::attr_traits(_idx())
	) _base_traits;
public:

	typedef meta_class< Class > scope;

	typedef typename aux::access_type_tag<
		typename mirror::_class::_<Class>::kind,
		typename _base_traits::access_type
	>::type access_type;

	typedef typename aux::storage_class_tag_mv<
		typename _base_traits::storage_class
	>::type storage_class;

	typedef typename aux::reflect_type<
		typename _base_traits::type_sel
	>::type type;

private:
	template <typename Local>
	static std::string make_name(Local local)
	{
		// get the full or local name of the scope
		std::string result(
			Local::value ?
			scope::local_name() :
			scope::full_name()
		);
		// append the separating double colon
		result.append("::");
		// and append the base name of this member variable
		result.append(base_name());
		// return the result
		return result;
	}
public:
	typedef decltype(
		mirror::_class::_<Class>::attr_ct_name(_idx())
	) static_name;

	// returns the base name of the member variable
	static std::string base_name(void)
	{
		return std::string(
			mirror::_class::_<Class>::attr_name(_idx()),
			mirror::_class::_<Class>::attr_name_length(_idx())
		);
	}

	// returns the full name (including the full nested name qualifier)
	static std::string full_name(void)
	{
		return make_name(std::false_type());
	}

	// returns the local name (including partial nested name qualifier)
	static std::string local_name(void)
	{
		return make_name(std::true_type());
	}

	// returns the value of the member when given a reference to Class
	static auto get(const Class& inst) ->
	decltype(mirror::_class::_<Class>::get(_idx(), inst))
	{
		return mirror::_class::_<Class>::get(_idx(), inst);
	}

	// sets the value of the member when given a reference to Class
	template <typename ValueType>
	static void set(Class& inst, const ValueType& value)
	{
		mirror::_class::_<Class>::set(_idx(), inst, value);
	}

private:
	typedef decltype(mirror::_class::_<Class>::has_address(_idx()))
		has_address;

	friend struct meta_object_category<meta_member_variable>;
public:
	// gets the address of the member variable if possible
	// users should always check if the meta-variable conforms
	// to MetaPlainMemberVariable before trying to call address()
	static auto address(Class& inst) ->
	decltype(mirror::_class::_<Class>::address(_idx(), inst))
	{
		return mirror::_class::_<Class>::address(_idx(), inst);
	}

	// gets the address of the member variable if possible
	static auto address(const Class& inst) ->
	decltype(mirror::_class::_<Class>::address(_idx(), inst))
	{
		return mirror::_class::_<Class>::address(_idx(), inst);
	}

	// gets the address of the member variable if possible
	static auto member_pointer(void) ->
	decltype(mirror::_class::_<Class>::mem_ptr(_idx()))
	{
		return mirror::_class::_<Class>::mem_ptr(_idx());
	}

	// NOTE: internal implementation detail do not use
	template <typename X>
	struct _by_name_typ
	{
		typedef decltype(
			mirror::_class::_<Class>::by_name_typ(
				_idx(),
				(X*)nullptr
			)
		) type;
	};

	// NOTE: internal implementation detail do not use
	template <typename X>
	struct _by_name_val
	{
		typedef decltype(
			mirror::_class::_<Class>::by_name_val(
				_idx(),
				(X*)nullptr
			)
		) type;
	};
};

template <class Class, int Index>
struct meta_object_category< meta_member_variable<Class, Index> >
{
private:
	typedef meta_member_variable<Class, Index> mmv;

	static meta_member_variable_tag _get_type(std::false_type);

	static meta_plain_member_variable_tag _get_type(std::true_type);
public:
	typedef decltype(_get_type(typename mmv::has_address())) type;
};

template <class Class>
struct meta_class
 : public meta_type<Class>
{
	typedef typename aux::elaborated_type_tag<
		typename _class::_<Class>::kind
	>::type elaborated_type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the class passed as the @a FULL_CLASS_NAME argument
/**
 *  This macro expands into a type conforming to the MetaClass
 *  concept, which provides meta-data about the given class.
 *  The class name passed as the @a FULL_CLASS_NAME argument
 *  should be a fully qualified class name.
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see mirror::MetaClass
 *  @see mirror::reflected
 *  @see lagoon::reflected_type
 *  @see lagoon::reflected_class
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_CLASS(FULL_CLASS_NAME) mirror::MetaClass
#else
#define MIRRORED_CLASS(FULL_CLASS_NAME) \
	mirror::meta_class< FULL_CLASS_NAME >
#endif

// The implementation of base_classes for instantiation of meta_class
template <class Class>
struct base_classes<meta_class<Class> >
{
	// A range of MetaInheritances reflecting the base-classes
	typedef typename aux::base_classes_helper<
		Class,
		typename _class::_<Class>::base_classes
	>::type type;
};

// The implementation of member_variables for instantiation of meta_class
template <class Class>
struct member_variables<meta_class<Class> >
{
	// A range of MetaMemberVariables reflecting the member variables
	typedef typename mp::apply_on_seq_pack<
		aux::mem_var_helper<Class>,
		typename mirror::_class::_<Class>::attr_count
	>::type type;
};

// The implementation of the class_layout meta-function for meta-classes
template <class Class>
struct class_layout<meta_class<Class> >
{
private:
	typedef aux::class_layout_helper<
		typename base_classes<
			meta_class<Class>
		>::type,
		meta_class<Class>,
		spec_non_virtual_tag
	> helper;
public:

	// the resulting range contains MetaClass(es)
	// in an order reflecting the layout of the class
	typedef typename mp::concat<
		typename mp::unique<typename helper::virt>::type,
		typename helper::reg
	>::type type;
};

// Implementation of the all_member_variables meta-function for meta-classes
template <class Class>
struct all_member_variables<meta_class<Class> >
{
	// the returned range contains MetaMemberVariables
	// reflecting the individual member variables of the Class
	typedef typename mp::fold<
		class_layout<meta_class<Class> >,
		mp::range<>,
		mp::concat<
			mp::arg<1>,
			member_variables<mp::arg<2> >
		>
	>::type type;
};


// implementation of the meta_constructor template
template <typename Type, int Index>
class meta_constructor
 : public aux::scoped_named_impl<
	aux::meta_constructor_base<Type, Index>
>
{
private:
	typedef std::integral_constant<int, Index> _idx;
public:
	// the scope of the constructor
	typedef meta_class< Type > scope;

	typedef typename aux::access_type_tag<
		typename _class::_<Type>::kind,
		decltype(_class::_<Type>::ctr_access(_idx()))
	>::type access_type;

	// the result of the constructor
	typedef typename aux::reflect_type<Type>::type result_type;
	typedef spec_static_tag storage_class;
	typedef spec_static_tag linkage;

	typedef decltype(_class::_<Type>::ctr_kind(_idx())) kind;
};

// The implementation of constructors for instantiations of meta_class
template <class Type>
struct constructors<meta_type<Type> >
{
	// A range of MetaMemberVariables reflecting the member variables
	typedef typename mp::apply_on_seq_pack<
		aux::constructor_helper<Type>,
		typename mirror::_class::_<Type>::ctr_count
	>::type type;
};

// The implementation of constructors for instantiations of meta_class
template <class Class>
struct constructors<meta_class<Class> >
 : constructors<meta_type<Class> >
{ };

// Implementation of meta_constructor_param template
template <typename Type, int ConstrIndex, int ParamIndex>
struct meta_constructor_param
 : public aux::scoped_named_impl<
	aux::meta_ctr_param_base<
		Type,
		ConstrIndex,
		ParamIndex
	>
>
{
private:
	typedef mirror::_class::_<Type> base_meta_type;
	typedef decltype(
		base_meta_type::ctr_params(
			std::integral_constant<int, ConstrIndex>()
		).type(
			std::integral_constant<int, ParamIndex>()
		)
	) type_helper;
public:
	typedef decltype(
		base_meta_type::ctr_params(
			std::integral_constant<int, ConstrIndex>()
		).ct_name(
			std::integral_constant<int, ParamIndex>()
		)
	) static_name;

	typedef spec_auto_tag storage_class;

	typedef meta_constructor<Type, ConstrIndex> scope;

	typedef typename aux::reflect_type<
		typename type_helper::type
	>::type type;

	// the type of the constructor parameter
	typedef std::integral_constant<int, ParamIndex> position;
};

// Implementation of the parameters meta-function template class
template <typename Type, int ConstrIndex>
struct parameters<meta_constructor<Type, ConstrIndex> >
{
private:
	typedef decltype(
		mirror::_class::_<Type>
		::ctr_params(
			std::integral_constant<int, ConstrIndex >()
		)
	) size_helper;
public:
	// A range of MetaVariables reflecting
	// the parameters of a meta-constructor
	typedef typename mp::apply_on_seq_pack<
		aux::ctr_param_helper<Type, ConstrIndex>,
		typename size_helper::size
	>::type type;
};

// implementation of the meta_initializer template
template <typename Type>
class meta_initializer
 : public aux::scoped_named_impl<
	aux::meta_ctr_base_base<Type>
>
{
public:
	typedef typename meta_class< Type >::static_name static_name;

	typedef meta_class< Type > scope;

	typedef spec_public_tag access_type;

	typedef typename aux::reflect_type<Type>::type result_type;
	typedef spec_static_tag storage_class;
	typedef spec_static_tag linkage;

	typedef struct_initializer_tag kind;
};

// Implementation of meta_constructor_param template
template <typename Type>
struct meta_initializer_param
 : public aux::scoped_named_impl<aux::meta_initlzr_param_base<Type> >
{
	typedef typename mirror::cts::string<>::type static_name;

	typedef spec_auto_tag storage_class;

	typedef meta_initializer<Type> scope;

	typedef typename aux::reflect_type<Type>::type type;

	typedef std::integral_constant<int, 0> position;
};

// Implementation of the parameters meta-function template class
template <typename Type>
struct parameters<meta_initializer<Type> >
{
	typedef mp::range<meta_initializer_param<Type> > type;
};


// implementation of the meta_member_function template
template <typename Class, int FuncIndex, int OvldIndex>
class meta_member_function
 : public aux::scoped_named_impl<
	aux::meta_mem_function_base<Class, FuncIndex>
>
{
private:
	typedef std::integral_constant<int, FuncIndex> _func_idx;
	typedef std::integral_constant<int, OvldIndex> _ovld_idx;

	// the set of overloaded functions
	typedef decltype(
		_class::_<Class>::
		mem_ovldfn(_func_idx())
	) _ovld_funcs;

	// the traits for this specific overload
	typedef decltype(
		_ovld_funcs::memfn_traits(_ovld_idx())
	) _base_traits;
public:
	typedef mirror::meta_class<Class> scope;

	typedef typename aux::access_type_tag<
		typename _class::_<Class>::kind,
		typename _base_traits::access_type
	>::type access_type;

	typedef typename aux::storage_class_tag_mf<
		typename _base_traits::storage_class
	>::type storage_class;
	typedef storage_class linkage;

	// the result of the constructor
	typedef typename aux::reflect_type<
		typename _base_traits::type_sel
	>::type result_type;

	typedef typename aux::constness_tag<
		decltype(
			_ovld_funcs::
			memfn_constness(_ovld_idx())
		)
	>::type constness;

	// makes an instance of the wrapper which allows
	// to call the member function reflected by this
	// MetaMemberFunction
	static inline auto wrap(Class& instance) ->
	decltype(_ovld_funcs::memfn_wrapper(
		_ovld_idx(),
		instance
	))
	{
		return _ovld_funcs::memfn_wrapper(
			_ovld_idx(),
			instance
		);
	}
};

template <typename Class, int FuncIndex>
class meta_ovlded_mem_funcs
 : public aux::scoped_named_impl<
	aux::meta_mem_function_base<Class, FuncIndex>
>
{
private:
	typedef mirror::_class::_<Class> base_meta_class;
public:
	typedef decltype(mirror::_class::_<Class>::memfn_ct_name(
		std::integral_constant<int, FuncIndex>()
	)) static_name;

	typedef mirror::meta_class<Class> scope;
};

// The implementation of member_functions for instantiations of meta_class
template <class Class>
struct member_functions<meta_class<Class> >
{
	// A range of MetaOverloadedFunctions reflecting the overloaded functions
	typedef typename mp::apply_on_seq_pack<
		aux::mem_ovlded_func_helper<Class>,
		typename mirror::_class::_<Class>::mem_ovldfn_count
	>::type type;
};

// The implementation of overloads for meta_ovlded_mem_funcs
template <class Class, int FuncIndex>
struct overloads<meta_ovlded_mem_funcs<Class, FuncIndex> >
{
	typedef decltype(
		mirror::_class::_<Class>::mem_ovldfn(
			std::integral_constant<int, FuncIndex>()
		)
	) _ovld_fns;
	// A range of MetaMemberFunctions reflecting the member functions
	typedef typename mp::apply_on_seq_pack<
		aux::mem_function_helper<Class, FuncIndex>,
		typename _ovld_fns::memfn_count
	>::type type;
};

template <typename Class, int FuncIndex, int OvldIndex, int ParamIndex>
struct meta_mem_function_param
 : public aux::scoped_named_impl<
	aux::meta_memfn_param_base<
		Class,
		FuncIndex,
		OvldIndex,
		ParamIndex
	>
>
{
private:
	typedef mirror::_class::_<Class> base_meta_type;
	typedef decltype(
		base_meta_type::mem_ovldfn(
			std::integral_constant<int, FuncIndex>()
		).memfn_params(
			std::integral_constant<int, OvldIndex>()
		).type(
			std::integral_constant<int, ParamIndex>()
		)
	) type_helper;
public:
	typedef decltype(
		base_meta_type::mem_ovldfn(
			std::integral_constant<int, FuncIndex>()
		).memfn_params(
			std::integral_constant<int, OvldIndex>()
		).ct_name(
			std::integral_constant<int, ParamIndex>()
		)
	) static_name;

	typedef spec_auto_tag storage_class;

	typedef meta_member_function<Class, FuncIndex, OvldIndex> scope;

	// the type of the function parameter
	typedef meta_class< typename type_helper::type > type;

	// the position of the function parameter
	typedef std::integral_constant<int, ParamIndex> position;

};


// Implementation of the parameters meta-function template class
template <typename Class, int FuncIndex, int OvldIndex>
struct parameters<meta_member_function<Class, FuncIndex, OvldIndex> >
{
private:
	typedef decltype(
		mirror::_class::_<Class>::
		mem_ovldfn(
			std::integral_constant<int, FuncIndex >()
		).memfn_params(
			std::integral_constant<int, OvldIndex >()
		)
	) size_helper;
public:
	// A range of MetaVariables reflecting
	// the parameters of a meta-constructor
	typedef typename mp::apply_on_seq_pack<
		aux::memfn_param_helper<Class, FuncIndex, OvldIndex>,
		typename size_helper::size
	>::type type;
};


// implementation of the meta_conversion_operator template
template <typename Class, typename Result>
class meta_conversion_operator
 : public aux::scoped_named_impl<
	aux::meta_conv_op_base<Class, Result>
>
{
public:
	typedef mirror::meta_class<Class> scope;

	typedef spec_public_tag access_type;

	typedef spec_auto_tag storage_class;
	typedef spec_auto_tag linkage;

	typedef mirror::meta_class<Result> result_type;
};

// The implementation of conversions for instantiations of meta_class
template <class Class>
struct conversions<meta_class<Class> >
{
private:
	typedef mirror::_class::_<Class> base_meta_class;
public:
	// A range of MetaConversionOperators reflecting the conversions
	typedef typename aux::conv_op_helper<
		Class,
		typename base_meta_class::conv_ops
	>::type type;
};


// Implementation of the parameters meta-function template class
template <typename Class, typename Result>
struct parameters<meta_conversion_operator<Class, Result> >
{
public:
	// An empty range reflecting
	// the parameters of a conversion operator
	typedef typename mp::range<> type;
};


// Implementation of the members meta-function for constructors
template <typename Type, int ConstrIndex>
struct members<meta_constructor<Type, ConstrIndex> >
{
	typedef typename parameters<
		meta_constructor<Type, ConstrIndex>
	>::type type;
};

template <typename Type>
struct members<meta_initializer<Type> >
{
	typedef typename parameters<meta_initializer<Type> >::type type;
};

// Implementation of the members meta-function for functions
template <typename Type, int FuncIndex, int OvldIndex>
struct members<meta_member_function<Type, FuncIndex, OvldIndex> >
{
	typedef typename parameters<
		meta_member_function<Type, FuncIndex, OvldIndex>
	>::type type;
};

// Implementation of the members meta-function for conversion operators
template <typename Class, typename Result>
struct members<meta_conversion_operator<Class, Result> >
{
	typedef typename parameters<
		meta_conversion_operator<Class, Result>
	>::type type;
};

// Implementation of the members meta-function for classes
template <class Class>
struct members<meta_class<Class> >
{
	//TODO: this should be finished when other class members
	// are supported
	typedef typename all_member_variables<
		meta_class<Class>
	>::type type;
};

MIRROR_NAMESPACE_END

#endif //include guard

