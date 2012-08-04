/**
 * @file mirror/utils/some_type.hpp
 * @brief A type erasure similar to boost any
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SOME_TYPE_1113061708_HPP
#define MIRROR_UTILS_SOME_TYPE_1113061708_HPP

#include <mirror/config.hpp>
#include <boost/any.hpp>
#include <type_traits>
#include <stdexcept>

MIRROR_NAMESPACE_BEGIN

/// A type erasure similar to Boost.Any with optional reference semantics
/** The @em some class is a type erasure which can store values
 *  or references to values of other types.
 *  The instances of @c some can be either @em standalone in cases when they
 *  contain the value, or they can refer to external instances. In case
 *  an instance of @c some refers to another instance, this instance must
 *  be kept alive during the lifetime of the type erased @c some instance or
 *  it must be @em detached before the lifetime of the instance referred-to
 *  ends.
 *
 *  @see some::empty()
 *  @see some::standalone()
 *  @see some::detach()
 */
class some
{
private:
	boost::any val_or_ptr;

	boost::any (*copier)(const boost::any&);

	template <typename T>
	static boost::any copy_or_throw(const boost::any& a, std::true_type)
	{
		return boost::any(*boost::any_cast<T*>(a));
	}

	static void do_throw(void)
	{
		throw std::runtime_error("some: Unable to copy stored value");
	}

	template <typename T>
	static boost::any copy_or_throw(const boost::any& a, std::false_type)
	{
		do_throw();
		return boost::any();
	}

	template <typename T>
	static boost::any do_copy(const boost::any& pointer)
	{
		return copy_or_throw<T>(
			pointer,
			typename mirror::is_copy_constructible<T>::type()
		);
	}

	bool const_;
	bool by_ptr;

	bool is_sane(void) const
	{
		return empty() || !by_ptr || (copier != nullptr);
	}

	void do_detach(void)
	{
		assert(copier != nullptr);
		val_or_ptr = copier(val_or_ptr);
		copier = nullptr;
		const_ = false;
		by_ptr = false;
		assert(is_sane());
	}

	some(const boost::any& a)
	 : val_or_ptr(a)
	 , copier(nullptr)
	 , const_(false)
	 , by_ptr(false)
	{
		assert(is_sane());
	}
public:
	/// Constructs an empty standalone instance without any value
	/** The some_cast and as function must not be called on empty
	 *  instances of @c some.
	 *
	 *  @see empty
	 *  @see standalone
	 */
	some(void)
	 : copier(nullptr)
	 , const_(false)
	 , by_ptr(false)
	{
		assert(is_sane());
	}

	/// Constructs a new instance containing the passed @p value
	/** This constructor creates a new standalone instance which
	 *  contains the @c value passed as parameter.
	 *
	 *  @see empty
	 *  @see standalone
	 *  @see is_value
	 *  @see is_reference
	 */
	template <typename T>
	some(T value, std::false_type = std::false_type())
	 : val_or_ptr(value)
	 , copier(nullptr)
	 , const_(std::is_const<T>::value)
	 , by_ptr(false)
	{
		assert(is_sane());
	}

	/// Constructs a new instance referring to the passed @p inst
	/** This constructor creates a new instance attached (referring)
	 *  to the value referred to by the @c inst reference.
	 *  The instance referred-to must be kept alive the whole time
	 *  the newly constructed instance of @c some is used or this
	 *  instance must be @c detached from the instance it refers to.
	 *
	 *  @see empty
	 *  @see standalone
	 *  @see is_const
	 *  @see is_reference
	 *  @see detach
	 */
	template <typename T>
	some(T& inst, std::true_type by_ptr)
	 : val_or_ptr(&const_cast<typename std::remove_cv<T>::type&>(inst))
	 , copier(&do_copy<typename std::remove_cv<T>::type>)
	 , const_(false)
	 , by_ptr(true)
	{
		assert(is_sane());
	}

	/// Constructs a new instance referring to the passed @p inst
	/** This constructor creates a new instance attached (referring)
	 *  to the value referred-to by the @c inst reference.
	 *  The instance referred-to must be kept alive the whole time
	 *  the newly constructed instance of @c some is used or this
	 *  instance must be @c detached from the instance it refers to.
	 *
	 *  @see empty
	 *  @see standalone
	 *  @see is_const
	 *  @see is_reference
	 *  @see detach
	 */
	template <typename T>
	some(const T& inst, std::true_type by_ptr)
	 : val_or_ptr(&const_cast<typename std::remove_cv<T>::type&>(inst))
	 , copier(&do_copy<typename std::remove_cv<T>::type>)
	 , const_(true)
	 , by_ptr(true)
	{
		assert(is_sane());
	}

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Tests if this instance of some is empty
	/** The @c some_cast and @c as member functions must not be used
	 *  on empty instances.
	 *
	 *  @see some_cast
	 *  @see as
	 */
	bool empty(void) const;
#else
	inline auto empty(void) const ->
	decltype(val_or_ptr.empty())
	{
		return val_or_ptr.empty();
	}
#endif

	/// Tests if this instance is standalone or it refers to something
	/** If this instance of @c some contains and owns its value then
	 *  standalone returns true otherwise, if it refers to an external
	 *  instance it returns false. Instances which are not standalone
	 *  can be made standalone by calling the @c detach member function.
	 *  This member function is the right one to test if this refers to
	 *  something, since @c is_value and @c is_reference member functions
	 *  are for other purposes.
	 *
	 *  @see detach
	 *  @see is_value
	 *  @see is_reference
	 */
	inline bool standalone(void) const
	{
		return !by_ptr;
	}

	/// Function that detaches this from the external instace by doing a copy
	/** If this instance is not standalone, then the @c detach member function
	 *  will make a copy of the external instance referred-to by this
	 *  instance and store the copy. If this instance already is standalone
	 *  then detach does not do anything.
	 *
	 *  @see standalone
	 */
	inline some& detach(void)
	{
		if(by_ptr && !empty()) do_detach();
		return *this;
	}

	/// Returns true if the the value referred-to by this instance is const
	/** This member function returns true, if the value referred-to by this
	 *  instance is const.
	 *  If true then the @c some_cast should be done only with const T.
	 *
	 *  @see some_cast
	 *  @see as
	 *  @see detach
	 */
	inline bool is_const(void) const
	{
		return const_;
	}

	/// Returns true if this is a non-constant reference to an external value
	/** This function can be used for testing if this instance of @some refers
	 *  to some non-const external instance which can be modified via this.
	 *  If this owns its value or it refers to a const value it returns false.
	 *
	 *  @see is_value
	 *  @see is_const
	 *  @see empty
	 *  @see standalone
	 *  @see detach
	 *  @see some_cast
	 */
	inline bool is_reference(void) const
	{
		return by_ptr && !const_;
	}

	/// Returns true if this owns its value or refers to a const instance
	/** This function can be used for testing if this instance owns its
	 *  value or it refers to an external instance via a const reference.
	 *  If this member function returns true, then it is necessary to
	 *  test for constness with the @c is_const member function before
	 *  requesting non-const access (for assignement, calling non-const
	 *  member functions, etc). If only read-only access is desired
	 *  then the some_cast should be used with @c const type or the
	 *  @c as member function should be used.
	 *
	 *  @see is_value
	 *  @see is_const
	 *  @see empty
	 *  @see standalone
	 *  @see detach
	 *  @see some_cast
	 *  @see as
	 */
	inline bool is_value(void) const
	{
		return !empty() && !is_reference();
	}

	/// Testing if this stores or refers to a value of a certain type
	/** This member function can be used to test if this instance of
	 *  @c some owns and stores or refers to an external instance
	 *  of the @c T type.
	 *
	 *  @see empty
	 *  @see as
	 *  @see some_cast
	 */
	template <typename T>
	inline bool is(void) const
	{
		return	val_or_ptr.type() == typeid(T*) ||
			val_or_ptr.type() == typeid(T);
	}

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Operator for casting to the stored type
	/** This function can be use to get read-only or read-write access
	 *  to the value stored inside or refered to by this instance of @c some.
	 *  To get read-only access use @c const @c Type as the template
	 *  parameter to get read-write reference to the value use a non-const
	 *  type. Before trying to get access for writing test for constness
	 *  with the @c is_const member function.
	 *
	 *  @see empty
	 *  @see is
	 *  @see is_const
	 *  @see detach
	 *  @see as
	 */
	template <typename T>
	friend T-or-T& some_cast(some& s);
#else
	template <typename T>
	friend typename std::remove_volatile<
		typename std::remove_reference<T>::type
	>::type& some_cast(some& s)
	{
		assert(!s.val_or_ptr.empty());
		assert(s.is_sane());
		typedef typename std::remove_volatile<
			typename std::remove_reference<T>::type
		>::type type;
		typedef typename std::remove_const<type>::type type_;
		bool const_ok = !s.const_ || std::is_const<type>::value;
		if(s.val_or_ptr.type() == typeid(type_*) && const_ok)
			return *boost::any_cast<type_*>(s.val_or_ptr);
		else return boost::any_cast<type&>(s.val_or_ptr);
	}
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Operator for casting to the stored type
	/** This function can be use to get read-only access to the value
	 *  stored inside or refered to by this instance of @c some.
	 *  To get read-only access use @c const @c Type as the template
	 *  parameter.
	 *
	 *  @see empty
	 *  @see is
	 *  @see detach
	 *  @see as
	 */
	template <typename T>
	friend T some_cast(const some& s);
#else
	template <typename T>
	friend const typename std::remove_cv<
		typename std::remove_reference<T>::type
	>::type& some_cast(const some& s)
	{
		assert(!s.val_or_ptr.empty());
		assert(s.is_sane());
		typedef typename std::remove_cv<
			typename std::remove_reference<T>::type
		>::type type;
		if(s.val_or_ptr.type() == typeid(type*))
			return *boost::any_cast<type*>(s.val_or_ptr);
		else return boost::any_cast<const type&>(s.val_or_ptr);
	}
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Function for getting the stored or referred-to value
	/** This member function can be use to get read-only access to the value
	 *  stored inside or refered-to by this instance of @c some.
	 *  You don't need to use the @c const specifier on the template
	 *  parameter in contrast to the some_cast operator.
	 *
	 *  @see empty
	 *  @see is
	 *  @see detach
	 *  @see as
	 */
	template <typename T>
	T as(const some& s) const;
#else
	template <typename T>
	inline const typename std::remove_cv<
		typename std::remove_reference<T>::type
	>::type& as(void) const
	{
		return some_cast<const T>(*this);
	}
#endif
};

template <typename T>
some some_ref(T& ref)
{
	return some(ref, std::true_type());
}

MIRROR_NAMESPACE_END

#endif //include guard

