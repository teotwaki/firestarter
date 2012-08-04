/**
 * @file mirror/meta_object_tags.hpp
 * @brief Declaration of trait/tag types for tag dispatching for
 * Mirror's meta-objects.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_OBJECT_TAGS_1011291729_HPP
#define MIRROR_META_OBJECT_TAGS_1011291729_HPP

#include <mirror/config.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

/** @defgroup meta_object_categories Mirror - Categorization and Tag dispatching
 *
 *  Tag types and categorization template meta-functions for tag dispatching.
 *  These tags,  the categorization functions and template meta-functions
 *  can be used for creating different function overloads and template
 *  specializations for different meta-object categories and for the
 *  dispatching of the proper overload and/or template specialization for
 *  a particular meta-object type, based on its capabilities.
 */


/// Tag used for recognizing types which are not Mirror meta-objects
/** This tag or tag derived from this tag is returned by the
 *  meta_object_category meta-function for types which are not
 *  Mirror's meta-object classes.
 *
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @ingroup meta_object_categories
 */
struct non_meta_object_tag
{ };

/// Tag for types which conform to the Mirror's MetaObject concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaObject
 *  @ingroup meta_object_categories
 */
struct meta_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_object(MACRO, DATA) \
	MACRO(Object, DATA)
#define MIRROR_FOR_EACH_MO_BASE_object(MACRO, DATA, SEP_MACRO)


/// Tag for types conforming to the Mirror's MetaNamedObject concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaNamedObject
 *  @ingroup meta_object_categories
 */
struct meta_named_object_tag
 : virtual public meta_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_named_object(MACRO, DATA) \
	MACRO(NamedObject, DATA)
#define MIRROR_FOR_EACH_MO_BASE_named_object(MACRO, DATA, SEP_MACRO) \
	MACRO(object, DATA)


/// Tag for types conforming to the Mirror's MetaMetaObject concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaMetaObject
 *  @ingroup meta_object_categories
 */
struct meta_meta_object_tag
 : virtual public meta_named_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_meta_object(MACRO, DATA) \
	MACRO(MetaObject, DATA)
#define MIRROR_FOR_EACH_MO_BASE_meta_object(MACRO, DATA, SEP_MACRO) \
	MACRO(named_object, DATA)


/// Tag for types conforming to the Mirror's MetaScopedObject concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaScopedObject
 *  @ingroup meta_object_categories
 */
struct meta_scoped_object_tag
 : virtual public meta_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_scoped_object(MACRO, DATA) \
	MACRO(ScopedObject, DATA)
#define MIRROR_FOR_EACH_MO_BASE_scoped_object(MACRO, DATA, SEP_MACRO) \
	MACRO(object, DATA)


/// Tag for types conforming to the Mirror's MetaNamedScopedObject concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaNamedScopedObject
 *  @ingroup meta_object_categories
 */
struct meta_named_scoped_object_tag
 : virtual public meta_named_object_tag
 , virtual public meta_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_named_scoped_object(MACRO, DATA) \
	MACRO(NamedScopedObject, DATA)
#define MIRROR_FOR_EACH_MO_BASE_named_scoped_object(MACRO, DATA, SEP_MACRO) \
	MACRO(named_object, DATA) \
	SEP_MACRO() \
	MACRO(scoped_object, DATA)


/// Tag for types conforming to the Mirror's MetaScope concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaScope
 *  @ingroup meta_object_categories
 */
struct meta_scope_tag
 : virtual public meta_named_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_scope(MACRO, DATA) \
	MACRO(Scope, DATA)
#define MIRROR_FOR_EACH_MO_BASE_scope(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)


/// Tag for types conforming to the MetaScope concept for an unspecified scope
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaScope
 *  @ingroup meta_object_categories
 */
struct meta_unspecified_scope_tag
 : virtual public meta_scope_tag
{ };

#define MIRROR_META_OBJECT_NAME_unspecified_scope(MACRO, DATA) \
	MACRO(Scope, DATA)
#define MIRROR_FOR_EACH_MO_BASE_unspecified_scope(MACRO, DATA, SEP_MACRO) \
	MACRO(scope, DATA)


/// Tag for types conforming to the Mirror's MetaNamespace concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaNamespace
 *  @ingroup meta_object_categories
 */
struct meta_namespace_tag
 : virtual public meta_scope_tag
{ };

#define MIRROR_META_OBJECT_NAME_namespace(MACRO, DATA) \
	MACRO(Namespace, DATA)
#define MIRROR_FOR_EACH_MO_BASE_namespace(MACRO, DATA, SEP_MACRO) \
	MACRO(scope, DATA)


/// Tag for types conforming to the MetaNamespace concept for the global scope
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaNamespace
 *  @ingroup meta_object_categories
 */
struct meta_global_scope_tag
 : virtual public meta_namespace_tag
{ };

#define MIRROR_META_OBJECT_NAME_global_scope(MACRO, DATA) \
	MACRO(GlobalScope, DATA)
#define MIRROR_FOR_EACH_MO_BASE_global_scope(MACRO, DATA, SEP_MACRO) \
	MACRO(namespace, DATA)


/// Tag for types conforming to the Mirror's MetaType concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaType
 *  @ingroup meta_object_categories
 */
struct meta_type_tag
 : virtual public meta_named_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_type(MACRO, DATA) \
	MACRO(Type, DATA)
#define MIRROR_FOR_EACH_MO_BASE_type(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)


/// Tag for types conforming to the Mirror's MetaTemplatedType concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaTemplatedType
 *  @ingroup meta_object_categories
 */
struct meta_templated_type_tag
 : virtual public meta_type_tag
{ };

#define MIRROR_META_OBJECT_NAME_templated_type(MACRO, DATA) \
	MACRO(TemplatedType, DATA)
#define MIRROR_FOR_EACH_MO_BASE_templated_type(MACRO, DATA, SEP_MACRO) \
	MACRO(type, DATA)


/// Tag for types conforming to the Mirror's MetaTypeTemplate concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaTypeTemplate
 *  @ingroup meta_object_categories
 */
struct meta_type_template_tag
 : virtual public meta_named_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_type_template(MACRO, DATA) \
	MACRO(TypeTemplate, DATA)
#define MIRROR_FOR_EACH_MO_BASE_type_template(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)


/// Tag for types conforming to the Mirror's MetaTypedef concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaTypedef
 *  @ingroup meta_object_categories
 */
struct meta_typedef_tag
 : virtual public meta_type_tag
{ };

#define MIRROR_META_OBJECT_NAME_typedef(MACRO, DATA) \
	MACRO(Typedef, DATA)
#define MIRROR_FOR_EACH_MO_BASE_typedef(MACRO, DATA, SEP_MACRO) \
	MACRO(type, DATA)


/// Tag for types conforming to the Mirror's MetaClass concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaClass
 *  @ingroup meta_object_categories
 */
struct meta_class_tag
 : virtual public meta_type_tag
 , virtual public meta_scope_tag
{ };

#define MIRROR_META_OBJECT_NAME_class(MACRO, DATA) \
	MACRO(Class, DATA)
#define MIRROR_FOR_EACH_MO_BASE_class(MACRO, DATA, SEP_MACRO) \
	MACRO(type, DATA) \
	SEP_MACRO() \
	MACRO(scope, DATA)


/// Tag for types conforming to the Mirror's MetaClassMember concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaClassMember
 *  @ingroup meta_object_categories
 */
struct meta_class_member_tag
 : virtual public meta_named_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_class_member(MACRO, DATA) \
	MACRO(ClassMember, DATA)
#define MIRROR_FOR_EACH_MO_BASE_class_member(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)


/// Tag for types conforming to the Mirror's MetaTemplatedClass concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaTemplatedClass
 *  @ingroup meta_object_categories
 */
struct meta_templated_class_tag
 : virtual public meta_templated_type_tag
 , virtual public meta_class_tag
{ };

#define MIRROR_META_OBJECT_NAME_templated_class(MACRO, DATA) \
	MACRO(TemplatedClass, DATA)
#define MIRROR_FOR_EACH_MO_BASE_templated_class(MACRO, DATA, SEP_MACRO) \
	MACRO(templated_type, DATA) \
	SEP_MACRO() \
	MACRO(class, DATA)


/// Tag for types conforming to the Mirror's MetaEnum concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaEnum,
 *  @ingroup meta_object_categories
 */
struct meta_enum_tag
 : virtual public meta_type_tag
 , virtual public meta_scope_tag
{ };

#define MIRROR_META_OBJECT_NAME_enum(MACRO, DATA) \
	MACRO(Enum, DATA)
#define MIRROR_FOR_EACH_MO_BASE_enum(MACRO, DATA, SEP_MACRO) \
	MACRO(type, DATA) \
	SEP_MACRO() \
	MACRO(scope, DATA)


/// Tag for types conforming to the Mirror's MetaEnumValue concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaEnumValue,
 *  @ingroup meta_object_categories
 */
struct meta_enum_value_tag
 : virtual public meta_named_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_enum_value(MACRO, DATA) \
	MACRO(EnumValue, DATA)
#define MIRROR_FOR_EACH_MO_BASE_enum_value(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)


/// Tag for types conforming to the Mirror's MetaInheritance concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaInheritance
 *  @ingroup meta_object_categories
 */
struct meta_inheritance_tag
 : virtual public meta_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_inheritance(MACRO, DATA) \
	MACRO(Inheritance, DATA)
#define MIRROR_FOR_EACH_MO_BASE_inheritance(MACRO, DATA, SEP_MACRO) \
	MACRO(object, DATA)


/// Tag for types conforming to the Mirror's MetaVariable concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaVariable
 *  @ingroup meta_object_categories
 */
struct meta_variable_tag
 : virtual public meta_named_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_variable(MACRO, DATA) \
	MACRO(Variable, DATA)
#define MIRROR_FOR_EACH_MO_BASE_variable(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)

/// Tag for types conforming to the Mirror's MetaPlainVariable concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaPlainVariable
 *  @ingroup meta_object_categories
 */
struct meta_plain_variable_tag
 : virtual public meta_variable_tag
{ };

#define MIRROR_META_OBJECT_NAME_plain_variable(MACRO, DATA) \
	MACRO(PlainVariable, DATA)
#define MIRROR_FOR_EACH_MO_BASE_plain_variable(MACRO, DATA, SEP_MACRO) \
	MACRO(variable, DATA)

/// Tag for types conforming to the Mirror's MetaFreeVariable concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaFreeVariable
 *  @ingroup meta_object_categories
 */
struct meta_free_variable_tag
 : virtual public meta_variable_tag
{ };

#define MIRROR_META_OBJECT_NAME_free_variable(MACRO, DATA) \
	MACRO(FreeVariable, DATA)
#define MIRROR_FOR_EACH_MO_BASE_free_variable(MACRO, DATA, SEP_MACRO) \
	MACRO(variable, DATA)


/// Tag for types conforming to the Mirror's MetaPlainFreeVariable concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaPlainFreeVariable
 *  @ingroup meta_object_categories
 */
struct meta_plain_free_variable_tag
 : virtual public meta_free_variable_tag
 , virtual public meta_plain_variable_tag
{ };

#define MIRROR_META_OBJECT_NAME_plain_free_variable(MACRO, DATA) \
	MACRO(PlainFreeVariable, DATA)
#define MIRROR_FOR_EACH_MO_BASE_plain_free_variable(MACRO, DATA, SEP_MACRO) \
	MACRO(free_variable, DATA) \
	SEP_MACRO() \
	MACRO(plain_variable, DATA)


/// Tag for types conforming to the Mirror's MetaMemberVariable concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaMemberVariable
 *  @ingroup meta_object_categories
 */
struct meta_member_variable_tag
 : virtual public meta_variable_tag
 , virtual public meta_class_member_tag
{ };

#define MIRROR_META_OBJECT_NAME_member_variable(MACRO, DATA) \
	MACRO(MemberVariable, DATA)
#define MIRROR_FOR_EACH_MO_BASE_member_variable(MACRO, DATA, SEP_MACRO) \
	MACRO(variable, DATA) \
	SEP_MACRO() \
	MACRO(class_member, DATA)


/// Tag for types conforming to the Mirror's MetaPlainMemberVariable concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaPlainMemberVariable
 *  @ingroup meta_object_categories
 */
struct meta_plain_member_variable_tag
 : virtual public meta_member_variable_tag
 , virtual public meta_plain_variable_tag
{ };

#define MIRROR_META_OBJECT_NAME_plain_member_variable(MACRO, DATA) \
	MACRO(PlainMemberVariable, DATA)
#define MIRROR_FOR_EACH_MO_BASE_plain_member_variable(MACRO, DATA, SEP_MACRO) \
	MACRO(member_variable, DATA) \
	SEP_MACRO() \
	MACRO(plain_variable, DATA)


/// Tag for types conforming to the Mirror's MetaParameter concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaParameter
 *  @ingroup meta_object_categories
 */
struct meta_parameter_tag
 : virtual public meta_variable_tag
{ };

#define MIRROR_META_OBJECT_NAME_parameter(MACRO, DATA) \
	MACRO(Parameter, DATA)
#define MIRROR_FOR_EACH_MO_BASE_parameter(MACRO, DATA, SEP_MACRO) \
	MACRO(variable, DATA)


/// Tag for types conforming to the Mirror's MetaFunction concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaFunction
 *  @ingroup meta_object_categories
 */
struct meta_function_tag
 : virtual public meta_scope_tag
{ };

#define MIRROR_META_OBJECT_NAME_function(MACRO, DATA) \
	MACRO(Function, DATA)
#define MIRROR_FOR_EACH_MO_BASE_function(MACRO, DATA, SEP_MACRO) \
	MACRO(scope, DATA)


/// Tag for types conforming to the Mirror's MetaMemberFunction concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaMemberFunction
 *  @ingroup meta_object_categories
 */
struct meta_member_function_tag
 : virtual public meta_function_tag
 , virtual public meta_class_member_tag
{ };

#define MIRROR_META_OBJECT_NAME_member_function(MACRO, DATA) \
	MACRO(MemberFunction, DATA)
#define MIRROR_FOR_EACH_MO_BASE_member_function(MACRO, DATA, SEP_MACRO) \
	MACRO(function, DATA) \
	SEP_MACRO() \
	MACRO(class_member, DATA)


/// Tag for types conforming to the Mirror's MetaConstructor concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaConstructor
 *  @ingroup meta_object_categories
 */
struct meta_constructor_tag
 : virtual public meta_function_tag
 , virtual public meta_class_member_tag
{ };

#define MIRROR_META_OBJECT_NAME_constructor(MACRO, DATA) \
	MACRO(Constructor, DATA)
#define MIRROR_FOR_EACH_MO_BASE_constructor(MACRO, DATA, SEP_MACRO) \
	MACRO(function, DATA) \
	SEP_MACRO() \
	MACRO(class_member, DATA)


/// Tag for types conforming to the Mirror's MetaConversionOperator concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaConversionOperator
 *  @ingroup meta_object_categories
 */
struct meta_conversion_operator_tag
 : virtual public meta_member_function_tag
{ };

#define MIRROR_META_OBJECT_NAME_conversion_operator(MACRO, DATA) \
	MACRO(ConversionOperator, DATA)
#define MIRROR_FOR_EACH_MO_BASE_conversion_operator(MACRO, DATA, SEP_MACRO) \
	MACRO(member_function, DATA)


/// Tag for types conforming to the Mirror's MetaOverloadedFunctions concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaOverloadedFunctions
 *  @ingroup meta_object_categories
 */
struct meta_overloaded_functions_tag
 : virtual public meta_named_scoped_object_tag
{ };


#define MIRROR_META_OBJECT_NAME_overloaded_functions(MACRO, DATA) \
	MACRO(OverloadedFunctions, DATA)
#define MIRROR_FOR_EACH_MO_BASE_overloaded_functions(MACRO, DATA, SEP_MACRO) \
	MACRO(named_scoped_object, DATA)

/// Tag for types conforming to the Mirror's MetaLocator concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaLocator
 *  @ingroup meta_object_categories
 */
struct meta_locator_tag
 : virtual public meta_named_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_locator(MACRO, DATA) \
	MACRO(Locator, DATA)
#define MIRROR_FOR_EACH_MO_BASE_locator(MACRO, DATA, SEP_MACRO) \
	MACRO(named_object, DATA)


/// Tag for types conforming to the Mirror's MetaTraversal concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaTraversal
 *  @ingroup meta_object_categories
 */
struct meta_traversal_tag
 : virtual public meta_named_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_traversal(MACRO, DATA) \
	MACRO(Traversal, DATA)
#define MIRROR_FOR_EACH_MO_BASE_traversal(MACRO, DATA, SEP_MACRO) \
	MACRO(named_object, DATA)


/// Tag for types conforming to the Mirror's MetaInserter concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaInserter
 *  @ingroup meta_object_categories
 */
struct meta_inserter_tag
 : virtual public meta_named_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_inserter(MACRO, DATA) \
	MACRO(Inserter, DATA)
#define MIRROR_FOR_EACH_MO_BASE_inserter(MACRO, DATA, SEP_MACRO) \
	MACRO(named_object, DATA)


/// Tag for types conforming to the Mirror's MetaEraser concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaEraser
 *  @ingroup meta_object_categories
 */
struct meta_eraser_tag
 : virtual public meta_named_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_eraser(MACRO, DATA) \
	MACRO(Eraser, DATA)
#define MIRROR_FOR_EACH_MO_BASE_eraser(MACRO, DATA, SEP_MACRO) \
	MACRO(named_object, DATA)


/// Tag for types conforming to the Mirror's MetaContainer concept
/**
 *  @see meta_object_category
 *  @see categorize_meta_object
 *  @see MetaContainer
 *  @ingroup meta_object_categories
 */
struct meta_container_tag
 : virtual public meta_scoped_object_tag
{ };

#define MIRROR_META_OBJECT_NAME_container(MACRO, DATA) \
	MACRO(Container, DATA)
#define MIRROR_FOR_EACH_MO_BASE_container(MACRO, DATA, SEP_MACRO) \
	MACRO(scoped_object, DATA)



namespace aux {

template <class T, class Bool>
struct meta_obj_cat;

template <class T>
struct meta_obj_cat<T, std::true_type>
{
	typedef typename T::category type;
};

template <class T>
struct meta_obj_cat<T, std::false_type>
{
	typedef mirror::non_meta_object_tag type;
};

template <typename T>
std::true_type meta_obj_has_cat(T*, typename T::category* = nullptr);

std::false_type meta_obj_has_cat(...);

} // namespace aux

/// Template meta-function for getting the category tag of a type
/** This template meta-function can be used to get the exact
 *  category tag for a (meta-object) type.
 *
 *  @tparam T the type to be examined for Mirror category tag
 *  @see categorize_meta_object
 *  @see meta_object_kind
 *  @see non_meta_object_tag
 *  @see meta_object_tag
 *  @see meta_named_object_tag
 *  @see meta_scoped_object_tag
 *  @see meta_named_scoped_object_tag
 *  @see meta_scope_tag
 *  @see meta_namespace_tag
 *  @see meta_global_scope_tag
 *  @see meta_type_tag
 *  @see meta_class_tag
 *  @ingroup meta_object_categories
 */
template <class T>
struct meta_object_category
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	/// The category tag for the type passed as template argument
	typedef CategoryTag type;
#else
	typedef typename aux::meta_obj_cat<
		T,
		decltype(aux::meta_obj_has_cat((T*)nullptr))
	>::type type;
#endif
};

namespace aux {

template <typename TestedCat, typename TargetCat>
struct meta_obj_kind_same
 : std::integral_constant<
	bool,
	std::is_same<TargetCat, TestedCat>::value ||
	std::is_base_of<TargetCat, TestedCat>::value
> { };

template <typename TestedCat, typename ... Categories>
struct meta_obj_kind_hlpr;

template <typename TestedCat>
struct meta_obj_kind_hlpr<TestedCat>
{
	typedef TestedCat type;

};

template <
	typename Bool,
	typename TestedCat,
	typename MismatchCat,
	typename ... Categories
> struct meta_obj_kind_hlpr2
 : meta_obj_kind_hlpr<TestedCat, Categories...>
{ };

template <typename TestedCat, typename MatchCat, typename ... Categories>
struct meta_obj_kind_hlpr2<std::true_type, TestedCat, MatchCat, Categories...>
{
	typedef MatchCat type;
};

template <typename TestedCat, typename Category, typename ... Categories>
struct meta_obj_kind_hlpr<TestedCat, Category, Categories...>
 : meta_obj_kind_hlpr2<
	typename meta_obj_kind_same<TestedCat, Category>::type,
	TestedCat,
	Category,
	Categories...
> { };

} // namespace aux

/// Template meta-function for getting a category tag of a type
/** This template meta-function can be used to get the possibly up-cast
 *  category tag for a (meta-object) type. The difference between this template
 *  and meta_object_category is, that meta_object_category returns the exact
 *  meta-object-tag type for a particular meta-object. Sometimes it is however
 *  desirable to upcast the tag to a more general meta-object-tag.
 *
 *  This template allows to specify a set of meta-object-tags to one of which
 *  the real meta-object tag is up-cast if possible. This is useful for example
 *  if there is a template overload for MetaType but we also want it to handle
 *  MetaClass, MetaTemplatedType, or other derived meta-objects.
 *
 *  @tparam T the type to be examined for Mirror category tag
 *  @tparam CategoryTags the pack of category tags to up-cast to if the real
 *   category tag is derived from one of those listed in this pack.
 *
 *  @see categorize_meta_object
 *  @see meta_object_category
 *  @ingroup meta_object_categories
 */
template <class T, class ... CategoryTags>
struct meta_object_kind
#ifndef MIRROR_DOCUMENTATION_ONLY
 : aux::meta_obj_kind_hlpr<
	typename mirror::meta_object_category<T>::type,
	CategoryTags...
>
#endif
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	typedef CategoryTag type;
#endif
};

/// This function returns the meta-object category tag for an instance
/**
 *  @tparam T the type to be categorized
 *  @param inst the instance type of which is to be categorized
 *  @see meta_object_category
 *  @ingroup meta_object_categories
 */
template <class T>
typename meta_object_category<T>::type
categorize_meta_object(const T& inst)
{
	return typename meta_object_category<T>::type();
}

#define MIRROR_FOR_EACH_META_OBJECT(MACRO, DATA) \
	MACRO(object, 0, DATA) \
	MACRO(named_object, 1, DATA) \
	MACRO(scoped_object, 2, DATA) \
	MACRO(named_scoped_object, 3, DATA) \
	MACRO(scope, 4, DATA) \
	MACRO(unspecified_scope, 5, DATA) \
	MACRO(namespace, 6, DATA) \
	MACRO(global_scope, 7, DATA) \
	MACRO(type, 8, DATA) \
	MACRO(templated_type, 9, DATA) \
	MACRO(type_template, 10, DATA) \
	MACRO(typedef, 11, DATA) \
	MACRO(class, 12, DATA) \
	MACRO(templated_class, 13, DATA) \
	MACRO(class_member, 14, DATA) \
	MACRO(enum, 15, DATA) \
	MACRO(enum_value, 16, DATA) \
	MACRO(inheritance, 17, DATA) \
	MACRO(variable, 18, DATA) \
	MACRO(plain_variable, 19, DATA) \
	MACRO(free_variable, 20, DATA) \
	MACRO(plain_free_variable, 21, DATA) \
	MACRO(member_variable, 22, DATA) \
	MACRO(plain_member_variable, 23, DATA) \
	MACRO(parameter, 24, DATA) \
	MACRO(function, 25, DATA) \
	MACRO(member_function, 26, DATA) \
	MACRO(constructor, 27, DATA) \
	MACRO(conversion_operator, 28, DATA) \
	MACRO(overloaded_functions, 29, DATA) \
	MACRO(traversal, 30, DATA) \
	MACRO(locator, 31, DATA) \
	MACRO(inserter, 32, DATA) \
	MACRO(eraser, 33, DATA) \
	MACRO(container, 34, DATA)

#define MIRROR_META_OBJECT_NICE_NAME(OBJECT, MACRO, DATA) \
	MIRROR_META_OBJECT_NAME_##OBJECT(MACRO, DATA)

#define MIRROR_HLPR_MO_NICE_NAME_STR(OBJECT, X) \
	#OBJECT

#define MIRROR_META_OBJECT_NICE_NAME_STR(OBJECT) \
	MIRROR_META_OBJECT_NICE_NAME(OBJECT, MIRROR_HLPR_MO_NICE_NAME_STR, _)

#define MIRROR_FOR_EACH_META_OBJECT_BASE(OBJECT, MACRO, DATA, SEP_MACRO) \
	MIRROR_FOR_EACH_MO_BASE_##OBJECT(MACRO, DATA, SEP_MACRO)

template <typename Expr>
struct eval_meta_object_tag
{
	typedef typename eval_meta_object_tag<
		typename Expr::type
	>::type type;
};

#define MIRROR_HLPR_IMPLEMENT_EVAL_MO_TAG(OBJECT, I, X) \
template <> \
struct eval_meta_object_tag< mirror::meta_ ## OBJECT ## _tag > \
{ \
	typedef mirror::meta_ ## OBJECT ## _tag type; \
};

MIRROR_FOR_EACH_META_OBJECT(MIRROR_HLPR_IMPLEMENT_EVAL_MO_TAG, _)

#undef MIRROR_HLPR_IMPLEMENT_EVAL_MO_TAG

MIRROR_NAMESPACE_END

#endif //include guard

