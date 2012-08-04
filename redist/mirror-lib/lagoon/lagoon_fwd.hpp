/**
 * @file lagoon/lagoon_fwd.hpp
 * @brief Forward declaration of the meta-level classes reflecting
 * base-level program constructs.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_LAGOON_FWD_1011291729_HPP
#define LAGOON_LAGOON_FWD_1011291729_HPP

#include <mirror/raw_ptr/raw_ptr.hpp>
#include <mirror/meta_object_tags.hpp>
#include <lagoon/config.hpp>
#include <string>

LAGOON_NAMESPACE_BEGIN

typedef mirror::raw_ptr raw_ptr;
typedef mirror::unique_raw_ptr unique_raw_ptr;
typedef mirror::shared_raw_ptr shared_raw_ptr;
using mirror::raw_cast;

// Forward declarations of the run-time meta-lavel interfaces

// MetaMetaObject
struct meta_meta_object;

// forward declaration of the interfaces
#define LAGOON_HLPR_FWD_MO_INTERFACE(OBJECT, I, X) \
struct meta_ ## OBJECT;
MIRROR_FOR_EACH_META_OBJECT(LAGOON_HLPR_FWD_MO_INTERFACE, _)
#undef LAGOON_HLPR_FWD_MO_INTERFACE

class meta_meta_object_;

class meta_namespace_;

class meta_global_scope_;

template <typename MetaType>
class meta_type_;

template <typename MetaType>
class meta_typedef_;

class meta_type_template_;

template <typename MetaTemplatedType>
class meta_templated_type_;

template <typename MetaClass>
class meta_class_;

template <typename MetaTemplatedClass>
class meta_templated_class_;

template <typename MetaEnum>
class meta_enum_;

class meta_inheritance_;

class meta_variable_;

class meta_free_variable_;

class meta_plain_free_variable_;

class meta_member_variable_;

class meta_parameter_;

class meta_function_;

class meta_member_function_;

class meta_overloaded_functions_;

class meta_conversion_operator_;

class meta_constructor_;


LAGOON_NAMESPACE_END

#endif //include guard

