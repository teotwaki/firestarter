/**
 * @file mirror/concept_check/meta_named_object.hpp
 * @brief MetaNamedObject and MetaNamedScopedObject concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_NAMED_OBJECT_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_NAMED_OBJECT_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_scoped_object.hpp>

MIRROR_NAMESPACE_BEGIN
// MetaNamedObject
MIRROR_CONCEPT_TESTER_BEGIN(meta_named_object_tag)
(X*,
	const std::string& bn = X::base_name()
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_object_tag>
))
MIRROR_CONCEPT_TESTER_END

// MetaNamedScopedObject
MIRROR_CONCEPT_TESTER_BEGIN(meta_named_scoped_object_tag)
(X*,
	const std::string& fn = X::full_name(),
	const std::string& ln = X::local_name()
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	mp::and_<
		conforms_to<Tested, meta_named_object_tag>,
		conforms_to<Tested, meta_scoped_object_tag>
	>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

