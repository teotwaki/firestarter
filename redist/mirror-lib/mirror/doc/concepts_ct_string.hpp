/**
 *  @file mirror/doc/concepts_ct_string.hpp
 *  @brief Documentation of compile-time string concepts
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_CT_STRING_1104131331_HPP
#define MIRROR_DOC_CONCEPTS_CT_STRING_1104131331_HPP

#include <mirror/config.hpp>

#ifdef MIRROR_DOCUMENTATION_ONLY

MIRROR_NAMESPACE_BEGIN
// This section is here for documentation purposes only

/** @defgroup ct_string_concepts Mirror - Compile-time strings concepts
 *
 *  These concept classes are defined here only for documentation purposes
 *  and cannot be used in any end-user code.
 */

/// Character constant type
/**
 *  @em Models:
 *   - @c std::integral_constant<char,C>
 *
 *  @ingroup ct_string_concepts
 */
struct CharacterConstantType
{
	static const char value;
	typedef CharacterConstantType type;
};

/// Compile-time string type
/**
 *  @em Models:
 *   - @c mirror::cts::basic_string
 *
 *  @ingroup ct_string_concepts
 */
struct CompileTimeString
{
	typedef CompileTimeString type;
};

MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

