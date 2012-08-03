/**
 * @file mirror/concept_check/meta_object.hpp
 * @brief MetaObject concept conformance test
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_META_OBJECT_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_META_OBJECT_1011291729_HPP

#include <mirror/concept_check/utils.hpp>

MIRROR_NAMESPACE_BEGIN

// MetaObject
MIRROR_CONCEPT_TESTER_BEGIN(meta_object_tag)
(X*); // no special requirements from the interface
MIRROR_CONCEPT_TESTER_CONSTRAINTS((std::true_type))
MIRROR_CONCEPT_TESTER_END

MIRROR_NAMESPACE_END

#endif //include guard

