/**
 * @file mirror/meta_type_template.hpp
 * @brief Implementation of type registering and reflection
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_TYPE_TEMPLATE_1011291729_HPP
#define MIRROR_META_TYPE_TEMPLATE_1011291729_HPP

#include <mirror/meta_namespace.hpp>
#include <mirror/preprocessor.hpp>

MIRROR_NAMESPACE_BEGIN

namespace _type {

template <template <typename ...> class Template, typename ... Params>
struct _<Template<Params...> >
 : _template::_<Template>
{
	typedef meta_templated_type_tag category;
	typedef typename mirror::_template::_<
		Template
	>::kind kind;
	typedef mirror::cts::string<> ct_name;
};

} // namespace _type

#define MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE_CTS( \
	ELABORATED_TYPE, \
	NAME, \
	SPELLED \
) \
namespace _template { \
template <> \
struct _< NAME > \
{  \
	typedef meta_type_template_tag category; \
	typedef spec_ ## ELABORATED_TYPE ## _tag kind; \
	typedef mirror::_namespace::_ scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace::_, \
	mirror::meta_type_template< NAME > \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a class template defined in the global scope
/**
 *  @param NAME the base type name
 *
 *  @see MIRROR_REG_TYPE_TEMPLATE
 *  @see MIRROR_REG_CLASS_TEMPLATE
 *  @see MIRRORED_TEMPLATE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE(ELABORATED_TYPE, NAME)
#else
#define MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE(ELABORATED_TYPE, NAME) \
	MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE_CTS(ELABORATED_TYPE, NAME, ())
#endif

#define MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE_CTS(NAME, SPELLED) \
	MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE_CTS(type, NAME, SPELLED)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a type template defined in the global scope
/**
 *  @param NAME the base type name
 *
 *  @see MIRROR_REG_TYPE_TEMPLATE
 *  @see MIRROR_REG_CLASS_TEMPLATE
 *  @see MIRRORED_TEMPLATE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE(NAME)
#else
#define MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE(NAME) \
	MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE_CTS(NAME, ())
#endif


#define MIRROR_REG_CLASS_TEMPLATE_CTS( \
	ELABORATED_TYPE, \
	NAMESPACE, \
	NAME, \
	SPELLED \
) \
namespace _template { \
template <> \
struct _< :: NAMESPACE :: NAME > \
{  \
	typedef meta_type_template_tag category; \
	typedef spec_ ## ELABORATED_TYPE ## _tag kind; \
	typedef mirror::_namespace:: NAMESPACE ::_ scope; \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#NAME) \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type ct_name; \
}; \
} MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace:: NAMESPACE ::_, \
	mirror::meta_type_template< :: NAMESPACE :: NAME > \
)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a template class nested in a named namespace with Mirror
/**
 *  @param ELABORATED_TYPE the elaborated type specifier (class | struct )
 *  @param NAMESPACE the full namespace name NOT containing the
 *  leading double colon
 *  @param NAME the base template type name
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE
 *  @see MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE
 *  @see MIRRORED_TEMPLATE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_TEMPLATE(ELABORATED_TYPE, NAMESPACE, NAME)
#else
#define MIRROR_REG_CLASS_TEMPLATE(ELABORATED_TYPE, NAMESPACE, NAME) \
	MIRROR_REG_CLASS_TEMPLATE_CTS(ELABORATED_TYPE, NAMESPACE, NAME, ())
#endif

#define MIRROR_REG_TYPE_TEMPLATE_CTS(NAMESPACE, NAME, SPELLED) \
	MIRROR_REG_CLASS_TEMPLATE_CTS(type, NAMESPACE, NAME, SPELLED)

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a template type nested in a named namespace with Mirror
/**
 *  @param NAMESPACE the full namespace name NOT containing the
 *  leading double colon
 *  @param NAME the base template type name
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE
 *  @see MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE
 *  @see MIRRORED_TEMPLATE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_TYPE_TEMPLATE(NAMESPACE, NAME)
#else
#define MIRROR_REG_TYPE_TEMPLATE(NAMESPACE, NAME) \
	MIRROR_REG_TYPE_TEMPLATE_CTS(NAMESPACE, NAME, ())
#endif


#define MIRROR_REG_CLASS_TEMPLATE_BEGIN_CTS( \
	TEMPLATE_PARAMS, \
	ELABORATED_TYPE, \
	NAMESPACE, \
	CLASS, \
	SPELLED, \
	PARAM_LIST \
) \
MIRROR_REG_CLASS_TEMPLATE_CTS(ELABORATED_TYPE, NAMESPACE, CLASS, SPELLED) \
namespace _class { \
template < MIRROR_PP_EXPAND_ARGS TEMPLATE_PARAMS > \
struct _< :: NAMESPACE :: CLASS < \
	MIRROR_PP_EXPAND_ARGS PARAM_LIST \
> > \
 : _defaults< :: NAMESPACE :: CLASS < \
	MIRROR_PP_EXPAND_ARGS PARAM_LIST \
>, spec_ ## ELABORATED_TYPE ## _tag > \
{ \
	typedef spec_ ## ELABORATED_TYPE ## _tag kind; \
	typedef :: NAMESPACE :: CLASS < \
		MIRROR_PP_EXPAND_ARGS PARAM_LIST \
	> _this;

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro begins the registering of a class template with Mirror
/** This macro is similar to the MIRROR_REG_CLASS_BEGIN
 *  except that it registers a class template instead of a regular
 *  class. The registering is ended with the MIRROR_REG_CLASS_END
 *  macro.
 *
 *  @param TEMPLATE_PARAMS the list of template parameters enclosed
 *  in parentheses. For example (typename A, typename B, typename C)
 *
 *  @param ELABORATED_TYPE the type of the class this parameter can have
 *  the following values (class, struct, union, enum and _ for default (class))
 *
 *  @param NAMESPACE the full namespace name inside of which the class is
 *  nested.
 *
 *  @param CLASS the base class name.
 *
 *  @param PARAM_LIST the list of template parameter names enclosed
 *  in parentheses. For example (A, B, C).
 *
 *  @see MIRROR_REG_CLASS_BEGIN
 *  @see MIRROR_REG_CLASS_END
 *  @see MIRROR_REG_TYPE_TEMPLATE
 *  @see MIRROR_REG_CLASS_TEMPLATE
 *  @see MIRROR_REG_GLOBAL_SCOPE_TYPE_TEMPLATE
 *  @see MIRROR_REG_GLOBAL_SCOPE_CLASS_TEMPLATE
 *  @see MIRRORED_TEMPLATE
 *  @ingroup registering_macros
 */
#define MIRROR_REG_CLASS_TEMPLATE_BEGIN( \
	TEMPLATE_PARAMS, \
	ELABORATED_TYPE, \
	NAMESPACE, \
	CLASS, \
	PARAM_LIST \
)
#else
#define MIRROR_REG_CLASS_TEMPLATE_BEGIN( \
	TEMPLATE_PARAMS, \
	ELABORATED_TYPE, \
	NAMESPACE, \
	CLASS, \
	PARAM_LIST \
) MIRROR_REG_CLASS_TEMPLATE_BEGIN_CTS( \
	TEMPLATE_PARAMS, \
	ELABORATED_TYPE, \
	NAMESPACE, \
	CLASS, (), \
	PARAM_LIST \
)
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the template passed as the @a FULL_TEMPLATE_NAME argument
/** @def MIRRORED_TEMPLATE(FULL_TEMPLATE_NAME)
 *  This macro expands into a type conforming to the MetaTypeTemplate
 *  concept, which provides meta-data about the given parameric type.
 *  The template name passed as the @a FULL_TEMPLATE_NAME argument
 *  should be a fully qualified type name.
 *
 *  @see MIRROR_REG_TYPE_TEMPLATE
 *  @see MIRROR_REG_CLASS_TEMPLATE
 *  @see mirror::MetaTypeTemplate
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_TEMPLATE(FULL_TEMPLATE_NAME) mirror::MetaTypeTemplate
#else
#define MIRRORED_TEMPLATE(FULL_TEMPLATE_NAME) \
	mirror::meta_type_template< FULL_TEMPLATE_NAME >
#endif

template <template <typename ...> class Template>
struct meta_type_template
 : public aux::scoped_named_impl<
	mirror::_template::_<Template>
>
{
	typedef typename mirror::_template::_<Template>::ct_name static_name;

	typedef typename aux::full_meta_object<
		typename mirror::_template::_<Template>::scope
	>::type scope;

	template <typename ... P>
	struct reflected_template
	{
		typedef Template<P...> type;
	};
};

MIRROR_NAMESPACE_END

#endif //include guard

