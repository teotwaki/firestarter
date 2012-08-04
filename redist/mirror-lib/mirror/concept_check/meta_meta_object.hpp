/**
 * @file mirror/concept_check/meta_meta_object.hpp
 * @brief MetaScopedObject concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_META_OBJECT_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_META_OBJECT_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_object.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaScopedObject
MIRROR_CONCEPT_TESTER_BEGIN(meta_meta_object_tag)
(X*,
	typename X::meta_object* mo = nullptr,
	typename X::category* c = nullptr,
	const std::string& bn = X::base_name()
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_object_tag>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

