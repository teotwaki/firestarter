/**
 * @file mirror/concept_check/meta_namespace.hpp
 * @brief MetaNamespace and MetaGlobalScope concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_NAMESPACE_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_NAMESPACE_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_named_object.hpp>
#include <mirror/concept_check/meta_scoped_object.hpp>
#include <mirror/concept_check/meta_scope.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaNamespace
MIRROR_CONCEPT_TESTER_BEGIN(meta_namespace_tag)
(X*);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	mp::and_<
		conforms_to<Tested, meta_named_scoped_object_tag>,
		conforms_to<Tested, meta_scope_tag>
	>
))
MIRROR_CONCEPT_TESTER_END

// MetaGlobalScope
MIRROR_CONCEPT_TESTER_BEGIN(meta_global_scope_tag)
(X*);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_namespace_tag>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

