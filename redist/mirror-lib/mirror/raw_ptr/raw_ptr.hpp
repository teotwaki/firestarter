/**
 *  @file mirror/raw_ptr/raw_ptr.hpp
 *  @brief Definition of raw_ptr class for safer working with untyped pointers
 *
 *  Copyright 2007-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef MIRROR_RAW_PTR_RAW_PTR_1011291729_HPP
#define MIRROR_RAW_PTR_RAW_PTR_1011291729_HPP

#include <cstdlib>
#include <memory>
#include <mirror/config.hpp>
#include <mirror/meta_prog/identity.hpp>
#include <mirror/raw_ptr/auditor.hpp>

#if MIRROR_NO_RTTI
#include <mirror/raw_ptr/int_seq_cmp.hpp>
#else
#include <mirror/raw_ptr/type_info_cmp.hpp>
#endif

#include <exception>

MIRROR_NAMESPACE_BEGIN

/** @defgroup raw_ptr Mirror - Raw pointers
 *
 *  Simple typechecking replacement for void* and partial replacement
 *  for static_cast<T>().
 *
 *  The purpose of 'raw' pointers is to provide safer, run-time typechecked
 *  replacement for the void*, where the use of an untyped pointer cannot be
 *  avoided or seems as the best option.
 *  More precisely the raw_ptr is usable in situations, where pointers
 *  to instances are temporarily cast to untyped pointers and then cast back.
 *
 *  As mentioned elsewhere, several reflection facilities use for the purpose
 *  of the base-level-to-meta-level transition a class that is an universal
 *  (and usually also the default) ancestor for all other types or classes.
 *  This is not a very good idea in general and especially not,
 *  if the native types should be reflected too. Thus, in order to refer
 *  to instances of any type the untyped void* can be used. However by using
 *  void* we loose all the information about the type pointed-to.
 *  This is where by using raw_ptr we can gain some of the type-safety back,
 *  if only at the run-time.
 *
 *  The raw pointers are not applicable to situations, where the pointer
 *  is used in operations like bit-block transfer or examination. i.e. where
 *  it is not cast back to the original type, and the block of memory
 *  pointed-to is handled as a sequence of bytes or bits regardless of the real
 *  data type.
 */

namespace aux {

// Forward declaration of raw pointer base class
template <typename BasePtrType>
class raw_ptr_base;

// Specialization for void*
template <>
class raw_ptr_base<void*>
{
protected:
	void* ptr;
public:
	typedef void* pointer_type;

	inline raw_ptr_base(void)
	 : ptr(nullptr)
	{ }

	template <typename T>
	inline raw_ptr_base(T* p)
	 : ptr((void*)p)
	{ }

	template <typename T>
	inline raw_ptr_base(const std::shared_ptr<T>& sp)
	 : ptr((void*)sp.get())
	{ }

	template <typename T>
	inline raw_ptr_base(std::unique_ptr<T>&& sp)
	 : ptr((void*)sp.release())
	{ }

	template <typename T>
	friend inline T* do_raw_cast(
		mp::identity<T*>,
		const raw_ptr_base& rp
	)
	{
		return (T*)rp.ptr;
	}

	template <typename T>
	friend inline const T* do_raw_cast(
		mp::identity<const T*>,
		const raw_ptr_base& rp
	)
	{
		return (const T*)rp.ptr;
	}
};

// Specialization for shared_ptr
template <>
class raw_ptr_base<std::shared_ptr<void> >
{
protected:
	typedef std::shared_ptr<void> pointer_type;

	std::shared_ptr<void> ptr;

	inline raw_ptr_base(void){ }

	template <typename T>
	inline raw_ptr_base(const std::shared_ptr<T>& p)
	 : ptr(p, (void*)p.get())
	{ }

	template <typename T>
	inline raw_ptr_base(std::unique_ptr<T>&& p)
	 : ptr(std::move(p))
	{ }

	template <typename T>
	friend inline std::shared_ptr<T> do_raw_cast(
		mp::identity<std::shared_ptr<T> >,
		const raw_ptr_base& rp
	)
	{
		return std::shared_ptr<T>(
			rp.ptr,
			(T*)rp.ptr.get()
		);
	}
public:
};

// Specialization for unique_ptr
template <>
class raw_ptr_base<std::unique_ptr<void> >
{
protected:
	typedef std::unique_ptr<void> pointer_type;

	std::unique_ptr<void> ptr;

	inline raw_ptr_base(void){ }

	template <typename T>
	inline raw_ptr_base(std::unique_ptr<T>&& p)
	 : ptr((void*)p.release(), p.get_deleter())
	{ }

	// no copying
	raw_ptr_base(const raw_ptr_base& other) = delete;

	inline raw_ptr_base(raw_ptr_base&& other)
	 : ptr(std::move(other.ptr))
	{ }

	template <typename T, typename D>
	friend inline std::unique_ptr<T, D> do_raw_cast(
		mp::identity<std::unique_ptr<T, D> >,
		raw_ptr_base&& rp
	)
	{
		return std::move(std::unique_ptr<T, D>(
			(T*)rp.ptr.release(),
			D()
			//TODO: D(rp.ptr.get_deleter())
		));
	}
public:
};

// Policy doing nothing at a bad cast
struct do_nothing_at_bad_cast_policy
{
	static inline bool bad_cast_handler(void)
	{
		return false;
	}
};

// Policy throwing std::bad_cast when a bad cast happens
struct throw_at_bad_cast_policy
{
	static bool bad_cast_handler(void)
	{
		throw std::bad_cast();
	}
};

// Policy aborting the process when a bad cast happens
struct abort_at_bad_cast_policy
{
	static bool bad_cast_handler(void)
	{
		std::abort();
		return false;
	}
};

} // namespace aux

/// Raw pointer template using a custom type comparator and a bad cast policy
/**
 *  @ingroup raw_ptr
 */
template <
	class Auditor,
	class BadCastPolicy,
	typename BasePtrType
>
class raw_ptr_templ : public aux::raw_ptr_base<BasePtrType>
{
private:
	// helper typedef of the base implementation
	typedef aux::raw_ptr_base<BasePtrType> base_class;

	// The bad cast policy
	BadCastPolicy bad_cast_policy;

	// The pointer cast auditor instance
	Auditor auditor;

	// does checks before the cast and performs some actions if necessary
	template <typename T>
	bool audit_cast(mp::identity<T*>) const
	{
		// if the cast is allowed
		if(auditor.convertible_to(Auditor((T*)nullptr)))
			return true;
		// use the policy's handler function
		return bad_cast_policy.bad_cast_handler();
	}

	template <
		template <typename, typename ... > class SmartPtr,
		typename T,
		typename ... P
	>
	inline bool audit_cast(mp::identity<SmartPtr<T, P...> >) const
	{
		return audit_cast(mp::identity<T*>());
	}
public:
	typedef BasePtrType base_ptr_type;

	inline const Auditor& get_auditor(void) const
	{
		return auditor;
	}

	raw_ptr_templ(void) = default;

	template <typename T>
	explicit raw_ptr_templ(T* ptr)
	 : base_class(ptr)
	 , auditor(ptr)
	{ }

	raw_ptr_templ(const raw_ptr_templ& other)
	 : base_class(other)
	 , auditor(other.auditor)
	{ }

	raw_ptr_templ(raw_ptr_templ&& other)
	 : base_class(std::move(other))
	 , auditor(std::move(other.auditor))
	{ }

	template <
		template <typename, typename ...> class SmartPtr,
		typename T,
		typename ... P
	>
	explicit raw_ptr_templ(const SmartPtr<T, P...>& ptr)
	 : base_class(ptr)
	 , auditor((T*)nullptr)
	{ }

	template <
		template <typename, typename ...> class SmartPtr,
		typename T,
		typename ... P
	>
	explicit raw_ptr_templ(SmartPtr<T, P...>&& ptr)
	 : base_class(std::move(ptr))
	 , auditor((T*)nullptr)
	{ }

	template <class OtherBadCastPolicy, typename OtherBasePtrType>
	explicit raw_ptr_templ(
		const raw_ptr_templ<
			Auditor,
			OtherBadCastPolicy,
			OtherBasePtrType
		>& other
	): base_class(other.get())
	 , auditor(other.get_auditor())
	{ }

	template <typename T>
	raw_ptr_templ& operator = (T* a_ptr)
	{
		*this = base_class(a_ptr);
		auditor = Auditor(a_ptr);
		return *this;
	}

	raw_ptr_templ& operator = (raw_ptr_templ&& other)
	{
		this->ptr = std::move(other.ptr);
		auditor = std::move(other.auditor);
		return *this;
	}

	template <
		template <typename, typename ...> class SmartPtr,
		typename T,
		typename ... P
	>
	raw_ptr_templ& operator = (const SmartPtr<T, P ...>& a_ptr)
	{
		*this = base_class(a_ptr);
		auditor = Auditor((T*)nullptr);
		return *this;
	}

	template <class OtherBadCastPolicy, typename OtherBasePtrType>
	raw_ptr_templ& operator = (
		const raw_ptr_templ<
			Auditor,
			OtherBadCastPolicy,
			OtherBasePtrType
		>& other
	)
	{
		*this = base_class(other.get());
	 	auditor = other.get_auditor();
		return *this;
	}

	// Conversion to bool
	inline operator bool (void) const
	{
		return bool(this->ptr);
	}

	// Logical negation
	inline bool operator !(void) const
	{
		return bool(!this->ptr);
	}

	inline const BasePtrType& get(void) const
	{
		return this->ptr;
	}

	template <typename T>
	friend T audited_raw_cast(
		mp::identity<T> tid,
		const raw_ptr_templ& rp
	)
	{
		if(rp.audit_cast(tid))
			return do_raw_cast(tid, rp);
		else return T();
	}

	template <typename T>
	friend T audited_raw_cast(
		mp::identity<T> tid,
		raw_ptr_templ&& rp
	)
	{
		if(rp.audit_cast(tid))
		{
			return do_raw_cast(
				tid,
				static_cast<raw_ptr_templ&&>(rp)
			);
		}
		else return T();
	}
};

template <
	typename T,
	class Auditor,
	class BadCastPolicy,
	typename BasePtrType
>
inline T raw_cast(raw_ptr_templ<Auditor, BadCastPolicy, BasePtrType>& a_ptr)
{
	return audited_raw_cast(mp::identity<T>(), a_ptr);
}

template <
	typename T,
	class Auditor,
	class BadCastPolicy,
	typename BasePtrType
>
inline T raw_cast(
	const raw_ptr_templ<Auditor, BadCastPolicy, BasePtrType>& a_ptr
)
{
	return audited_raw_cast(mp::identity<T>(), a_ptr);
}

template <
	typename T,
	class Auditor,
	class BadCastPolicy,
	typename BasePtrType
>
inline T raw_cast(raw_ptr_templ<Auditor, BadCastPolicy, BasePtrType>&& a_ptr)
{
	return std::move(audited_raw_cast(
		mp::identity<T>(),
		std::move(a_ptr)
	));
}

// Declare the default type comparator and bad cast policy
#ifndef MIRROR_FAST_RAW_PTR /* Debug mode  */

#if MIRROR_NO_RTTI
typedef aux::int_seq_type_cmp default_type_cmp;
#else
typedef aux::type_info_cmp default_type_cmp;
#endif

typedef aux::abort_at_bad_cast_policy default_bad_cast_policy;

#else /* Not debugging mode */

#if MIRROR_NO_RTTI
typedef aux::int_seq_type_cmp default_type_cmp;
#else
typedef aux::type_info_cmp default_type_cmp;
#endif
typedef aux::do_nothing_at_bad_cast_policy default_bad_cast_policy;

#endif
//
/// The basic raw pointer type
/** This is an instantiation of the raw_ptr_templ template using the default
 *  type comparator, the default on-bad-cast policy and the void* as the
 *  underlying pointer type.
 *
 *  This type should be used when the ownership of the data pointed-to
 *  is managed by the application logic i.e. not by the pointer itself.
 *
 *  @ingroup raw_ptr
 */
typedef raw_ptr_templ<
	aux::ptr_cast_auditor<default_type_cmp>,
	default_bad_cast_policy,
	void*
> raw_ptr;
//
//
/// Raw pointer type with shared-ownership
/** This is an instantiation of the raw_ptr_templ template using the default
 *  type comparator, the default on-bad-cast policy and the shared_ptr<void>
 *  smart pointer as the underlying pointer type.
 *
 *  The ownership and the deallocation of the data pointed-to is managed
 *  by this pointer.
 *
 *  @ingroup raw_ptr
 */
typedef raw_ptr_templ<
	aux::ptr_cast_auditor<default_type_cmp>,
	default_bad_cast_policy,
	std::shared_ptr<void>
> shared_raw_ptr;

//
/// Raw pointer type with unique-ownership
/** This is an instantiation of the raw_ptr_templ template using the default
 *  type comparator, the default on-bad-cast policy and the unique_ptr<void>
 *  smart pointer as the underlying pointer type.
 *
 *  The ownership and the deallocation of the data pointed-to is managed
 *  by this pointer.
 *
 *  @ingroup raw_ptr
 */
typedef raw_ptr_templ<
	aux::ptr_cast_auditor<default_type_cmp>,
	default_bad_cast_policy,
	std::unique_ptr<void>
> unique_raw_ptr;

MIRROR_NAMESPACE_END

#endif // BOOST_RAW_PTR_RAW_PTR_HPP
