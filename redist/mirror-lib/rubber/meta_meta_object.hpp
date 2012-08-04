/**
 * @file rubber/meta_meta_object.hpp
 * @brief Wrapper for meta-meta-objects
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_META_META_OBJECT_1103151301_HPP
#define RUBBER_META_META_OBJECT_1103151301_HPP

#include <rubber/config.hpp>
#include <mirror/meta_object_tags.hpp>
#include <puddle/meta_object_tags.hpp>

#include <string>

RUBBER_NAMESPACE_BEGIN

/** @defgroup rubber_type_erased Rubber - Type-erased meta-objects, specifiers, etc.
 *
 *  Rubber defines types which (with several minor exceptions) follow
 *  the interitance hierarchy of Mirror's concepts and erase the types
 *  of the types conforming to those concepts. This allows for example
 *  to separate algorithms from real types of the meta-object.
 *
 *  Optionally, if the #RUBBER_POLYMORPHIC_META_OBJECT preprocessor
 *  symbol is defined, Rubber's meta-object can be even more polymorphic
 *  and allow downcasting to more specialized meta-objects from generalizations.
 */

/// Meta-meta-object reflecting a meta-object
/** This type wraps types conforming to Mirror's MetaMetaObject concept
 *
 *  @see mirror::MetaMetaObject
 *  @ingroup rubber_type_erased
 */
class meta_meta_object
{
private:
	std::string _base_name;
	std::string _construct_name;

	static inline bool category_matches(unsigned a, unsigned b)
	{
		return (a & b) == b;
	}

	puddle::meta_object_category _cat;
public:
	/// Constructor, erasing the type of a Mirror's MetaMetaObject
	template <typename MetaMetaObject>
	meta_meta_object(MetaMetaObject mo)
	 : _base_name(MetaMetaObject::base_name())
	 , _construct_name(MetaMetaObject::construct_name())
	 , _cat(puddle::get_category(typename MetaMetaObject::category()))
	{ }

	/// Returns the base name of the reflected meta-object
	inline std::string base_name(void) const
	{
		return _base_name;
	}

	/// Returns a human-readable name of the reflected construct
	inline std::string construct_name(void) const
	{
		return _construct_name;
	}

	/// Returns the category of the reflected meta-object
	puddle::meta_object_category category(void) const
	{
		return _cat;
	}

	/// Returns true if the reflected meta-object is of the cat category
	inline bool is_a(puddle::meta_object_category cat) const
	{
		return category_matches(
			unsigned(category()),
			unsigned(cat)
		);
	}

	#ifdef MIRROR_DOCUMENTATION_ONLY
	/// A set of member functions indicating reflected meta-object's category
	/** This is not a single member-function, it is instead a set of
 	 *  functions like is_meta_type, is_meta_namespace, is_meta_variable, etc.
	 *  returning true or false based on whether the reflected meta-object
	 *  is of a particular kind.
	 *
	 *  @see is_a
	 *  @see category
	 */
	inline bool is_meta_X(void) const;
	#endif
#define RUBBER_HELPER_MAKE_MO_IS_X(OBJECT, I, X) \
	inline bool is_meta_## OBJECT(void) const \
	{ \
		return is_a( \
			puddle::meta_object_category::meta_ ## OBJECT ## _tag \
		); \
	}
MIRROR_FOR_EACH_META_OBJECT(RUBBER_HELPER_MAKE_MO_IS_X, _)
#undef RUBBER_HELPER_MAKE_MO_IS_X
};

RUBBER_NAMESPACE_END

#endif //include guard

