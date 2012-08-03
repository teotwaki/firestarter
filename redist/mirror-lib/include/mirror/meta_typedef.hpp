/**
 * @file mirror/meta_typedef.hpp
 * @brief Implementation of typedefined type registering and reflection
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_TYPEDEF_1011291729_HPP
#define MIRROR_META_TYPEDEF_1011291729_HPP

#include <mirror/meta_type.hpp>

MIRROR_NAMESPACE_BEGIN
namespace _typedef {

// This type is used for distinguishing typedefined types from other T<x>
// template types
template <typename BaseTypedef>
struct _selector { };

} // namespace _typedef

namespace _type {

template <typename BaseTypedef>
struct _< mirror::_typedef::_selector<BaseTypedef> >
 : BaseTypedef
{ };

} // namespace _type

template <typename BaseTypedef>
struct meta_type< mirror::_typedef::_selector<BaseTypedef> >
 : aux::scoped_named_impl< BaseTypedef >
{
	typedef meta_typedef_tag _type_category;
	typedef meta_typedef_tag _class_category;

	typedef typename aux::full_meta_object<
		typename BaseTypedef::scope
	>::type scope;

	typedef typename BaseTypedef::source_type original_type;

	typedef meta_class<original_type> type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a typedef from the global scope with Mirror
/**
 *  @param TYPEDEF the typefined type name
 *
 *  @see MIRROR_REG_TYPEDEF
 *  @see MIRRORED_TYPEDEF
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_TYPEDEF(TYPEDEF)
#else
#define MIRROR_REG_GLOBAL_SCOPE_TYPEDEF(TYPEDEF) \
namespace _typedef { \
template <typename Namespace> struct _ ## TYPEDEF ; \
template <> struct _ ## TYPEDEF< mirror::_namespace::_ >\
{  \
	typedef TYPEDEF source_type; \
	typedef meta_type_tag category; \
	typedef mirror::_namespace::_ scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#TYPEDEF) \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace::_, \
	mirror::meta_type< \
	mirror::_typedef::_selector< \
	mirror::_typedef::_ ## TYPEDEF< \
	mirror::_namespace::_ \
	> > > \
)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a typedef nested in a named namespace with Mirror
/**
 *  @param NAMESPACE the full namespace name NOT containing the
 *  leading double colon
 *  @param TYPEDEF the base type name
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_TYPEDEF
 *  @see MIRRORED_TYPEDEF
 *  @ingroup registering_macros
 */
#define MIRROR_REG_TYPEDEF(NAMESPACE, TYPEDEF)
#else
#define MIRROR_REG_TYPEDEF(NAMESPACE, TYPEDEF) \
namespace _typedef { \
template <typename Namespace> struct _ ## TYPEDEF; \
template <> \
struct _ ## TYPEDEF< mirror::_namespace:: NAMESPACE :: _ > \
{  \
	typedef :: NAMESPACE :: TYPEDEF source_type; \
	typedef meta_type_tag category; \
	typedef mirror::_namespace:: NAMESPACE ::_ scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#TYPEDEF) \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace :: NAMESPACE :: _, \
	mirror::meta_type< \
	mirror::_typedef::_selector< \
	mirror::_typedef::_ ## TYPEDEF< \
	mirror::_namespace:: NAMESPACE ::_ \
	> > > \
)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a special type representing a nested typedef to Mirror
/** This macro expands into a special type that represents
 *  a typedef registered with Mirror. This special type can be
 *  used for example in registering class member variables
 *  having typedefined types.
 *
 *  @param NAMESPACE the full namespace name NOT containing the
 *  leading double colon
 *  @param TYPEDEF the base typedef-ined name
 *
 *  @see MIRROR_GLOBAL_SCOPE_TYPEDEF
 *  @see MIRROR_REG_TYPEDEF
 *  @see MIRRORED_TYPEDEF
 *  @see MIRROR_REG_CLASS_MEM_VAR
 *
 *  @ingroup utility_macros
 *  @include mirror/example/typedefd_members.cpp
 */
#define MIRROR_TYPEDEF(NAMESPACE, TYPEDEF)
#else
#define MIRROR_TYPEDEF(NAMESPACE, TYPEDEF) \
	mirror::_typedef::_selector < \
	mirror::_typedef::_ ## TYPEDEF < \
	mirror::_namespace:: NAMESPACE :: _ \
	> >
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Returns a special type representing a typedef from the global scope
/** This macro expands into a special type that represents
 *  a typedef registered with Mirror. This special type can be
 *  used for example in registering class member variables
 *  having typedefined types.
 *
 *  @param TYPEDEF the base typedef-ined name
 *
 *  @see MIRROR_TYPEDEF
 *  @see MIRROR_REG_GLOBAL_SCOPE_TYPEDEF
 *  @see MIRRORED_TYPEDEF_GLOBAL_SCOPE
 *  @see MIRROR_REG_CLASS_MEM_VAR
 *
 *  @ingroup utility_macros
 */
#define MIRROR_GLOBAL_SCOPE_TYPEDEF(TYPEDEF)
#else
#define MIRROR_GLOBAL_SCOPE_TYPEDEF(TYPEDEF) \
	mirror::_typedef::_selector < \
	mirror::_typedef::_ ## TYPEDEF < \
	mirror::_namespace::  _ \
	> >
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the typedef @a TYPEDEF defined in the @a NAMESPACE namespace
/**
 *  This macro expands into a type conforming to the MetaTypedef
 *  concept, which provides meta-data about the given typedef.
 *
 *  @see MIRRORED_TYPEDEF_GLOBAL_SCOPE
 *  @see MIRROR_REG_TYPE
 *  @see MIRROR_REG_TYPEDEF
 *  @see mirror::MetaTypedef
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_TYPEDEF(NAMESPACE, TYPEDEF) mirror::MetaTypedef
#else
#define MIRRORED_TYPEDEF(NAMESPACE, TYPEDEF) \
	mirror::meta_type< \
	mirror::_typedef::_selector< \
	mirror::_typedef::_ ## TYPEDEF< \
	mirror::_namespace::NAMESPACE::_ \
	> > >
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the typedef @a TYPEDEF defined in the global scope
/**
 *  This macro expands into a type conforming to the MetaTypedef
 *  concept, which provides meta-data about the given typedef.
 *
 *  @see MIRRORED_TYPEDEF
 *  @see MIRROR_REG_TYPE
 *  @see MIRROR_REG_TYPEDEF
 *  @see mirror::MetaTypedef
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_GLOBAL_SCOPE_TYPEDEF(TYPEDEF) mirror::MetaTypedef
#else
#define MIRRORED_GLOBAL_SCOPE_TYPEDEF(TYPEDEF) \
	mirror::meta_type< \
	mirror::_typedef::_selector< \
	mirror::_typedef::_ ## TYPEDEF< \
	mirror::_namespace::_ \
	> > >
#endif

MIRROR_NAMESPACE_END

#endif //include guard

