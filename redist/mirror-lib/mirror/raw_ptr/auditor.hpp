/**
 *  @file mirror/raw_ptr/auditor.hpp
 *  @brief Generic template for type cast auditors
 *
 *  Copyright 2007-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_RAW_PTR_AUDITOR_1011291729_HPP
#define MIRROR_RAW_PTR_AUDITOR_1011291729_HPP

#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

#ifdef MIRROR_FAST_RAW_PTR
template <typename TypeCmp>
class ptr_cast_auditor
{
public:
	ptr_cast_auditor(void) = default;

	template <typename T>
	inline ptr_cast_auditor(T*)
	{ }

	template <typename T>
	inline ptr_cast_auditor& operator = (T*)
	{
		return *this;
	}

	inline bool convertible_to(const ptr_cast_auditor&) const
	{
		return true;
	}
};
#else // ! MIRROR_FAST_RAW_PTR
template <typename TypeCmp>
class ptr_cast_auditor
{
private:
	// the object used for comparing types
	typename TypeCmp::type cmpr;
	// whether the type is const
	bool const_;
	// whether the type is volatile
	bool volatile_;
public:
	// default construction
	ptr_cast_auditor(void)
	 : cmpr(TypeCmp::template create<void>())
	 , const_(false)
	 , volatile_(false)
	{ }

	// initialization from a typed pointer
	template <typename T>
	ptr_cast_auditor(T*)
	 : cmpr(TypeCmp::template create<typename std::remove_cv<T>::type>())
	 , const_(std::is_const<T>::value)
	 , volatile_(std::is_volatile<T>::value)
	{ }

	/// assignment from a typed pointer
	template <typename T>
	ptr_cast_auditor& operator = (T*)
	{
		cmpr = TypeCmp::template create<
			typename std::remove_cv<T>::type
		>();
		const_ = std::is_const<T>::value;
		volatile_ = std::is_volatile<T>::value;
		return *this;
	}

	/** returns true if the type represented by this is convertible
	 *  to type represented by x.
	 */
	bool convertible_to(const ptr_cast_auditor& x) const
	{
		return (this->cmpr == x.cmpr) &&
			(!this->const_ || x.const_) &&
			(!this->volatile_ || x.volatile_);
	}
};
#endif

} // namespace aux
MIRROR_NAMESPACE_END

#endif // MIRROR_RAW_PTR_TYPE_INFO_CMP_HPP
