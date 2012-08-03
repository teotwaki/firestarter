/**
 * @file mirror/by_name.hpp
 * @brief Implementation of the by_name meta-function returning meta-variables
 * reflecting class member variables by its name
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_BY_NAME_1012132135_HPP
#define MIRROR_BY_NAME_1012132135_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/auxiliary/by_name.hpp>
#include <mirror/meta_prog/as_a.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_object_tags.hpp>

MIRROR_NAMESPACE_BEGIN

template <class MetaVariables>
struct by_name : aux::by_name_hlpr<typename MetaVariables::type>
{ };

template <class MetaNamespaceExpr>
struct free_variables_by_name;

template <class X>
struct free_variables_by_name<meta_namespace<X> >
 : by_name<typename free_variables<meta_namespace<X> >::type>
{ };

template <class MetaNamespaceExpr>
struct free_variables_by_name : free_variables_by_name<
	typename mp::as_a<
		MetaNamespaceExpr,
		meta_namespace_tag
	>::type
>{ };

template <class MetaClassExpr>
struct member_variables_by_name;

template <class Class>
struct member_variables_by_name<meta_class<Class> >
 : by_name<typename member_variables<meta_class<Class> >::type>
{ };

template <class MetaClassExpr>
struct member_variables_by_name : member_variables_by_name<
	typename mp::as_a<
		MetaClassExpr,
		meta_class_tag
	>::type
>{ };

template <class MetaClassExpr>
struct all_member_variables_by_name;

template <class Class>
struct all_member_variables_by_name<meta_class<Class> >
 : by_name<typename all_member_variables<meta_class<Class> >::type>
{ };

template <class MetaClassExpr>
struct all_member_variables_by_name : all_member_variables_by_name<
	typename mp::as_a<
		MetaClassExpr,
		meta_class_tag
	>::type
>{ };


MIRROR_NAMESPACE_END

#endif //include guard

