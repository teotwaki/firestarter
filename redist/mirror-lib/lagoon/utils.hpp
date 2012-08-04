/**
 * @file lagoon/utils.hpp
 * @brief Forward declaration of the meta-level classes reflecting
 * base-level program constructs.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_1011291729_HPP
#define LAGOON_UTILS_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/range/fwd_decl.hpp>
#include <vector>
#include <cassert>

LAGOON_NAMESPACE_BEGIN

/** @defgroup lagoon_support Lagoon - Support classes
 *
 *  Lagoon defines several support classes which are used for
 *  meta-object lifetime management and other similar things.
 */

template <typename MetaObject> class shared;

namespace aux {

// helper function used for getting to the wrapped raw pointer inside
// of a shared<MetaObject> in internal code
// NOTE: this is an implementation detail, never use this function in
// client code
template <typename MetaObject>
MetaObject* _get_mo(const shared<MetaObject>&);

} // namespace aux

/// The smart pointer-like wrapper for MetaObject(s)
/** This wrapper should be used instead or raw pointers, when dealing
 *  with instances of various meta-object types. The meta-object
 *  wrapped by this class is managed by the library and NEVER
 *  should be deleted manually.
 *
 *  @ingroup lagoon_support
 */
template <typename MetaObject>
class shared
{
private:
	MetaObject* mo;

	MetaObject* get(void) const
	{
		return mo;
	}

	friend MetaObject* aux::_get_mo<>(const shared<MetaObject>&);
public:
	inline shared(void)
	 : mo(nullptr)
	{ }

	explicit inline shared(MetaObject* p)
	 : mo(p)
	{ }

	template <class OtherMO>
	inline shared(const shared<OtherMO>& other)
	 : mo(aux::_get_mo(other))
	{ }

	inline operator bool (void) const
	{
		return mo != nullptr;
	}

	inline bool operator ! (void) const
	{
		return mo == nullptr;
	}

	inline MetaObject* operator -> (void) const
	{
		assert(mo != nullptr);
		return mo;
	}

	/// MetaObject interface cast function
	/** This function tries to query the @c MetaObject wrapped
	 *  by this @c shared for the requested @c Interface and returns
	 *  a shared wrapper with this interface.
	 *  If the @c MetaObject wrapped by this @c shared does not
	 *  implement the requested interface the returned wrapper
	 *  is nil.
	 */
	template <class Interface>
	shared<Interface> as(void) const
	{
		// try to cast to the specified interface
#if MIRROR_NO_RTTI
		Interface* imo = static_cast<Interface*>(get()->cast_to(
			categorize_interface((Interface*)nullptr)
		));
#else
		Interface* imo = dynamic_cast<Interface*>(get());
#endif
		// wrap the raw pointer and return it
		return shared<Interface>(imo);
	}

	/// MetaObject interface capability query function
	/** This member function checks if the @c MetaObject wrapper
	 *  by this @c shared wrapper implements the requested @c Interface
	 */
	template <class Interface>
	bool is(void) const
	{
		return mo->self()->is_a(categorize_interface(
			(Interface*)nullptr
		));
	}
};

namespace aux {

template <typename MetaObject>
inline MetaObject* _get_mo(const shared<MetaObject>& shared_mo)
{
	return shared_mo.get();
}

} // namespace aux


/// Trait class returning the meta-object type of a shared 'pointer'
/** This template class can be used to get the type of meta-objects
 *  wrapped by the shared template.
 *
 *  @ingroup lagoon_support
 */
template <class SharedMetaObject>
struct shared_meta_object;

// Implementation of the shared_meta_object trait
template <class MetaObject>
struct shared_meta_object<shared<MetaObject> >
{
	typedef MetaObject type;
};

template <typename MetaObject>
class range;

/// The base template for the implementation of ranges
/** This is a more flexible variant of the range<> template, allowing
 *  to specify the underlying item container to traverse.
 *
 *  @ingroup lagoon_support
 */
template <typename MetaObject, class Iterator>
class range_base
{
protected:
	typedef Iterator iterator;
	iterator current, end;
	friend class lagoon::aux::std_range_adapt<range<MetaObject> >;
public:
	/// Constructor taking two iterators
	/** this constructor should NOT be used directly. Ranges
	 *  are instantiated by Lagoon's internal code.
	 */
	inline range_base(const iterator& b, const iterator& e)
	 : current(b)
	 , end(e)
	{ }

	/// Returns true if the range is empty
	inline bool empty(void) const
	{
		return end == current;
	}

	/// Returns the size of the range i.e. the number of items
	inline size_t size(void) const
	{
		return end - current;
	}

	/// Moves the front of the range one item forward
	void step_front(void)
	{
		assert(!empty());
		++current;
	}

	/// Moves the front of the range n items forward
	/** This member function moves the front of the range @a leap
	 *  items forward. The @a leap parameter may be greater than
	 *  size() resulting in an empty range.
	 *  Returns the number of items actually leaped through.
	 */
	size_t leap_front(size_t leap)
	{
		if(leap > size())
		{
			current = end;
			return size();
		}
		else
		{
			current += leap;
			return leap;
		}
	}

	/// Returns the (pointer to the) item at the front of the range
	const shared<MetaObject>& front(void) const
	{
		assert(!empty());
		return *current;
	}

	/// Returns the (pointer to the) item @c offs steps from the front
	shared<MetaObject> at(size_t offs) const
	{
		assert(offs < size());
		return *(current + offs);
	}

	/// Checks if the front of both ranges points to the same elem
	friend bool same_position(const range_base& a, const range_base& b)
	{
		return a.current == b.current;
	}
};

/// The type of a range of managed pointers to the MetaObject
/** This type should be used when dealing with a set or range of
 *  managed meta-objects. This class allows to do forward traversals
 *  of the range.
 *
 *  @ingroup lagoon_support
 */
template <typename MetaObject>
class range : public range_base<
	MetaObject,
	typename std::vector<shared<MetaObject> >::const_iterator
>
{
private:
	typedef range_base<
		MetaObject,
		typename std::vector<shared<MetaObject> >::const_iterator
	> base_class;
	typedef typename base_class::iterator iterator;
	friend class lagoon::aux::std_range_adapt<range<MetaObject> >;
public:
	// TODO: replace this when inherited constructors become supported
	inline range(const iterator& b, const iterator& e)
	 : base_class(b, e)
	{ }
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Trait class returning the meta-object type of a range
/** This template class can be used to get the type of meta-objects
 *  within the range without the shared<> wrapper template.
 *
 *  @ingroup lagoon_support
 */
template <typename Range>
struct range_meta_object
{
	typedef unspecified type;
};
#else
template <typename Range>
struct range_meta_object
{
private:
	static Range& dummy_range(void);
public:
	typedef typename shared_meta_object<
		decltype(dummy_range().front())
	>::type type;
};
#endif

template <typename MetaObject>
struct range_meta_object<range<MetaObject> >
{
	typedef MetaObject type;
};

LAGOON_NAMESPACE_END

#endif //include guard

