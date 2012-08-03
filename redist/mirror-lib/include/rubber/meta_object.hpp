/**
 * @file rubber/meta_object.hpp
 * @brief Wrapper for meta-object
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_META_OBJECT_1103151301_HPP
#define RUBBER_META_OBJECT_1103151301_HPP

#include <rubber/config.hpp>
#include <rubber/meta_meta_object.hpp>
#include <mirror/meta_object_tags.hpp>

RUBBER_NAMESPACE_BEGIN

/// General meta-object type-erasure
/** This type wraps types conforming to Mirror's MetaObject concept
 *
 *  @see mirror::MetaObject
 *  @ingroup rubber_type_erased
 */
class meta_object
{
private:
	int _uid;
	aux::return_value<meta_meta_object>::type (*_get_self)(void);
public:
	template <typename MetaObject>
	meta_object(MetaObject mo)
	 : _uid(aux::meta_object_uid(mo))
	 , _get_self(
		&wrap_shaped<
			MIRRORED_META_OBJECT(MetaObject),
			meta_meta_object
		>
	)
	{ }

#if RUBBER_POLYMORPHIC_META_OBJECT
	virtual ~meta_object(void)
	{ }
#endif

	/// Returns a (run-time) unique identifier of the meta-object
	inline int uid(void) const
	{
		return _uid;
	}

	/// Equality comparison
	friend bool operator == (const meta_object& a, const meta_object& b)
	{
		return a.uid() == b.uid();
	}

	/// Inequality comparison
	friend bool operator != (const meta_object& a, const meta_object& b)
	{
		return a.uid() != b.uid();
	}

	/// Less-equal comparison
	friend bool operator <= (const meta_object& a, const meta_object& b)
	{
		return a.uid() <= b.uid();
	}

	/// Greater-equal comparison
	friend bool operator >= (const meta_object& a, const meta_object& b)
	{
		return a.uid() >= b.uid();
	}

	/// Less-than comparison
	friend bool operator <  (const meta_object& a, const meta_object& b)
	{
		return a.uid() <  b.uid();
	}

	/// Greater-than comparison
	friend bool operator >  (const meta_object& a, const meta_object& b)
	{
		return a.uid() >  b.uid();
	}

	/// Returns the meta_meta_object reflecting this meta-object
	aux::return_value<meta_meta_object>::type self(void) const
	{
		assert(_get_self != nullptr);
		return _get_self();
	}

	/// Returns the category of this meta-object
	puddle::meta_object_category category(void) const
	{
		return self().category();
	}

	#ifdef MIRROR_DOCUMENTATION_ONLY
	/// A set of member functions indicating this meta-object's category
	/** This is not a single member-function, it is instead a set of
	 *  functions like is_type, is_namespace, is_variable, etc.
	 *  returning true or false based on whether this meta-object
	 *  is of a particular kind.
	 *
	 *  @see category
	 *  @see as
	 */
	bool is_X(void) const;
	#endif
#define RUBBER_HELPER_MAKE_MO_IS_X(OBJECT, I, X) \
	inline bool is_## OBJECT(void) const \
	{ \
		return self().is_meta_## OBJECT(); \
	}
MIRROR_FOR_EACH_META_OBJECT(RUBBER_HELPER_MAKE_MO_IS_X, _)
#undef RUBBER_HELPER_MAKE_MO_IS_X

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Casts this meta-object to another type-erased meta-object
	/**
	 *  This member-function is available only if the
	 *  #RUBBER_POLYMORPHIC_META_OBJECT preprocessor symbol is
	 *  set to a nonzero integer value.
	 *
	 *  @see self
	 *  @see category
	 *  @see is_X
	 */
	template <typename TypeErased>
	const TypeErased& as(void) const
#endif
#if RUBBER_POLYMORPHIC_META_OBJECT
	template <typename TypeErased>
	const TypeErased& as(void) const
	{
		return dynamic_cast<const TypeErased&>(*this);
	}
#endif
};

RUBBER_NAMESPACE_END

#endif //include guard

