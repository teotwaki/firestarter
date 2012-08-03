/**
 * @file mirror/concept_check/meta_class.hpp
 * @brief MetaClass concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_CLASS_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_CLASS_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_type.hpp>
#include <mirror/concept_check/meta_scope.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaType
MIRROR_CONCEPT_TESTER_BEGIN(meta_class_tag)
(X*,
	const typename members<X>::type* m = nullptr,
	const typename base_classes<X>::type* bc = nullptr,
	const typename member_variables<X>::type* mv = nullptr,
	const typename all_member_variables<X>::type* amv = nullptr,
	const typename constructors<X>::type* c = nullptr
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_type_tag>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

