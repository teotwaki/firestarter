/**
 *  @file mirror/mirror_base.hpp
 *  @brief All-in-one inclusion of everything in Mirror without
 *  pre-registered namespaces, types, etc.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_MIRROR_NO_PREREG_1011291729_HPP
#define MIRROR_MIRROR_NO_PREREG_1011291729_HPP

// basic meta-object implementation
#include <mirror/meta_namespace.hpp>
#include <mirror/meta_type.hpp>
#include <mirror/meta_typedef.hpp>
#include <mirror/meta_class.hpp>
#include <mirror/meta_enum.hpp>
#include <mirror/meta_type_template.hpp>
#include <mirror/meta_variable.hpp>
#include <mirror/meta_function.hpp>
#include <mirror/meta_container.hpp>
#include <mirror/meta_std_container.hpp>
// meta-meta-objects
#include <mirror/meta_meta_object.hpp>
//
// intrinsic meta-functions
#include <mirror/intrinsic.hpp>
//
// meta-programming utilities
#include <mirror/meta_prog.hpp>
//
// specifier tags
#include <mirror/specifier_tags.hpp>
//
// type traits
#include <mirror/type_traits.hpp>
//
// the using directive utility
#include <mirror/using_directive.hpp>
//
// object tagging
#include <mirror/object_tagging.hpp>
//
// factory generators
#include <mirror/factory.hpp>

#endif //include guard

