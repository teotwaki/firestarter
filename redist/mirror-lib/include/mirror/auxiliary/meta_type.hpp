/**
 * @file mirror/auxiliary/meta_type.hpp
 * @brief Internal helpers for type registering and reflection
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_META_TYPE_1011291729_HPP
#define MIRROR_AUX_META_TYPE_1011291729_HPP

#include <mirror/specifier_tags.hpp>
#include <mirror/auxiliary/template_type_name.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Helper class used for selecting the implementation of MetaScopedNamedObject
template <class Type, class Tag>
struct meta_type_name_impl;

template <typename Type>
struct meta_template_of;

template <template <typename...> class Template, typename ... Params>
struct meta_template_of<Template<Params...> >
{
	typedef meta_type_template<Template> type;
};

// This is the implementation for types registered as non-template types
template <class Type>
struct meta_type_name_impl<Type, mirror::meta_type_tag>
 : scoped_named_impl< mirror::_type::_<Type> >
{
	//  Helper typedef for categorizing this meta-type
	/*  Do not use this typedef(s) in client code, it is implementation
	 *  detail and is subject to change without any notice.
	 *  Use the meta_object_category meta-function template class
	 *  or the categorize_meta_object free function to find
	 *  out about the category of the meta-type.
	 */
	typedef meta_type_tag _type_category;
	typedef meta_class_tag _class_category;
};

// This is the implementation for types registered as template types
template <class Type>
struct meta_type_name_impl<Type, mirror::meta_templated_type_tag>
 : template_type_name<
	scoped_named_impl<
		mirror::_type::_<Type>
	>,
	Type
>
{
	//  Helper typedef for categorizing this meta-type
	/*  Do not use this typedef(s) in client code, it is implementation
	 *  detail and is subject to change without any notice.
	 *  Use the meta_object_category meta-function template class
	 *  or the categorize_meta_object free function to find
	 *  out about the category of the meta-type.
	 */
	typedef meta_templated_type_tag _type_category;
	typedef meta_templated_class_tag _class_category;

	// The reflection of template instantiated to create the reflected type
	typedef typename meta_template_of<Type>::type type_template;
};


// Helper which duplicates some of the meta_type typedefs
template <typename Type>
struct dup_meta_type
{
	typedef typename meta_type<Type>::_type_category _type_category;
	typedef typename meta_type<Type>::_class_category _class_category;
	typedef typename meta_type<Type>::scope scope;
};

// Helper which defined some of the meta_type typedefs in non-trivial types
template <typename Type>
struct non_trivial_meta_type
{
	typedef meta_type_tag _type_category;
	typedef meta_class_tag _class_category;
	typedef typename meta_type<Type>::scope scope;
};

// Helper that wraps the passed type by the proper template
// meta_type / meta_typedef / meta_class / meta_enum / etc.
template <typename Type, typename Tag>
struct do_reflect_type
{
	typedef mirror::meta_type<Type> type;
};

template <typename Class>
struct do_reflect_type<Class, spec_class_tag>
{
	typedef mirror::meta_class<Class> type;
};

template <typename Class>
struct do_reflect_type<Class, spec_struct_tag>
{
	typedef mirror::meta_class<Class> type;
};

template <typename Enum>
struct do_reflect_type<Enum, spec_enum_tag>
{
	typedef mirror::meta_enum<Enum> type;
};

template <typename X>
struct reflect_type
 : public do_reflect_type<
	X,
	typename mirror::_type::_<X>::kind
>
{ };

template <typename Typedef>
struct reflect_type<mirror::_typedef::_selector<Typedef> >
{
	typedef mirror::meta_type<
		mirror::_typedef::_selector<Typedef>
	> type;
};

// Helper template used in implementation of full_meta_object
template <class BaseMetaObject, class Tag>
struct get_full_meta_object;

// Full meta object getter for namespaces
template <class Namespace>
struct get_full_meta_object<
	Namespace,
	mirror::meta_namespace_tag
>
{
	typedef meta_namespace<Namespace> type;
};

// Full meta object getter for types
template <class Type>
struct get_full_meta_object<
	mirror::_type::_<Type>,
	mirror::meta_type_tag
>
{
	typedef meta_type<Type> type;
};

// Full meta object getter for class
template <class Class>
struct get_full_meta_object<
	mirror::_class::_<Class>,
	mirror::meta_class_tag
>
{
	typedef meta_class<Class> type;
};

// Meta-function template getting full blown meta-object
// from base meta-object
template <class BaseMetaObject>
struct full_meta_object
 : public get_full_meta_object<
	BaseMetaObject,
	typename BaseMetaObject::category
>{ };

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

