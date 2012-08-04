/**
 * @file mirror/cntnr_kind_tags.hpp
 * @brief Tags for container kinds
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CNTNR_KIND_TAGS_1104291200_HPP
#define MIRROR_CNTNR_KIND_TAGS_1104291200_HPP

#include <mirror/config.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

/** @defgroup container_kind_tags Mirror - Container kind class
 *
 *  These tag types can be used to distinguish between various container
 *  kinds and to do function overload or template class dispatching.
 */

#define MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS(UID) \
static std::integral_constant<int, UID> _uid(void) \
{ \
	return std::integral_constant<int, UID>(); \
}


/// Tag for non-container types
/**
 *  For non-container classes @c X the following is true:
 *  @code
 *  mp::empty<all_containers<reflected_type<X> > >::value;
 *  @endcode

 *  @ingroup container_kind_tags
 */
struct non_container_tag
{ MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS(__LINE__) };

/// Base tag for container types
/**
 *  @ingroup container_kind_tags
 */
struct container_tag
{ };

/// Tag for simple multi-value single type containers
/**
 *  Examples of simple containers are the containers from
 *  the standard library, like @c vector, @c list, @c set, etc.
 *  Such container classes only contain the elements and do not
 *  have any member variables.
 *
 *  For simple container classes @c X the following is true:
 *  @code
 *  mp::empty<all_member_variables<reflected_type<X> > >::value;
 *  mp::size<all_containers<reflected_type<X> >::value == 1;
 *  @endcode
 *
 *  @ingroup container_kind_tags
 */
struct simple_container_tag
 : public container_tag
{ MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS(__LINE__) };

/// Tag for single value containers
/**
 *
 *  @see optional_container_tag
 *  @see variant_container_tag
 *  @ingroup container_kind_tags
 */
struct single_value_container_tag
 : public container_tag
{ };

/// Tag for zero-or-single-value single type containers
/**
 *  An example of this kind of container is the Boost.Optional type
 *
 *  For optional container classes @c X the following is true:
 *  @code
 *  mp::empty<all_member_variables<reflected_type<X> > >::value;
 *  mp::size<all_containers<reflected_type<X> > >::value == 1;
 *  single_value<mp::front<all_containers<reflected_type<X> > > >::value;
 *  @endcode
 *
 *  @ingroup container_kind_tags
 */
struct optional_container_tag
 : public single_value_container_tag
{ MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS(__LINE__) };

/// Tag for single-value multi type discriminated union containers
/**
 *  An example of this kind of container is the Boost.Variant type
 *
 *  For variant container classes @c X, and integral constants @c I, @c N
 *  the following is true:
 *  @code
 *  I >= 0;
 *  N >= 0;
 *  I < N;
 *  mp::empty<all_member_variables<reflected_type<X> > >::value;
 *  mp::size<all_containers<reflected_type<X> > >::value == N;
 *  single_value<mp::at_c<all_containers<reflected_type<X> > >, I>::value;
 *  @endcode
 *
 *  @ingroup container_kind_tags
 */
struct variant_container_tag
 : public single_value_container_tag
{ MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS(__LINE__) };

/// Tag for complex container classes
/**
 *
 *  @ingroup container_kind_tags
 */
struct complex_container_tag
 : public container_tag
{ MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS(__LINE__) };


/// Returns all defined container kind tags
/**
 *  @see ContainerKindTag
 *
 *  @ingroup container_kind_tags
 */
struct container_kind_tags
{
#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Range containing all container kind tags
	typedef Range<ContainerKindTag> type;
#else
	typedef mp::range<
		non_container_tag,
		simple_container_tag,
		optional_container_tag,
		variant_container_tag,
		complex_container_tag
	> type;
#endif
};

#undef MIRROR_IMPLEMENT_CONTAINER_KIND_FUNCTIONS

MIRROR_NAMESPACE_END

#endif //include guard

