/**
 * @file puddle/meta_object_tags.hpp
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_META_OBJECT_TAGS_1103151324_HPP
#define PUDDLE_META_OBJECT_TAGS_1103151324_HPP

#include <puddle/config.hpp>

PUDDLE_NAMESPACE_BEGIN

/** @defgroup puddle_meta_object_categories Puddle - Categorization and Tag dispatching
 *
 *  Puddle adds several new tag types to the ones already defined by Mirror.
 *  These types can be used for compile-time template specialization
 *  selection and function overload dispatching.
 */

/// Tag for types which conform to the Puddle's MetaInstance concept
/**
 *  @see MetaInstance
 *  @ingroup puddle_meta_object_categories
 */
struct meta_instance_tag
{ };

/// Tag for types which conform to the Puddle's MetaPlainInstance concept
/**
 *  @see MetaPlainInstance
 *  @ingroup puddle_meta_object_categories
 */
struct meta_plain_instance_tag
 : virtual meta_instance_tag
{ };

PUDDLE_NAMESPACE_END

#endif //include guard

