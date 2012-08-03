/**
 * @file mirror/concept_check/meta_type.hpp
 * @brief MetaType concept conformance test
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_TYPE_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_TYPE_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_named_object.hpp>
#include <mirror/concept_check/meta_scoped_object.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaType
MIRROR_CONCEPT_TESTER_BEGIN(meta_type_tag)
(X*,
	const typename X::original_type* ot = nullptr
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_named_scoped_object_tag>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

