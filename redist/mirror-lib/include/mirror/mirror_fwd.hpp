/**
 * @file mirror/mirror_fwd.hpp
 * @brief Forward declaration of the meta-level classes reflecting
 * base-level program constructs.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_MIRROR_FWD_1011291729_HPP
#define MIRROR_MIRROR_FWD_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/specifier_tags.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename MetaObjectCategory>
struct meta_meta_object;

//  Specialization of meta_object_category for meta_namespaces
template <typename MetaObjectCategory>
struct meta_object_category< meta_meta_object<MetaObjectCategory> >
{
	// returns meta_global_scope_tag
	typedef meta_meta_object_tag type;
};

//  Namespace containing the implementation details
/*  This namespace contains internal implementation
 *  details of various parts of the Mirror library.
 *  Do not use anything defined inside of this namespace
 *  because these implementation details are subject to
 *  change without any notice.
 */
namespace aux { }

/// Returns the type of parameters for intrinsic meta-functions
/** This template can be used to get the tag of the concept
 *  required by the intrinsic unary meta-function template
 *  @a Getter.
 *
 *  @see categorize_meta_object
 *  @see meta_object_category
 */
template <template <class> class Getter>
struct required_param_of;

//  Forward declaration of the meta_namespace template class
template <class Namespace>
struct meta_namespace;

//  Specialization of meta_object_category for meta_namespaces
template <class Namespace>
struct meta_object_category< meta_namespace<Namespace> >
{
	typedef meta_namespace_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the members of a scope
/** This meta-function returns a MetaScopedObject range containing
 *  all registered members of the scope (namespace, class, etc.).
 *  @see MetaScope
 *  @see MetaScopedObject
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaScope>
struct members
{
	/// A mp::range of MetaScopedObjects reflecting the members
	typedef Range<MetaScopedObject> type;
};
#else
template <class MetaScope>
struct members;
#endif

template <>
struct required_param_of<members>
{
	typedef mirror::meta_scope_tag type;
};

//  Namespace containing basic meta-data about registered namespaces
/*  Every namespace registered with Mirror has its counterpart inside
 *  of this (Mirror's internal _namespace) namespace. Each of this
 *  counterparts must contain a structure called _ (a single underscore)
 *  which provides the basic meta-data about the namespace.
 *  Do NOT use the contents of this namespace directly for anything
 *  else than registering the namespaces. Use the meta_namespace
 *  template for reflecting of namespaces.
 */
namespace _namespace {

// This structure provides basic meta-data about the global scope namespace
struct _;
// This structure provides basic meta-data about an unspecified scope
struct _unspec_scope;

} // namespace _namespace

template <>
struct meta_object_category< meta_namespace<_namespace::_> >
{
	typedef meta_global_scope_tag type;
};

template <>
struct meta_object_category< meta_namespace<_namespace::_unspec_scope> >
{
	typedef meta_unspecified_scope_tag type;
};

//  Forward declaration of the meta_type template class
template <class Type>
struct meta_type;

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the parameters of a template instantiation
/** This meta-function returns a MetaType range containing
 *  all parameters used in the template instantiation.
 *  @see MetaTemplatedType
 *  @see MetaType
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaTemplatedType>
struct template_parameters
{
	/// A mp::range of MetaTypes reflecting the parameters
	typedef Range<MetaType> type;
};
#else
template <class MetaTemplatedType>
struct template_parameters;
#endif

template <>
struct required_param_of<template_parameters>
{
	typedef mirror::meta_templated_type_tag type;
};

//  Namespace containing basic meta-data about registered types
/*  Every type registered with Mirror has a separate specialization
 *  of a helper structure containing the basic meta-data about
 *  the registered type
 */
namespace _type {

// Specialization of this structure provide basic meta-data about types
template <typename Type>
struct _;

} // namespace _type

//  Specialization of meta_object_category for meta_types
template <class Type>
struct meta_object_category< meta_type<Type> >
{
	// returns meta_type_tag/meta_templated_type_tag/...
	typedef typename meta_type<Type>::_type_category type;
};

//  Namespace containing basic meta-data about registered typedefs
/*  Every typedef registered with Mirror has a separate specialization
 *  of a helper structure containing the basic meta-data about
 *  the registered typedefined type
 */
namespace _typedef {

// This type is used for distinguishing typedefined types from other T<x>
// template types
template <typename BaseTypedef>
struct _selector;

} // namespace _typedef

//  Specialization of meta_object_category for meta_types
template <class BaseTypedef>
struct meta_object_category<
	meta_type<_typedef::_selector<BaseTypedef> >
>
{
	// meta_typedef_tag
	typedef meta_typedef_tag type;
};


//  Forward declaration of the meta_type_template template class
template <template <typename ...> class Template>
struct meta_type_template;

//  Namespace containing basic meta-data about registered templates
/*  Every template type registered with Mirror has a separate specialization
 *  of a helper structure containing the basic meta-data about
 *  the registered template type
 */
namespace _template {

// Specialization of this structure provide basic meta-data about templates
template <template <typename ...> class Template>
struct _;

} // namespace _template


//  Specialization of meta_object_category for meta_types
template <template <typename ...> class Template>
struct meta_object_category< meta_type_template<Template> >
{
	// returns meta_type_template_tag
	typedef meta_type_template_tag type;
};


// Forward declaration of the meta_class template
template <class Class>
struct meta_class;

//  Namespace containing basic meta-data about registered elaborate types
/*  Every class registered with Mirror has a separate specialization
 *  of a helper structure containing the basic meta-data about
 *  the registered elaborated type
 */
namespace _class {

// Specialization of this structure provide basic meta-data about classes
template <typename Class>
struct _;

} // namespace _class

//  Specialization of meta_object_category for meta_classes
template <class Class>
struct meta_object_category< meta_class<Class> >
{
	// returns meta_class_tag/meta_templated_class_tag
	typedef typename meta_class<Class>::_class_category type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the nested namespaces of a namespace
/** This meta-function returns a MetaNamespace range containing
 *  all registered nested namespaces of the reflected namespace.
 *
 *  @see MetaNamespace
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaNamespace>
struct namespaces
{
	/// A mp::range of MetaNamespaces reflecting the nested namespaces
	typedef Range<MetaNamespace> type;
};
#else
template <class MetaNamespace>
struct namespaces;
#endif

template <>
struct required_param_of<namespaces>
{
	typedef mirror::meta_namespace_tag type;
};


template <class VariableSelector>
struct meta_free_variable;

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the free (global) variables of a namespace
/** This meta-function returns a MetaFreeVariable range containing
 *  all registered free variables of the reflected namespace.
 *
 *  @see MetaNamespace
 *  @see MetaFreeVariable
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaNamespace>
struct free_variables
{
	/// A mp::range of MetaFreeVariables reflecting the free variables
	typedef Range<MetaFreeVariable> type;
};
#else
template <class MetaNamespace>
struct free_variables;
#endif

template <>
struct required_param_of<free_variables>
{
	typedef mirror::meta_namespace_tag type;
};

template <class VariableSelector>
struct meta_free_variable;

//  Namespace containing basic meta-data about registered free variables
/*  Every free variable registered with Mirror is registered inside
 *  of this (Mirror's internal _variable) namespace.
 *  Do NOT use the contents of this namespace directly for anything
 *  else than registering the variables.
 */
namespace _variable { }

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the free (global) functions of a namespace
/** This meta-function returns a MetaOverloadedFunctions range containing
 *  all registered overloaded free functions of the reflected namespace.
 *
 *  @see MetaNamespace
 *  @see MetaOverloadedFunctions
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaNamespace>
struct free_functions
{
	/// A mp::range of MetaOverloadedFunctions reflecting the free functions
	typedef Range<MetaOverloadedFunctions> type;
};
#else
template <class MetaNamespace>
struct free_functions;
#endif

template <>
struct required_param_of<free_functions>
{
	typedef mirror::meta_namespace_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the individual overloads of a function
/** This meta-function returns a range of MetaFunctions containing
 *  all registered overloaded functions of a scope.
 *
 *  @see MetaOverloadedFunctions
 *  @see MetaFunction
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaOverloadedFunctions>
struct overloads
{
	/// A mp::range of MetaFunctions reflecting the individial functions
	typedef Range<MetaFunction> type;
};
#else
template <class MetaOverloadedFunctions>
struct overloads;
#endif

template <>
struct required_param_of<overloads>
{
	typedef mirror::meta_class_tag type;
};

template <class VariableSelector>
struct meta_ovlded_free_funcs;

//  Specialization of meta_object_category for meta_ovlded_free_funcs'
template <typename Selector>
struct meta_object_category<meta_ovlded_free_funcs<Selector> >
{
	typedef meta_overloaded_functions_tag type;
};

template <class VariableSelector, int Index>
struct meta_free_function;

template <typename FunctionSelector, int FuncIndex>
struct meta_object_category<
	meta_free_function<FunctionSelector, FuncIndex>
>
{
	typedef meta_function_tag type;
};

// Forward declaration of the meta_free_func_param template
template <typename FunctionSelector, int FuncIndex, int ParamIndex>
struct meta_free_func_param;

template <typename FunctionSelector, int FuncIndex, int ParamIndex>
struct meta_object_category<
	meta_free_func_param<FunctionSelector, FuncIndex, ParamIndex>
>
{
	typedef meta_parameter_tag type;
};

//  Namespace containing basic meta-data about registered free functions
/*  Every (overloaded) free function registered with Mirror is registered inside
 *  of this (Mirror's internal _function) namespace.
 *  Do NOT use the contents of this namespace directly for anything
 *  else than registering the variables.
 */
namespace _function { }

//  Namespace containing basic meta-data about registered container classes
/*  Every container registered with Mirror is registered inside of this
 *  (Mirror's internal) namespace .
 *  Do NOT use the contents of this namespace directly for anything
 *  else than registering the containers.
 */
namespace _container {

template <typename Container>
struct _;

} // namespace _container

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the base classes of a class
/** This meta-function returns a MetaInheritance range containing
 *  all registered base classes of the reflected class.
 *  @see MetaClass
 *  @see MetaInheritance
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct base_classes
{
	/// A mp::range of MetaInheritances reflecting the base classes
	typedef Range<MetaInheritance> type;
};
#else
template <class MetaClass>
struct base_classes;
#endif

template <>
struct required_param_of<base_classes>
{
	typedef mirror::meta_class_tag type;
};

// Forward declaration of the meta_inheritance template
template <class Class, class BaseMetaInheritance>
struct meta_inheritance;


//  Specialization of meta_object_category for meta_inheritances
template <class Class, class BaseMetaInheritance>
struct meta_object_category< meta_inheritance<Class, BaseMetaInheritance> >
{
	typedef meta_inheritance_tag type;
};

/** @defgroup mirror_intrinsic_meta_func Mirror - Intrinsic meta-functions
 *
 *  Some of the meta-objects provided by Mirror have besides their member
 *  functions and typedefs also intrinsic meta-function templates which
 *  can be used in compile-time meta-programming algorithms.
 */

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the reflected type for a MetaType
/** This meta-function returns the type that the MetaType
 *  passed as argument reflects
 *
 *  @see MetaType
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaType>
struct original_type
{
	/// same as MetaType::original_type
	typedef unspecified type;
};
#else
template <class MetaType>
struct original_type;
#endif

template <>
struct required_param_of<original_type>
{
	typedef mirror::meta_type_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the compile-time name of a MetaNamedObject
/**
 *
 *  @see CompileTimeString
 *  @see MetaNamedObject
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaNamedObject>
struct static_name
{
	/// Compile-time the name of the named object
	typedef CompileTimeString type;
};
#else
template <class MetaNamedObject>
struct static_name;
#endif

template <>
struct required_param_of<static_name>
{
	typedef mirror::meta_named_object_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the scope of a MetaScopedObject
/** This meta-function returns the MetaScope for the MetaScopedObject
 *
 *  @see MetaScope
 *  @see MetaScopedObject
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaScopedObject>
struct scope
{
	/// MetaScope reflecting the scope of the scoped object
	typedef MetaScope type;
};
#else
template <class MetaScopedObject>
struct scope;
#endif

template <>
struct required_param_of<scope>
{
	typedef mirror::meta_scoped_object_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning MetaType reflecting a modified original type
/** This Meta-function returns a MetaType reflecting a type that is the
 *  result of application of the @a ModifierLambdaExpression on the
 *  @c original_type reflected by the passed @c MetaType.
 *
 *  @tparam MetaType meta-type reflecting the original type to be
 *  modified and reflected.
 *
 *  @tparam ModifierLambdaExpression unary lambda expression doing
 *  the modification of the original type. For example:
 *  @code
 *  std::add_const<std::add_pointer<mirror::mp::arg<1>>>
 *  @endcode
 *  The type trait templates used to construct the modifier expression
 *  must not return a boolean or integral constants.
 *
 *  @see MetaType
 *  @see mp::arg
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaType, class ModifierLambdaExpression>
struct apply_modifier
{
	/// MetaType reflecting the type modified by the modifier expression
	typedef MetaType type;
};
#else
template <class MetaType, class ModifierLambdaExpression>
struct apply_modifier;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the container kind tag for a MetaClass
/** This meta-function returns the ContainerKindTag for the MetaClass
 *
 *  @see MetaClass
 *  @see ContainerKindTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaType>
struct container_kind
{
	/// indicating which kind of container type is reflected
	typedef ContainerTypeTag type;
};
#else
template <class MetaType>
struct container_kind;
#endif

template <>
struct required_param_of<container_kind>
{
	typedef mirror::meta_type_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the elaborated type specifier of a MetaClass
/** This meta-function returns the ElaboratedTypeTag for the MetaClass
 *
 *  @see MetaClass
 *  @see ElaboratedTypeTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaClass>
struct elaborated_type
{
	/// indicating which kind of elaborated type is reflected
	typedef ElaboratedTypeTag type;
};
#else
template <class MetaClass>
struct elaborated_type;
#endif

template <>
struct required_param_of<elaborated_type>
{
	typedef mirror::meta_class_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the storage class specifier of a MetaVariable
/** This meta-function returns the StorageClassTag for the MetaVariable
 *
 *  @see MetaVariable
 *  @see StorageClassTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaVariable>
struct storage_class
{
	/// indicating the storage class of the reflected variable
	typedef StorageClassTag type;
};
#else
template <class MetaVariable>
struct storage_class;
#endif

template <>
struct required_param_of<storage_class>
{
	typedef mirror::meta_variable_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the linkage specifier of a MetaFunction
/** This meta-function returns the StorageClassTag for the MetaFunction
 *
 *  @see MetaFunction
 *  @see StorageClassTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaFunction>
struct linkage
{
	/// indicating the linkage of the reflected function
	typedef StorageClassTag type;
};
#else
template <class MetaFunction>
struct linkage;
#endif

template <>
struct required_param_of<linkage>
{
	typedef mirror::meta_function_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the constness specifier of a MetaMemberFunction
/** This meta-function returns the ConstnessTag for the MetaMemberFunction
 *
 *  @see MetaMemberFunction
 *  @see ConstnessTag
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaMemberFunction>
struct constness
{
	/// indicating the constness of the reflected member function
	typedef ConstnessTag type;
};
#else
template <class MetaMemberFunction>
struct constness;
#endif

template <>
struct required_param_of<constness>
{
	typedef mirror::meta_member_function_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the inheritance type specifier of a MetaInheritance
/** This meta-function returns the InheritanceTypeTag for the MetaInheritance
 *
 *  @see MetaInheritance
 *  @see InheritanceTypeTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaInheritance>
struct inheritance_type
{
	/// indicating the type of inheritance reflected
	typedef InheritanceTypeTag type;
};
#else
template <class MetaInheritance>
struct inheritance_type;
#endif

template <>
struct required_param_of<inheritance_type>
{
	typedef mirror::meta_inheritance_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the base class of a class inheritance
/** This meta-function returns the MetaClass reflecting the base class
 *  in the inheritance reflected by MetaInheritance
 *
 *  @see MetaInheritance
 *  @see MetaClass
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaInheritance>
struct base_class
{
	/// MetaClass reflecting the base class in the MetaInheritance
	typedef MetaClass type;
};
#else
template <class MetaInheritance>
struct base_class;
#endif

template <>
struct required_param_of<base_class>
{
	typedef mirror::meta_inheritance_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the derived class in an class inheritance
/** This meta-function returns the MetaClass reflecting the derived class
 *  in the inheritance reflected by MetaInheritance
 *
 *  @see MetaInheritance
 *  @see MetaClass
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaInheritance>
struct derived_class
{
	/// MetaClass reflecting the base class in the MetaInheritance
	typedef MetaClass type;
};
#else
template <class MetaInheritance>
struct derived_class;
#endif

template <>
struct required_param_of<derived_class>
{
	typedef mirror::meta_inheritance_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning access type of the base-class in a MetaInheritance
/** This meta-function returns the AccessTypeTag reflecting the base class
 *  access in the MetaInheritance
 *
 *  @see MetaInheritance
 *  @see AccessTypeTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaInheritance>
struct base_class_access_type
{
	/// AccessTypeTag indicating the access type of a base-class
	typedef AccessTypeTag type;
};
#else
template <class MetaInheritance>
struct base_class_access_type;
#endif

template <>
struct required_param_of<base_class_access_type>
{
	typedef mirror::meta_inheritance_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning access type of a member reflected by MetaClassMember
/** This meta-function returns the AccessTypeTag reflecting the class member
 *  access
 *
 *  @see MetaClassMember
 *  @see AccessTypeTag
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaClassMember>
struct class_member_access_type
{
	/// AccessTypeTag indicating the access type of a class member
	typedef AccessTypeTag type;
};
#else
template <class MetaClassMember>
struct class_member_access_type;
#endif

template <>
struct required_param_of<class_member_access_type>
{
	typedef mirror::meta_class_member_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the template of a templated type
/** This meta-function returns the MetaTypeTemplate reflecting the template
 *  which was used for the instantiation of a templated type
 *
 *  @see MetaTemplatedType
 *  @see MetaTypeTemplate
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaTemplatedType>
struct type_template
{
	/// MetaTypeTemplate reflecting the template of a templated type
	typedef MetaTypeTemplate type;
};
#else
template <class MetaTemplatedType>
struct type_template;
#endif

template <>
struct required_param_of<type_template>
{
	typedef mirror::meta_templated_type_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the position of a parameter reflected by MetaParameter
/** This meta-function returns an IntegralConstantType indicating the position
 *  of a parameter in function parameter list
 *
 *  @see MetaParameter
 *  @see IntegralConstantType
 *  @see required_param_of
 *
 *  @ingroup mirror_intrinsic_meta_func
 */
template <class MetaParameter>
struct parameter_position
{
	/// IntegralConstantType indicating the position of a parameter
	typedef IntegralConstantType type;
};
#else
template <class MetaParameter>
struct parameter_position;
#endif

template <>
struct required_param_of<parameter_position>
{
	typedef mirror::meta_parameter_tag type;
};


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the member variables (attributes) of a class
/** This meta-function returns a MetaMemberVariable range containing
 *  all registered member variables (attributes) of the reflected class
 *  not including those inherited from the base classes.
 *
 *  @see MetaClass
 *  @see MetaMemberVariable
 *  @see all_member_variables
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct member_variables
{
	/// A mp::range of MetaMemberVariables reflecting the attributes
	typedef Range<MetaMemberVariable> type;
};
#else
template <class MetaClass>
struct member_variables;
#endif

template <>
struct required_param_of<member_variables>
{
	typedef mirror::meta_class_tag type;
};

// Forward declaration of the meta_member_variable template
template <class Class, int Index>
struct meta_member_variable;


// The specialization of meta_object_category for meta_member_variabless
// is defined in meta_class.hpp

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the layout of base classes of a class
/** This meta-function returns a MetaClass range containing
 *  all registered base classes (directly or indirectly inherited)
 *  of the reflected class.
 *
 *  @see MetaClass
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct class_layout
{
	/// A mp::range of MetaClasses reflecting the class layout
	typedef Range<MetaClass> type;
};
#else
template <class MetaClass>
struct class_layout;
#endif

template <>
struct required_param_of<class_layout>
{
	typedef mirror::meta_class_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning all member variables (attributes) of a class
/** This meta-function returns a MetaMemberVariable range containing
 *  all registered member variables (attributes) of the reflected class
 *  including those inherited from base classes
 *
 *  @see MetaClass
 *  @see MetaMemberVariable
 *  @see member_variables
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct all_member_variables
{
	/// A mp::range of MetaMemberVariables reflecting all attributes
	typedef Range<MetaMemberVariable> type;
};
#else
template <class MetaClass>
struct all_member_variables;
#endif

template <>
struct required_param_of<all_member_variables>
{
	typedef mirror::meta_class_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the constructors of a class or type
/** This meta-function returns a range of MetaConstructors containing
 *  all registered constructors of the reflected class.
 *
 *  @see MetaClass
 *  @see MetaConstructor
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct constructors
{
	/// A mp::range of MetaConstructors reflecting the constructors
	typedef Range<MetaConstructor> type;
};
#else
template <class MetaClass>
struct constructors;
#endif

template <>
struct required_param_of<constructors>
{
	typedef mirror::meta_class_tag type;
};

// Forward declaration of the meta_constructor template
template <class Class, int Index>
struct meta_constructor;

template <class Class, int Index>
struct meta_object_category< meta_constructor<Class, Index> >
{
	typedef meta_constructor_tag type;
};

// Forward declaration of the meta_constructor_param template
template <typename Type, int ConstrIndex, int ParamIndex>
struct meta_constructor_param;

template <typename Type, int ConstrIndex, int ParamIndex>
struct meta_object_category<
	meta_constructor_param<Type, ConstrIndex, ParamIndex>
>
{
	typedef meta_parameter_tag type;
};

template <class Class>
struct meta_initializer;

template <class Class>
struct meta_object_category< meta_initializer<Class> >
{
	typedef meta_constructor_tag type;
};

// Forward declaration of the meta_constructor_param template
template <typename Type>
struct meta_initializer_param;

template <typename Type>
struct meta_object_category<meta_initializer_param<Type> >
{
	typedef meta_parameter_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the member functions of a class
/** This meta-function returns a range of MetaMemberFunctions
 *  containing all registered member function of the reflected class.
 *
 *  @see MetaClass
 *  @see MetaMemberFunction
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct member_functions
{
	/// A mp::range of MetaMemberFunctions reflecting the functions
	typedef Range<MetaMemberFunction> type;
};
#else
template <class MetaClass>
struct member_functions;
#endif

template <>
struct required_param_of<member_functions>
{
	typedef mirror::meta_class_tag type;
};

template <class Class, int FuncIndex>
struct meta_ovlded_mem_funcs;

//  Specialization of meta_object_category for meta_ovlded_mem_funcs'
template <typename Class, int FuncIndex>
struct meta_object_category<meta_ovlded_mem_funcs<Class, FuncIndex> >
{
	typedef meta_overloaded_functions_tag type;
};

// Forward declaration of the meta_member_function template
template <class Class, int FuncIndex, int OvldIndex>
struct meta_member_function;

//  Specialization of meta_object_category for meta_member_functions
template <class Class, int FuncIndex, int OvldIndex>
struct meta_object_category< meta_member_function<Class, FuncIndex, OvldIndex> >
{
	typedef meta_member_function_tag type;
};

// Forward declaration of the meta_mem_function_param template
template <typename Type, int FnIndex, int OvldIndex, int ParamIndex>
struct meta_mem_function_param;

//  Specialization of meta_object_category for meta_inheritances
template <typename Type, int FnIndex, int OvldIndex, int ParamIndex>
struct meta_object_category<
	meta_mem_function_param<Type, FnIndex, OvldIndex, ParamIndex>
>
{
	typedef meta_parameter_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the conversion operators of a class
/** This meta-function returns a range of MetaConversionOperators
 *  containing all registered conversion operators of the reflected class.
 *
 *  @see MetaClass
 *  @see MetaConversionOperator
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct conversions
{
	/// A mp::range of MetaMemberFunctions reflecting the functions
	typedef Range<MetaMemberFunction> type;
};
#else
template <class MetaClass>
struct conversions;
#endif

template <>
struct required_param_of<conversions>
{
	typedef mirror::meta_class_tag type;
};

// Forward declaration of the meta_conversion_operator template
template <class Class, class Result>
struct meta_conversion_operator;

// Specialization of meta_object_category for meta_conversion_operators
template <class Class, class Result>
struct meta_object_category< meta_conversion_operator<Class, Result> >
{
	typedef meta_conversion_operator_tag type;
};


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the parameters of a function
/** This meta-function returns a MetaVariable range containing
 *  all parameters of a function
 *  @see MetaFunction
 *  @see MetaVariable
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaFunction>
struct parameters
{
	/// A mp::range of MetaParameters reflecting the parameters
	typedef Range<MetaParameter> type;
};
#else
template <class MetaFunction>
struct parameters;
#endif

template <>
struct required_param_of<parameters>
{
	typedef mirror::meta_class_tag type;
};


// Forward declaration of the meta_enum template
template <class Enum>
struct meta_enum;

//  Namespace containing basic meta-data about registered enum classes
/*  Every enum registered with Mirror has a separate specialization
 *  of a helper structure containing the basic meta-data about
 *  the registered enumeration
 */
namespace _enum {

// Specialization of this structure provide basic meta-data about enumerations
template <typename Enum>
struct _;

} // namespace _enum

//  Specialization of meta_object_category for meta_enums
template <class Enum>
struct meta_object_category< meta_enum<Enum> >
{
	typedef meta_enum_tag type;
};

// Forward declaration of the meta_enum_value template
template <class Enum, int Index>
struct meta_enum_value;

//  Specialization of meta_object_category for meta_enums
template <class Enum, int Index>
struct meta_object_category< meta_enum_value<Enum, Index> >
{
	typedef meta_enum_value_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the values of an enumeration
/** This meta-function returns a MetaEnumValue range containing
 *  all values of an enum
 *  @see MetaEnum
 *  @see MetaEnumValue
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaEnum>
struct enum_values
{
	/// A mp::range of MetaEnumValues reflecting the values
	typedef Range<MetaEnumValue> type;
};
#else
template <class MetaEnum>
struct enum_values;
#endif

template <>
struct required_param_of<enum_values>
{
	typedef mirror::meta_enum_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the meta-type reflecting a type of a variable
/** This meta-function returns a MetaType that reflects the type of a variable
 *  reflected by @c MetaVariable.
 *
 *  @see MetaVariable
 *  @see MetaType
 *  @see required_param_of
 */
template <class MetaVariable>
struct type_of
{
	/// A MetaType reflecting the type of a variable
	typedef MetaType type;
};
#else
template <class MetaVariable>
struct type_of;
#endif

template <>
struct required_param_of<type_of>
{
	typedef mirror::meta_variable_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the meta-type reflecting a result type of a function
/** This meta-function returns a MetaType that reflects the result of a function
 *  reflected by @c MetaFunction
 *
 *  @see MetaFunction
 *  @see MetaType
 *  @see required_param_of
 */
template <class MetaFunction>
struct result_type
{
	/// A MetaType reflecting the result type of a function
	typedef MetaType type;
};
#else
template <class MetaFunction>
struct result_type;
#endif

template <>
struct required_param_of<result_type>
{
	typedef mirror::meta_function_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the count of values in an enumerated type
/** This meta-function returns an IntegralConstantType that indicates the number
 *  of values in an @c MetaEnum.
 *
 *  @see MetaEnum
 *  @see required_param_of
 */
template <class MetaEnum>
struct enum_size
{
	/// The count of elements in an enumeration
	typedef IntegralConstantType type;
};
#else
template <class MetaEnum>
struct enum_size;
#endif

template <>
struct required_param_of<enum_size>
{
	typedef mirror::meta_enum_tag type;
};

template <class Container, class IsSpecial, int Index>
struct meta_container;

template <class Container, class IsSpecial, int Index>
struct meta_object_category<meta_container<Container, IsSpecial, Index> >
{
	typedef meta_container_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the containers of a class
/** This meta-function returns a MetaContainer range of
 *  the registered containers of the reflected class.
 *
 *  @see MetaClass
 *  @see MetaContainer
 *  #see all_containers
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaType>
struct containers
{
	/// A range of MetaContainers reflecting the containers
	typedef Range<MetaContainer> type;
};
#else
template <class MetaType>
struct containers;
#endif

template <>
struct required_param_of<containers>
{
	typedef mirror::meta_type_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning all containers of a class including inherited ones
/** This meta-function returns a MetaContainer range of
 *  all registered containers of the reflected class including
 *  those inherited from base classes.
 *
 *  @see MetaClass
 *  @see MetaContainer
 *  #see containers
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaClass>
struct all_containers
{
	/// A range of MetaContainers reflecting the containers
	typedef Range<MetaContainer> type;
};
#else
template <class MetaClass>
struct all_containers;
#endif

template <>
struct required_param_of<all_containers>
{
	typedef mirror::meta_class_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the default traversal method of a container
/** This meta-function returns an MetaTraversal providing meta-data
 *  about a traversal method of the reflected container.
 *
 *  @see MetaContainer
 *  @see MetaTraversal
 *  @see traversals
 *  @see required_param_of
 */
template <class MetaContainer>
struct default_traversal
{
	/// An MetaTraversal providing meta-data about a traversal
	typedef MetaTraversal type;
};
#else
template <class MetaContainer>
struct default_traversal;
#endif

template <>
struct required_param_of<default_traversal>
{
	typedef mirror::meta_container_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the traversal methods of a container
/** This meta-function returns a range of MetaTraversals providing meta-data
 *  about all traversal methods of the reflected container.
 *
 *  @see MetaContainer
 *  @see MetaTraversal
 *  @see default_traversal
 *  @see required_param_of
 */
template <class MetaContainer>
struct traversals
{
	/// A range of MetaTraversals providing meta-data about traversals
	typedef Range<MetaTraversal> type;
};
#else
template <class MetaContainer>
struct traversals;
#endif

template <>
struct required_param_of<traversals>
{
	typedef mirror::meta_container_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the element location methods of a container
/** This meta-function returns a range of MetaLocators providing meta-data
 *  about all element location methods of the reflected container.
 *
 *  @see MetaContainer
 *  @see MetaLocator
 *  @see required_param_of
 */
template <class MetaContainer>
struct locators
{
	/// A range of MetaLocators providing meta-data about element locators
	typedef Range<MetaLocator> type;
};
#else
template <class MetaContainer>
struct locators;
#endif

template <>
struct required_param_of<locators>
{
	typedef mirror::meta_container_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the insertion methods of a container
/** This meta-function returns a range of MetaInserters providing meta-data
 *  about all element insertion methods of the reflected container.
 *
 *  @see MetaContainer
 *  @see MetaInserter
 *  @see default_traversal
 *  @see required_param_of
 */
template <class MetaContainer>
struct inserters
{
	/// A range of MetaInserters providing meta-data about insertion methods
	typedef Range<MetaInserter> type;
};
#else
template <class MetaContainer>
struct inserters;
#endif

template <>
struct required_param_of<inserters>
{
	typedef mirror::meta_container_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the removal methods of a container
/** This meta-function returns a range of MetaErasers providing meta-data
 *  about all element removal methods of the reflected container.
 *
 *  @see MetaContainer
 *  @see MetaEraser
 *  @see default_traversal
 *  @see required_param_of
 */
template <class MetaContainer>
struct erasers
{
	/// A range of MetaErasers providing meta-data about removal methods
	typedef Range<MetaEraser> type;
};
#else
template <class MetaContainer>
struct erasers;
#endif

template <>
struct required_param_of<erasers>
{
	typedef mirror::meta_container_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the generalizations of a MetaObject
/** This meta-function returns a range of MetaErasers providing meta-data
 *  about all element removal methods of the reflected container.
 *
 *  @see MetaMetaObject
 *  @see required_param_of
 */
template <class MetaMetaObject>
struct generalizations
{
	/// A range of MetaMetaObjects reflecting generalizations of a MetaObject
	typedef Range<MetaEraser> type;
};
#else
template <class MetaMetaObject>
struct generalizations;
#endif

template <>
struct required_param_of<generalizations>
{
	typedef mirror::meta_meta_object_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function returning the tags of a meta-object
/** This meta-function returns a range of Tags which
 *  have been assigned to the @a MetaObject passed
 *  as argument
 *
 *  @see mp::range
 *  @see required_param_of
 */
template <class MetaObject>
struct tags
{
	/// A mp::range of Tags assigned to the MetaObject
	typedef Range<Tag> type;
};
#else
template <class MetaObject>
struct tags;
#endif

template <>
struct required_param_of<tags>
{
	typedef mirror::meta_object_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Meta-function reflecting the type passed as template parameter
/** This meta-function template returns a the MetaType, MetaClass
 *  of MetaEnum reflecting the @a Type passed as the template
 *  parameter.
 *  This meta-function is the prefered way of type reflection
 *  and should be used instead of the @c #MIRRORED_TYPE, @c #MIRRORED_CLASS
 *  and @c #MIRRORED_ENUM macros whenever possible.
 *
 *  @see MetaType
 *  @see MetaClass
 *  @see MetaEnum
 *  @see required_param_of
 *
 *  @ingroup mirror_reflection_expressions
 */
template <typename Type>
struct reflected
{
	/// MetaType reflecting the Type
	typedef MetaType type;
};
#else
template <typename Type>
struct reflected;
#endif

template <>
struct required_param_of<reflected>
{
	typedef mirror::non_meta_object_tag type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Template, instantiations of which conform to the MetaType concept
/** Instantiations of this template conform to the MetaType, MetaClass
 *  of MetaEnum reflecting the @a Type passed as the template
 *  parameter.
 *  This template is only 'syntactic sugar' and has several limitations;
 *  The @c mirror::reflected should be used in most situations instead.
 *  Most notably the @c categorize_meta_object and the @c meta_object_category
 *  do not work properly for this template, but it can be converted to
 *  a proper MetaType model via the mp::as_a meta-function if necessary.
 *  It can also be safely used as a MetaType expression:
 *  @code
 *  cout << mp::size<member_variables<reflected_type<T> > >::value << endl;
 *  @endcode
 *
 *  @see mirror::reflected
 *  @see MetaType
 *  @see MetaClass
 *  @see MetaEnum
 *
 *  @ingroup mirror_reflection_expressions
 */
template <typename Type>
struct reflected_type
{ };
#else
template <typename Type>
struct reflected_type;
#endif

MIRROR_NAMESPACE_END

#endif //include guard

