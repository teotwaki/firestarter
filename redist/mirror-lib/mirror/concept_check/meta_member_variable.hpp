/**
 * @file mirror/concept_check/meta_member_variable.hpp
 * @brief MetaMemberVariable concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_MEMBER_VARIABLE_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_MEMBER_VARIABLE_1011291729_HPP

#include <mirror/concept_check/utils.hpp>
#include <mirror/concept_check/meta_variable.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaType
MIRROR_CONCEPT_TESTER_BEGIN(meta_member_variable_tag)
(X*,
	const typename X::access_type* at = nullptr
);
MIRROR_CONCEPT_TESTER_CONSTRAINTS((
	conforms_to<Tested, meta_variable_tag>
))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

