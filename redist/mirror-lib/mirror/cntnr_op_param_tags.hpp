/**
 * @file mirror/cntnr_op_param_tags.hpp
 * @brief Tags for various container-modifying operation parameter kinds
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CNTNR_OP_PARAM_TAGS_1104270816_HPP
#define MIRROR_CNTNR_OP_PARAM_TAGS_1104270816_HPP

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @defgroup container_mod_param_tags Mirror - Container modifier parameter tags
 *
 *  These tag types can be used to distinguish between various parameter
 *  kinds of container manipulation methods. Container modification/manipulation
 *  methods like element inserters and erasers can have several kinds
 *  of parameters. Inserters usually have the value to be inserted as
 *  a parameter but they can also have a parameter indicating the position,
 *  where the value will be inserted or the number of copies of the element
 *  to be inserted or some combination thereof. Removal methods can specify
 *  the element or elements to be erased by their position, or a range
 *  of positions, by the actual value to be removed, etc.
 *
 *  The position of the element can be depending on the implementation
 *  of the container be specified by iterators, pointers, indices, etc.
 *  Mirror uses Locator objects to adapt these iterators, indices, and so on
 *  and to give them a uniform interface. Locators can be obtained from
 *  Traversal objects and can thus be used to access, modify or even insert
 *  or remove the elements of the container.
 *
 *  For examle the standard containers have several insertion methods
 *  with various signatures like push(T), push_front(T), push_back(T),
 *  insert(Iter, T) or insert(Iter, T, size_t) and removal methods
 *  like clear(), pop(), pop_front(), pop_back(), erase(Iter), erase(Iter, Iter)
 *  or erase(T).
 *
 *  The tag types defined here are composed into ranges and specify
 *  the signature of a container modifier. These signatures can be
 *  used to do tag dispatching between functions which can work
 *  with various kinds of manipulators.
 *
 *  In order to avoid combinatorial problems with the signatures
 *  the inserter/eraser parameter kind tags can appear only in
 *  a particular order in the signatures. If @em position(sig, X)
 *  returns an integer denoting the position of a parameter
 *  kind with tag X in the signature the following is true:
 *
 *  position(signature, container_op_location_param) <
 *  position(signature, container_op_end_location_param) <
 *  position(signature, container_op_count_param).
 *  position(signature, container_op_value_param) <
 */

/// Container operation parameter tag for value parameters
/**
 *  @ingroup container_mod_param_tags
 */
struct container_op_value_param { };

/// Container operation parameter tag for positional parameters
/**
 *  @ingroup container_mod_param_tags
 */
struct container_op_location_param { };

/// Container operation parameter tag for range end postion parameters
/**
 *  @ingroup container_mod_param_tags
 */
struct container_op_end_location_param { };

/// Container operation parameter tag for count parameters
/**
 *  @ingroup container_mod_param_tags
 */
struct container_op_count_param { };

MIRROR_NAMESPACE_END

#endif //include guard

