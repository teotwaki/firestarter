/**
 * @file mirror/meta_std_container.hpp
 * @brief Implementation of standard container registering and reflection
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_STD_CONTAINER_1104181551_HPP
#define MIRROR_META_STD_CONTAINER_1104181551_HPP

#include <mirror/meta_container.hpp>
#include <mirror/auxiliary/std_containers.hpp>

MIRROR_NAMESPACE_BEGIN

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro begins the registering of a standard container template
/**
 *  @param TEMPLATE_PARAMS the list of template parameters enclosed
 *  in parentheses. For example (typename A, typename B, typename C)
 *
 *  @param NAMESPACE the namespace where the container is defined
 *
 *  @param CONTAINER the base container class name.
 *
 *  @param PARAM_LIST the list of template parameter names enclosed
 *  in parentheses. For example (A, B, C).
 *
 *  @param ELEMENT the element type
 *
 *  @see MIRROR_REG_STD_CONTAINER_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN( \
	TEMPLATE_PARAMS, \
	NAMESPACE, \
	CLASS, \
	PARAM_LIST, \
	ELEMENT \
)
#else
#define MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN( \
	TEMPLATE_PARAMS, \
	NAMESPACE, \
	CLASS, \
	PARAM_LIST, \
	ELEMENT \
) \
namespace _container { \
template < MIRROR_PP_EXPAND_ARGS TEMPLATE_PARAMS > \
struct _< :: NAMESPACE :: CLASS < \
	MIRROR_PP_EXPAND_ARGS PARAM_LIST \
> > \
{ \
	typedef :: NAMESPACE :: CLASS < \
		MIRROR_PP_EXPAND_ARGS PARAM_LIST \
	> _this; \
	typedef simple_container_tag kind; \
	struct cntnr : _std_base<_this> \
	{ \
		typedef ELEMENT elem_type;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Ends the registering of a standard container
/**
 *
 *  @see MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_STD_CONTAINER_END
#else
#define MIRROR_REG_STD_CONTAINER_END \
	}; \
	static cntnr _cntnr(std::integral_constant<int, 0>); \
	typedef std::integral_constant<int, 1> cntnr_count; \
}; \
} /* namespace _container */
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Registers the traversals of a standard forward container
/**
 *
 *  @see MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN
 *  @see MIRROR_REG_STD_CONTAINER_END
 *  @see MIRROR_REG_STD_REV_CONTAINER_TRAVERSALS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_STD_FWD_CONTAINER_TRAVERSALS
#else
#define MIRROR_REG_STD_FWD_CONTAINER_TRAVERSALS \
	typedef mirror::aux::meta_std_cntnr_trav_fwd<_this> \
		default_traversal; \
	typedef mirror::mp::range< \
		mirror::aux::meta_std_cntnr_trav_fwd<_this> \
	> traversals;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Registers the traversals of a standard reversible container
/**
 *
 *  @see MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN
 *  @see MIRROR_REG_STD_CONTAINER_END
 *  @see MIRROR_REG_STD_FWD_CONTAINER_TRAVERSALS
 *  @ingroup registering_macros
 */
#define MIRROR_REG_STD_REV_CONTAINER_TRAVERSALS
#else
#define MIRROR_REG_STD_REV_CONTAINER_TRAVERSALS \
	typedef mirror::aux::meta_std_cntnr_trav_fwd<_this> \
		default_traversal; \
	typedef mirror::mp::range< \
		mirror::aux::meta_std_cntnr_trav_fwd<_this>, \
		mirror::aux::meta_std_cntnr_trav_bwd<_this> \
	> traversals;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Registers a container operation (inserter, eraser, locator)
/**
 *
 *  @see MIRROR_REG_STD_CONTAINER_TEMPLATE_BEGIN
 *  @see MIRROR_REG_STD_CONTAINER_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_STD_CONTAINER_OP(KIND, NAME)
#else
#define MIRROR_REG_STD_CONTAINER_OP(KIND, NAME) \
	mirror::aux::meta_std_ ## NAME ## _ ## KIND<_this>
#endif

MIRROR_NAMESPACE_END

#endif //include guard

