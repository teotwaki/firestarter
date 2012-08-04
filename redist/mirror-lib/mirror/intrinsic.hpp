/**
 * @file mirror/intrinsic.hpp
 * @brief Implementation of several intrinsic meta-functions
 * for various meta-object concepts
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_INTRINSIC_1011291729_HPP
#define MIRROR_INTRINSIC_1011291729_HPP

#include <mirror/meta_prog/is_a.hpp>
#include <mirror/meta_prog/as_a.hpp>
#include <mirror/meta_object_tags.hpp>

MIRROR_NAMESPACE_BEGIN

#ifndef MIRROR_DOCUMENTATION_ONLY

template <class MetaTypeExpr>
struct original_type
{
	// try to evaluate the expression as a MetaType
	// and get its original_type
	typedef typename mp::as_a<
		MetaTypeExpr,
		meta_type_tag
	>::type::original_type type;
};

template <class MetaNamedObjectExpr>
struct static_name
{
	typedef typename mp::as_a<
		MetaNamedObjectExpr,
		meta_named_object_tag
	>::type::static_name type;
};

template <class MetaScopedObjectExpr>
struct scope
{
	typedef typename mp::as_a<
		MetaScopedObjectExpr,
		meta_scoped_object_tag
	>::type::scope type;
};

template <class MetaTypeExpr>
struct container_kind
{
	typedef typename mp::as_a<
		MetaTypeExpr,
		meta_type_tag
	>::type::container_kind type;
};

template <class MetaClassExpr>
struct elaborated_type
{
	typedef typename mp::as_a<
		MetaClassExpr,
		meta_class_tag
	>::type::elaborated_type type;
};

template <class MetaVariableExpr>
struct storage_class
{
	typedef typename mp::as_a<
		MetaVariableExpr,
		meta_variable_tag
	>::type::storage_class type;
};

template <class MetaFunctionExpr>
struct linkage
{
	typedef typename mp::as_a<
		MetaFunctionExpr,
		meta_function_tag
	>::type::linkage type;
};

template <class MetaMemberFunctionExpr>
struct constness
{
	typedef typename mp::as_a<
		MetaMemberFunctionExpr,
		meta_member_function_tag
	>::type::constness type;
};

template <class MetaInheritanceExpr>
struct inheritance_type
{
	typedef typename mp::as_a<
		MetaInheritanceExpr,
		meta_inheritance_tag
	>::type::inheritance_type type;
};

template <class MetaInheritanceExpr>
struct derived_class
{
	typedef typename mp::as_a<
		MetaInheritanceExpr,
		meta_inheritance_tag
	>::type::derived_class type;
};

template <class MetaInheritanceExpr>
struct base_class
{
	typedef typename mp::as_a<
		MetaInheritanceExpr,
		meta_inheritance_tag
	>::type::base_class type;
};

template <class MetaInheritanceExpr>
struct base_class_access_type
{
	typedef typename mp::as_a<
		MetaInheritanceExpr,
		meta_inheritance_tag
	>::type::access_type type;
};

template <class MetaClassMemberExpr>
struct class_member_access_type
{
	typedef typename mp::as_a<
		MetaClassMemberExpr,
		meta_class_member_tag
	>::type::access_type type;
};

template <class MetaParameterExpr>
struct parameter_position
{
	typedef typename mp::as_a<
		MetaParameterExpr,
		meta_parameter_tag
	>::type::position type;
};

template <class MetaTemplatedTypeExpr>
struct type_template
{
	typedef typename mp::as_a<
		MetaTemplatedTypeExpr,
		meta_templated_type_tag
	>::type::type_template type;
};

template <class MetaVariableExpr>
struct type_of
{
	typedef typename mp::as_a<
		MetaVariableExpr,
		meta_variable_tag
	>::type::type type;
};

template <class MetaEnumExpr>
struct enum_size
{
	typedef typename mp::as_a<
		MetaEnumExpr,
		meta_enum_tag
	>::type::size type;
};

template <class X>
struct members
{
	MIRROR_ASSERT_RETURNS_META(scope, X);
	typedef typename members<typename X::type>::type type;
};

template <class X>
struct template_parameters
{
	MIRROR_ASSERT_RETURNS_META(templated_type, X);
	typedef typename template_parameters<typename X::type>::type type;
};

template <class X>
struct base_classes
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename base_classes<typename X::type>::type type;
};

template <class X>
struct member_variables
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename member_variables<typename X::type>::type type;
};

template <class X>
struct all_member_variables
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename all_member_variables<typename X::type>::type type;
};

template <class X>
struct class_layout
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename class_layout<typename X::type>::type type;
};

template <class X>
struct constructors
{
	MIRROR_ASSERT_RETURNS_META(type, X);
	typedef typename constructors<typename X::type>::type type;
};

template <class X>
struct overloads
{
	MIRROR_ASSERT_RETURNS_META(overloaded_functions, X);
	typedef typename overloads<typename X::type>::type type;
};

template <class X>
struct member_functions
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename member_functions<typename X::type>::type type;
};

template <class X>
struct conversions
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename conversions<typename X::type>::type type;
};

template <class X>
struct parameters
{
	MIRROR_ASSERT_RETURNS_META(function, X);
	typedef typename parameters<typename X::type>::type type;
};

template <class X>
struct enum_values
{
	MIRROR_ASSERT_RETURNS_META(enum, X);
	typedef typename enum_values<typename X::type>::type type;
};

template <class X>
struct namespaces
{
	MIRROR_ASSERT_RETURNS_META(namespace, X);
	typedef typename namespaces<typename X::type>::type type;
};

template <class X>
struct free_variables
{
	MIRROR_ASSERT_RETURNS_META(namespace, X);
	typedef typename free_variables<typename X::type>::type type;
};

template <class X>
struct containers
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename containers<typename X::type>::type type;
};

template <class X>
struct all_containers
{
	MIRROR_ASSERT_RETURNS_META(class, X);
	typedef typename all_containers<typename X::type>::type type;
};

template <class X>
struct default_traversal
{
	MIRROR_ASSERT_RETURNS_META(container, X);
	typedef typename default_traversal<typename X::type>::type type;
};

template <class X>
struct traversals
{
	MIRROR_ASSERT_RETURNS_META(container, X);
	typedef typename traversals<typename X::type>::type type;
};

template <class X>
struct locators
{
	MIRROR_ASSERT_RETURNS_META(container, X);
	typedef typename locators<typename X::type>::type type;
};

template <class X>
struct inserters
{
	MIRROR_ASSERT_RETURNS_META(container, X);
	typedef typename inserters<typename X::type>::type type;
};

template <class X>
struct erasers
{
	MIRROR_ASSERT_RETURNS_META(container, X);
	typedef typename erasers<typename X::type>::type type;
};


template <class X>
struct generalizations
{
	MIRROR_ASSERT_RETURNS_META(meta_object, X);
	typedef typename generalizations<typename X::type>::type type;
};

#endif // MIRROR_DOCUMENTATION_ONLY

MIRROR_NAMESPACE_END

#endif //include guard

