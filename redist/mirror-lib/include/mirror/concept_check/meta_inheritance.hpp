/**
 * @file mirror/concept_check/meta_inheritance.hpp
 * @brief MetaInheritance concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_INHERITANCE_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_INHERITANCE_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_object.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaScopedObject
MIRROR_CONCEPT_TESTER_BEGIN(meta_inheritance_tag)
(X*,
	typename X::derived_class* dc = nullptr,
	typename X::inheritance_type * it = nullptr,
	typename X::access_type * at = nullptr,
	typename X::base_class* bc = nullptr
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_object_tag>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

