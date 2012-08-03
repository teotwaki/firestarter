/**
 *  @file mirror/raw_ptr/type_info_cmp.hpp
 *  @brief standard type_info-based type comparator
 *
 *  Copyright 2007-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_RAW_PTR_TYPE_INFO_CMP_1011291729_HPP
#define MIRROR_RAW_PTR_TYPE_INFO_CMP_1011291729_HPP

#include <typeinfo>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// @c std::type_info - based type comparator
class type_info_cmp
{
private:
	// type definition for the type-info type
	typedef ::std::type_info std_ti;

	// pointer to the type info wrapped by this class
	const std_ti* the_ti;

	// getter member function
	const std_ti& get(void) const
	{
		return *the_ti;
	}

	// equality comparison
	static bool equal(
		const type_info_cmp& a,
		const type_info_cmp& b
	)
	{
		return a.get() == b.get();
	}

	// less than comparison
	static bool less_than(
		const type_info_cmp& a,
		const type_info_cmp& b
	)
	{
		return a.get().before(b.get());
	}
public:
	typedef type_info_cmp type;

	template <typename T>
	static const std_ti* create(void)
	{
		return &typeid(typename std::remove_cv<T>::type);
	}

	type_info_cmp(const std_ti* a_ti)
	 : the_ti(a_ti)
	{ }

	/** @name Comparison operators */
	//@{
	/// Equality comparison
	friend bool operator == (
		const type_info_cmp& a,
		const type_info_cmp& b
	) { return equal(a, b); }

	/// Nonequality comparison
	friend bool operator != (
		const type_info_cmp& a,
		const type_info_cmp& b
	) { return !equal(a, b); }

	/// Less-than comparison
	friend bool operator <  (
		const type_info_cmp& a,
		const type_info_cmp& b
	) { return less_than(a, b); }

	/// Greater than comparison
	friend bool operator >  (
		const type_info_cmp& a,
		const type_info_cmp& b
	) { return !equal(a, b) && !less_than(a, b); }

	/// Less-that or equal comparison
	friend bool operator <= (
		const type_info_cmp& a,
		const type_info_cmp& b
	) { return equal(a, b) || less_than(a, b); }

	/// Greater-that or equal comparison
	friend bool operator >= (
		const type_info_cmp& a,
		const type_info_cmp& b
	) { return !less_than(a, b); }
	//@}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif // MIRROR_RAW_PTR_TYPE_INFO_CMP_HPP
