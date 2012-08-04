/**
 *  @file mirror/constructor_tags.hpp
 *  @brief Declaration of tags for various constructor kinds like
 *  default, copy, move, custom, etc.
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONSTRUCTOR_TAGS_1102041538_HPP
#define MIRROR_CONSTRUCTOR_TAGS_1102041538_HPP

MIRROR_NAMESPACE_BEGIN

/** @defgroup constructor_tags Mirror - Constructor kind tags
 *
 *  Tag types designating various constructor kinds
 *  like default, copy, custom, initializer-list, etc.
 */

/// Tag indicating that the constructor is a default constructor
/**
 *  @ingroup constructor_tags
 */
struct default_constructor_tag { };

/// Tag indicating that the constructor is a default constructor
/**
 *  @ingroup constructor_tags
 */
struct copy_constructor_tag { };
struct move_constructor_tag { };

/// Tag indicating that the constructor is a custom constructor
/** This tag type indicates that the constructor is a custom
 *  (non-default and non-copy/move) constructor with one or
 *  several parameters.
 *
 *  @ingroup constructor_tags
 */
struct custom_constructor_tag { };

/// Tag indicating that the constructor is a initializer-list ctr.
/**
 *  @ingroup constructor_tags
 */
struct initializer_list_constructor_tag { };

/// Tag indicating that the constructor is struct initializer
/**
 *  @ingroup constructor_tags
 */
struct struct_initializer_tag { };

MIRROR_NAMESPACE_END

#endif //include guard

