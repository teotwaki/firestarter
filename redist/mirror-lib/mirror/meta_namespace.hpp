/**
 * @file mirror/meta_namespace.hpp
 * @brief Implementation of namespace registering and reflection
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_NAMESPACE_1011291729_HPP
#define MIRROR_META_NAMESPACE_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/auxiliary/scoped_named.hpp>
#include <mirror/auxiliary/global_list.hpp>
#include <mirror/ct_string/basic_string.hpp>
#include <mirror/meta_prog/only_if.hpp>
#include <string>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

template <class Namespace>
struct meta_namespace
 : public aux::scoped_named_impl<Namespace>
{
	typedef meta_namespace<typename Namespace::scope> scope;
	typedef typename Namespace::ct_name static_name;
};

// Implementation of the members meta-function for namespaces
template <class Namespace>
struct members<meta_namespace<Namespace> >
{
	typedef MIRROR_GET_GLOBAL_LIST_BASE(
		Namespace,
		MIRROR_COUNTER_LUID,
		typename
	) type;
};

// The implementation of namespaces for instantiation of meta_namespace
template <class X>
struct namespaces<meta_namespace<X> >
{
	typedef typename mp::only_if<
		typename members<meta_namespace<X> >::type,
		mp::is_a<mp::arg<1>, meta_namespace_tag>
	>::type type;
};

namespace _namespace {

// This structure provides basic meta-data about the global scope namespace
/*  Do not use this structure directly, use the appropriate specialization
 *  of meta_namespace instead.
 *  .see meta_namespace
 */
struct _
{
	typedef mirror::meta_namespace_tag category;

	// The scope of this namespace is again the global scope
	typedef _ scope;

	// Implement the individual overloads of the name function
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS("")
	typedef typename cts::string<>::type ct_name;
};

// This typedef is used by the nested namespaces
typedef _ _parent_namespace;

} // namespace _namespace

// Initialize the global list of members of this namespace
MIRROR_REGISTER_GLOBAL_LIST( mirror::_namespace::_ )

namespace _namespace {

// This structure provides basic meta-data about unspecified local scopes
/*  Do not use this structure directly, use the appropriate specialization
 *  of meta_namespace instead.
 *  .see meta_namespace
 */
struct _unspec_scope
{
	typedef mirror::meta_unspecified_scope_tag category;

	// The scope of this namespace is again the global scope
	typedef _ scope;

	// Implement the individual overloads of the name function
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS("<unspecified>")
	typedef typename cts::string<
		'<','u','n','s','p','e','c','i','f','i','e','d','>'
	>::type ct_name;
};

} // namespace _namespace

// This macro implements the _ structure containing the basic metadata
#define MIRROR_REGISTER_BASE_NAMESPACE_META_DATA(NAME, SPELLED) \
struct _ \
{  \
	typedef meta_namespace_tag category; \
	typedef _parent_namespace scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
}; \
typedef _ _parent_namespace;

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro begins the registering of namespace(s) with Mirror
/**
 *  @see MIRROR_REG_NAMESPACE_END
 *  @see MIRROR_REG_NAMESPACE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_NAMESPACE_BEGIN
#else
#define MIRROR_REG_NAMESPACE_BEGIN namespace _namespace {
#endif

#define MIRROR_REG_NAMESPACE_CTS(NAMESPACE_NAME, SPELLED) \
namespace NAMESPACE_NAME { \
MIRROR_REGISTER_BASE_NAMESPACE_META_DATA(NAMESPACE_NAME, SPELLED) \
}

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a namespace with Mirror
/**
 *  @see MIRROR_REG_NAMESPACE_BEGIN
 *  @see MIRROR_REG_NAMESPACE_END
 *  @see MIRRORED_NAMESPACE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_NAMESPACE(NAMESPACE_NAME)
#else
#define MIRROR_REG_NAMESPACE(NAMESPACE_NAME) \
	MIRROR_REG_NAMESPACE_CTS(NAMESPACE_NAME, ())
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro ends the registering of namespace(s) with Mirror
/**
 *  @see MIRROR_REG_NAMESPACE_BEGIN
 *  @see MIRROR_REG_NAMESPACE
 *  @see MIRROR_INIT_NAMESPACE_MEMBER_LIST
 *  @ingroup registering_macros
 */
#define MIRROR_REG_NAMESPACE_END
#else
#define MIRROR_REG_NAMESPACE_END }
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which initializes the namespace's member list
/** This macro initializes the compile-time list of the members
 *  of the @a FULL_NAMESPACE_NAME namespace.
 *  This macro must be used between the @c MIRROR_REG_BEGIN
 *  and the @c MIRROR_REG_END macros.
 *  after registering the namespace, i.e. after the
 *  MIRROR_REG_NAMESPACE_END macro.
 *
 *  @see MIRROR_REG_NAMESPACE_BEGIN
 *  @see MIRROR_REG_NAMESPACE
 *  @see MIRROR_REG_NAMESPACE_END
 *  @ingroup registering_macros
 */
#define MIRROR_INIT_NAMESPACE_MEMBER_LIST(FULL_NAMESPACE_NAME)
#else
#define MIRROR_INIT_NAMESPACE_MEMBER_LIST(FULL_NAMESPACE_NAME) \
MIRROR_REGISTER_GLOBAL_LIST( \
	mirror::_namespace:: FULL_NAMESPACE_NAME ::_ \
) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace:: FULL_NAMESPACE_NAME ::_::scope, \
	mirror::meta_namespace< \
		mirror::_namespace:: FULL_NAMESPACE_NAME ::_ \
	 > \
)
#endif

#define MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE_CTS(NAMESPACE, SPELLED) \
MIRROR_REG_NAMESPACE_BEGIN \
MIRROR_REG_NAMESPACE_CTS(NAMESPACE, SPELLED) \
MIRROR_REG_NAMESPACE_END \
MIRROR_INIT_NAMESPACE_MEMBER_LIST(NAMESPACE)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of a single namespace under the global scope
/** This macro can be used for registering of a single namespace defined
 *  directly in the global scope i.e. not a nested namespace. This macro
 *  takes care of registering the namespace and initializing the member type
 *  list.
 *
 *  @param NAMESPACE the name of the namespace to be registered
 *  @see MIRROR_REG_NAMESPACE_BEGIN
 *  @see MIRROR_REG_NAMESPACE
 *  @see MIRROR_REG_NAMESPACE_END
 *  @see MIRROR_INIT_NAMESPACE_MEMBER_LIST
 *  @see MIRROR_QREG_NESTED_NAMESPACE
 *  @ingroup quick_registering_macros
 */
#define MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE(NAMESPACE)
#else
#define MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE(NAMESPACE) \
	MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE_CTS(NAMESPACE, ())
#endif

#define MIRROR_QREG_NESTED_NAMESPACE_CTS( \
	PARENT_NAMESPACE, \
	NESTED_NAMESPACE, \
	SPELLED \
) \
MIRROR_REG_NAMESPACE_BEGIN \
namespace PARENT_NAMESPACE { \
MIRROR_REG_NAMESPACE_CTS(NESTED_NAMESPACE, SPELLED) \
} \
MIRROR_REG_NAMESPACE_END \
MIRROR_INIT_NAMESPACE_MEMBER_LIST(PARENT_NAMESPACE::NESTED_NAMESPACE)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of a namespace nested in top-level namespace
/** This macro can be used for registering of a single namespace defined
 *  in another namespace nested directly in the global scope. This macro
 *  takes care of registering the namespace and initializing the member type
 *  list. The enclosing parent namespace must be registered before the
 *  nested namespace is registered.
 *
 *  @param PARENT_NAMESPACE the name of the parent namespace
 *  @param NESTED_NAMESPACE the name of the nested namespace to be registered

 *  @see MIRROR_REG_NAMESPACE_BEGIN
 *  @see MIRROR_REG_NAMESPACE
 *  @see MIRROR_REG_NAMESPACE_END
 *  @see MIRROR_INIT_NAMESPACE_MEMBER_LIST
 *  @see MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE
 *  @ingroup quick_registering_macros
 */
#define MIRROR_QREG_NESTED_NAMESPACE(PARENT_NAMESPACE, NESTED_NAMESPACE)
#else
#define MIRROR_QREG_NESTED_NAMESPACE(PARENT_NAMESPACE, NESTED_NAMESPACE)\
	MIRROR_QREG_NESTED_NAMESPACE_CTS(PARENT_NAMESPACE, NESTED_NAMESPACE, ())
#endif

#define MIRROR_QREG_NESTED_NESTED_NAMESPACE_CTS( \
	GRANDPARENT_NAMESPACE, \
	PARENT_NAMESPACE, \
	NESTED_NAMESPACE, \
	SPELLED \
)\
MIRROR_REG_NAMESPACE_BEGIN \
namespace GRANDPARENT_NAMESPACE { \
namespace PARENT_NAMESPACE { \
MIRROR_REG_NAMESPACE_CTS(NESTED_NAMESPACE, SPELLED) \
} } \
MIRROR_REG_NAMESPACE_END \
MIRROR_INIT_NAMESPACE_MEMBER_LIST( \
	GRANDPARENT_NAMESPACE::PARENT_NAMESPACE::NESTED_NAMESPACE \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro for quick registering of a namespace nested in a nested namespace
/** This macro can be used for registering of a single namespace defined
 *  in another namespace nested in namespace under the global scope. This macro
 *  takes care of registering the namespace and initializing the member type
 *  list. The enclosing parent namespace must be registered before the
 *  nested namespace is registered.
 *
 *  @param GRANDPARENT_NAMESPACE the name of the top-level namespace
 *  @param PARENT_NAMESPACE the name of the parent namespace
 *  @param NESTED_NAMESPACE the name of the nested namespace to be registered

 *  @see MIRROR_REG_NAMESPACE_BEGIN
 *  @see MIRROR_REG_NAMESPACE
 *  @see MIRROR_REG_NAMESPACE_END
 *  @see MIRROR_INIT_NAMESPACE_MEMBER_LIST
 *  @see MIRROR_QREG_GLOBAL_SCOPE_NAMESPACE
 *  @see MIRROR_QREG_NESTED_NAMESPACE
 *  @ingroup quick_registering_macros
 */
#define MIRROR_QREG_NESTED_NESTED_NAMESPACE( \
	GRANDPARENT_NAMESPACE, \
	PARENT_NAMESPACE, \
	NESTED_NAMESPACE \
)
#else
#define MIRROR_QREG_NESTED_NESTED_NAMESPACE( \
	GRANDPARENT_NAMESPACE, \
	PARENT_NAMESPACE, \
	NESTED_NAMESPACE \
)	 MIRROR_QREG_NESTED_NESTED_NAMESPACE_CTS( \
	GRANDPARENT_NAMESPACE, \
	PARENT_NAMESPACE, \
	NESTED_NAMESPACE, \
	() \
)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the namespace passed as the @a FULL_NAMESPACE_NAME argument
/** @def MIRRORED_NAMESPACE(FULL_NAMESPACE_NAME)
 *  This macro expands into a type conforming to the MetaNamespace
 *  concept, which provides meta-data about the given namespace.
 *  The namespace name passed as the @a FULL_NAMESPACE_NAME argument
 *  must NOT contain the leading :: (double colon).
 *
 *  @see MIRRORED_GLOBAL_SCOPE
 *  @see MIRROR_REG_NAMESPACE
 *  @see mirror::MetaNamespace
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_NAMESPACE(FULL_NAMESPACE_NAME) mirror::MetaNamespace
#else
#define MIRRORED_NAMESPACE(FULL_NAMESPACE_NAME) \
	mirror::meta_namespace< \
	mirror::_namespace :: FULL_NAMESPACE_NAME :: _ \
	>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the global scope namespace
/** @def MIRRORED_GLOBAL_SCOPE
 *  This macro expands into a type conforming to the MetaNamespace
 *  concept, which provides meta-data about the given namespace.
 *
 *  @see MIRRORED_NAMESPACE
 *  @see mirror::MetaNamespace
 *  @see lagoon::reflected_global_scope
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_GLOBAL_SCOPE() mirror::MetaNamespace
#else
#define MIRRORED_GLOBAL_SCOPE() \
mirror::meta_namespace< \
	mirror::_namespace:: _ \
>
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects an unspecified (local) scope
/** @def MIRRORED_UNSPECIFIED_SCOPE
 *  This macro expands into a type conforming to the MetaNamespace
 *  concept, which provides meta-data about an unspecified scope
 *  used in cases where the scope of an MetaScopedObject is not
 *  important.
 *
 *  @see MIRRORED_NAMESPACE
 *  @see mirror::MetaNamespace
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_UNSPECIFIED_SCOPE() mirror::MetaNamespace
#else
#define MIRRORED_UNSPECIFIED_SCOPE() \
mirror::meta_namespace< \
	mirror::_namespace:: _unspec_scope \
>
#endif


MIRROR_NAMESPACE_END

#endif //include guard

