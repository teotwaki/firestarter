/**
 * @file mirror/meta_type.hpp
 * @brief Implementation of type registering and reflection
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_TYPE_1011291729_HPP
#define MIRROR_META_TYPE_1011291729_HPP

#include <mirror/preprocessor.hpp>
#include <mirror/meta_namespace.hpp>
#include <mirror/auxiliary/template_type_name.hpp>
#include <mirror/auxiliary/meta_type.hpp>
#include <mirror/ct_string/basic_string.hpp>
#include <mirror/meta_prog/lambda.hpp>

MIRROR_NAMESPACE_BEGIN

namespace _type {

// Specialization of this structure provide basic meta-data about types
template <typename Type>
struct _
{
	typedef spec_type_tag kind;
	typedef mirror::cts::string<> ct_name;
};

} // namespace _type

// Implementation of the type_category meta-function
template <typename T>
struct type_category
{
	typedef typename mirror::_type::_<T>::kind type;
};

template <typename T>
typename type_category<T>::type
categorize_type(const T&)
{
    return typename type_category<T>::type();
}


template <class Type>
struct meta_type
 : aux::meta_type_name_impl<
	Type,
	typename mirror::_type::_<Type>::category
>
{
	typedef typename aux::full_meta_object<
		typename mirror::_type::_<Type>::scope
	>::type scope;

	typedef Type original_type;

	typedef typename mirror::_type::_<Type>::ct_name static_name;
};

template <class Type>
struct reflected
{
	typedef typename aux::reflect_type<Type>::type type;
};

template <class Type>
struct reflected_type : public aux::reflect_type<Type>::type
{
	typedef typename aux::reflect_type<Type>::type type;
};

template <class MetaType, class ModifierExpr>
struct apply_modifier
{
	typedef typename reflected<
		typename mirror::mp::apply<
			ModifierExpr,
			typename original_type<
				MetaType
			>::type
		>::type
	>::type type;
};

// Implementation of the parameters meta-function for templated types
template <template <typename...> class Template, typename ... Params>
struct template_parameters<meta_type<Template<Params...> > >
{
	typedef mp::range<meta_type<Params>...> type;
};

// Implementation of the parameters meta-function for templated classes
template <template <typename...> class Template, typename ... Params>
struct template_parameters<meta_class<Template<Params...> > >
{
	typedef mp::range<meta_class<Params>...> type;
};

#define MIRROR_REG_GLOBAL_SCOPE_TYPE_BASICS(KIND, NAME, SPELLED) \
namespace _type { \
template <> \
struct _< NAME > \
{  \
	typedef meta_type_tag category; \
	typedef mirror::spec_ ## KIND ## _tag kind; \
	typedef mirror::_namespace::_ scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
}; \
}

#define MIRROR_REG_GLOBAL_SCOPE_TYPE_CTS(NAME, SPELLED) \
MIRROR_REG_GLOBAL_SCOPE_TYPE_BASICS(type, NAME, SPELLED) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace::_, \
	mirror::meta_type< NAME > \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a type defined in the global scope with Mirror
/**
 *  @param NAME the base type name
 *
 *  @see MIRROR_REG_TYPE
 *  @see MIRRORED_TYPE
 *  @see mirror::reflected
 *
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_TYPE(NAME)
#else
#define MIRROR_REG_GLOBAL_SCOPE_TYPE(NAME) \
	MIRROR_REG_GLOBAL_SCOPE_TYPE_CTS(NAME, ())
#endif

// Helper macro used in implementation of MIRROR_REG_TYPE
#define MIRROR_REG_TYPE_BASICS(KIND, NAMESPACE, NAME, SPELLED) \
namespace _type { \
template <> \
struct _< :: NAMESPACE :: NAME > \
{  \
	typedef meta_type_tag category; \
	typedef mirror::spec_ ## KIND ## _tag kind; \
	typedef mirror::_namespace:: NAMESPACE ::_ scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
}; \
} /* namespace _type */

#define MIRROR_REG_TYPE_CTS(NAMESPACE, NAME, SPELLED) \
MIRROR_REG_TYPE_BASICS(type, NAMESPACE, NAME, SPELLED) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace :: NAMESPACE :: _, \
	mirror::meta_type< :: NAMESPACE :: NAME > \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a type nested in a named namespace with Mirror
/**
 *  @param NAMESPACE the full namespace name NOT containing the
 *  leading double colon
 *  @param NAME the base type name
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_TYPE
 *  @see MIRRORED_TYPE
 *  @see mirror::reflected
 *
 *  @ingroup registering_macros
 */
#define MIRROR_REG_TYPE(NAMESPACE, NAME)
#else
#define MIRROR_REG_TYPE(NAMESPACE, NAME) \
	MIRROR_REG_TYPE_CTS(NAMESPACE, NAME, ())
#endif

// Helper macro used in implementation of MIRROR_REG_*_NESTED
#define MIRROR_REG_NESTED_TYPE_BASICS(KIND, PARENT_CLASS, NAME, SPELLED) \
namespace _type { \
template <> \
struct _< :: PARENT_CLASS :: NAME > \
{  \
	typedef meta_type_tag category; \
	typedef mirror::spec_ ## KIND ## _tag kind; \
	typedef mirror::_class::_< :: PARENT_CLASS > scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
}; \
} /* namespace _type */

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the type passed as the @a FULL_TYPE_NAME argument
/** @def MIRRORED_TYPE(FULL_TYPE_NAME)
 *  This macro expands into a type conforming to the MetaType
 *  concept, which provides meta-data about the given type.
 *  The type name passed as the @a FULL_TYPE_NAME argument
 *  should be a fully qualified type name.
 *
 *  @see MIRROR_REG_TYPE
 *  @see mirror::MetaType
 *  @see mirror::reflected
 *  @see lagoon::reflected_type
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_TYPE(FULL_TYPE_NAME) mirror::MetaType
#else
#define MIRRORED_TYPE(FULL_TYPE_NAME) \
	mirror::meta_type< FULL_TYPE_NAME >
#endif

MIRROR_NAMESPACE_END

#endif //include guard

