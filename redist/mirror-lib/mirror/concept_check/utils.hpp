/**
 * @file mirror/concept_check/utils.hpp
 * @brief Concept checking utilities
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CONCEPT_CHECK_UTILS_1011291729_HPP
#define MIRROR_CONCEPT_CHECK_UTILS_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/meta_prog/is_a.hpp>
#include <mirror/meta_prog/and.hpp>
#include <mirror/preprocessor.hpp>


MIRROR_NAMESPACE_BEGIN

// helper template use in implementation of conforms_to
// Specializations of this template check the conformance
// of the Tested type with various concepts
//
// Don't use or specialize this template directly use
// the MIRROR_CONCEPT_TESTER_* macros
template <typename Tested, typename ConceptTag>
class conforms_to_helper;


#define MIRROR_CONCEPT_TESTER_BEGIN(CONCEPT_TAG) \
template <typename Tested> \
class conforms_to_helper<Tested, CONCEPT_TAG> \
{ \
private: \
	template <typename X> \
	static typename mirror::mp::is_a< \
		X, \
		CONCEPT_TAG \
	>::type category_test(X*); \
	static std::false_type category_test(...); \
	static std::false_type test(...); \
	template <class X> \
	static std::true_type test

#define MIRROR_CONCEPT_TESTER_END \
public: \
	typedef typename mirror::mp::and_< \
		other_constraints, \
		decltype(category_test((Tested*)nullptr)), \
		decltype(test((Tested*)nullptr)) \
	>::type type; \
};

#define MIRROR_CONCEPT_TESTER_CONSTRAINTS(PARAM) \
	typedef typename MIRROR_PP_EXPAND_ARGS PARAM :: type \
		other_constraints;

/// Checks if the @c Tested type conforms to a concept
/** This type inherits from @c std::true_type if the @c Tested type
 *  conforms to the concept specified by the @c ConceptTag,
 *  inherits from @c std::false_type otherwise.
 *
 *  @see MIRROR_ASSERT_CONCEPT
 *  @ingroup concept_checking
 */
template <typename Tested, typename ConceptTag>
struct conforms_to
 : public conforms_to_helper<Tested, ConceptTag>::type
{ };

/// Compile-time concept conformance assertion
/** This macro triggers a compile-time error with a readable message
 *  if the @a TESTED type does not conform to a concept specified
 *  by the @a CONCEPT_TAG
 *
 *  @see conforms_to
 *  @ingroup concept_checking
 */
#define MIRROR_ASSERT_CONCEPT(TESTED, CONCEPT_TAG) \
static_assert( \
	mirror::conforms_to< \
		TESTED, \
		mirror::CONCEPT_TAG \
	>::value, \
	#TESTED " does not conform to " #CONCEPT_TAG \
)

MIRROR_NAMESPACE_END

#endif //include guard

