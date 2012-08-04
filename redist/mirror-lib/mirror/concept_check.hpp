/**
 * @file mirror/concept_check.hpp
 * @brief Concept conformance checking
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_1011291729_HPP

#include <mirror/concept_check/meta_object.hpp>
#include <mirror/concept_check/meta_meta_object.hpp>
#include <mirror/concept_check/meta_named_object.hpp>
#include <mirror/concept_check/meta_scoped_object.hpp>
#include <mirror/concept_check/meta_scope.hpp>
#include <mirror/concept_check/meta_namespace.hpp>
#include <mirror/concept_check/meta_type.hpp>
#include <mirror/concept_check/meta_typedef.hpp>
#include <mirror/concept_check/meta_class.hpp>
#include <mirror/concept_check/meta_inheritance.hpp>
#include <mirror/concept_check/meta_variable.hpp>
#include <mirror/concept_check/meta_function.hpp>
#include <mirror/concept_check/meta_member_variable.hpp>
#include <mirror/concept_check/meta_parameter.hpp>
#include <mirror/concept_check/meta_constructor.hpp>

MIRROR_NAMESPACE_BEGIN

/** @defgroup concept_checking Mirror - Concept checking
 *
 *  Concept checking utilities allow to check if a particular type
 *  conforms to a concept at compile time. If the tested type does not
 *  conform a readable compile time error message is issued. These
 *  utilities are meant to be used in unit tests.
 */

MIRROR_NAMESPACE_END

#endif //include guard

