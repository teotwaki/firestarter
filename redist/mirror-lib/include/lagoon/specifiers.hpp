/**
 * @file lagoon/specifiers.hpp
 * @brief Definitions of the specifiers and categorization tag values
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_SPECIFIERS_1011291729_HPP
#define LAGOON_SPECIFIERS_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/auxiliary/fwd.hpp>
#include <lagoon/utils.hpp>
#include <puddle/specifiers.hpp>
#include <mirror/specifier_tags.hpp>
#include <mirror/preprocessor.hpp>

LAGOON_NAMESPACE_BEGIN

/** @defgroup lagoon_specifiers Lagoon - Specifiers
 *
 *  Lagoon provides access to specifier meta-data through specifier
 *  interfaces and allows to do run-time categorization with specifier
 *  tags.
 */

/// Enumeration of tags categorizing various specifiers
/**
 *  The enumerated tag value names match to the names of
 *  Mirror's specifier tag type names, like spec_virtual_tag, ...,
 *  spec_public_tag, spec_class_tag, etc.
 *
 *  @ingroup lagoon_specifiers
 */
#ifdef MIRROR_DOCUMENTATION_ONLY
enum specifier_category;
#else
using puddle::specifier_category;
#endif

/// Common base interface for various specifiers
/** Serves as the base interface for classes representing various
 *  specifiers.
 *
 *  @ingroup lagoon_specifiers
 */
struct specifier
{
private:
	const std::string _keyword;
	specifier_category _category;

	static inline bool category_matches(unsigned a, unsigned b)
	{
		return (a & b) == b;
	}
protected:
	template <class Specifier>
	specifier(Specifier)
	 : _keyword(Specifier::keyword())
	 , _category(puddle::get_category(Specifier()))
	{ }
public:
	/// Returns the specifier keyword
	std::string keyword(void)
	{
		return _keyword;
	}

	/// Returns the tag identifying the category of the specifier
	specifier_category category(void)
	{
		return _category;
	}

	/// Convenience function for checking if the specifier is of a category
	inline bool is_a(specifier_category cat)
	{
		return category_matches(
			unsigned(category()),
			unsigned(cat)
		);
	}

	/// Returns true if this is the non-specifier
	bool is_none(void)
	{
		return is_a(specifier_category::spec__tag);
	}

#define LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN_false(KIND)
#define LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN_true(KIND) \
	bool is_ ## KIND ## _spec(void) \
	{ \
		return is_a(specifier_category::spec_##KIND##_tag); \
	}
#define LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN(SPEC, KW, GRP, I, X) \
	LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN_##GRP(SPEC)

MIRROR_FOR_EACH_SPECIFIER(
	LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN,
	0
)

#undef LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN_false
#undef LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN_true
#undef LAGOON_HELPER_MAKE_IS_SPEC_GROUP_FN
};


#define LAGOON_HELPER_MAKE_SPEC_KIND_IS_X_FN(SPEC, X) \
	bool is_ ## SPEC(void) \
	{ \
		return is_a(specifier_category::spec_## SPEC ##_tag);\
	}


#define LAGOON_HELPER_MAKE_SPEC_KIND_INTF_false(KIND)
#define LAGOON_HELPER_MAKE_SPEC_KIND_INTF_true(KIND) \
struct KIND ## _specifier : public specifier \
{ \
private: \
	template <typename Specifier, typename Interface> \
	friend shared<Interface> aux::get_##KIND##_spec(void);\
\
	template <class Specifier> \
	KIND ## _specifier(Specifier spec) \
	 : specifier(spec) \
	{ } \
public: \
	MIRROR_FOR_EACH_SPECIFIER_TYPE( \
		KIND, \
		LAGOON_HELPER_MAKE_SPEC_KIND_IS_X_FN, \
		0, \
		MIRROR_PP_EMPTY \
	) \
};

#define LAGOON_HELPER_MAKE_SPEC_KIND_INTF(SPEC, KW, GRP, I, X)\
	LAGOON_HELPER_MAKE_SPEC_KIND_INTF_##GRP(SPEC)

MIRROR_FOR_EACH_SPECIFIER(
	LAGOON_HELPER_MAKE_SPEC_KIND_INTF,
	0
)

#undef LAGOON_HELPER_MAKE_SPEC_KIND_INTF
#undef LAGOON_HELPER_MAKE_SPEC_KIND_INTF_false
#undef LAGOON_HELPER_MAKE_SPEC_KIND_INTF_true
#undef LAGOON_HELPER_MAKE_SPEC_KIND_IS_X_FN

LAGOON_NAMESPACE_END


#endif //include guard

