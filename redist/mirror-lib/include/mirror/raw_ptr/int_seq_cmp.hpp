/**
 *  @file mirror/raw_ptr/int_seq_cmp.hpp
 *  @brief Custom type comparator
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_RAW_PTR_INT_SEQ_CMP_1103080409_HPP
#define MIRROR_RAW_PTR_INT_SEQ_CMP_1103080409_HPP

MIRROR_NAMESPACE_BEGIN
namespace aux {

// @c std::type_info - based type comparator
class int_seq_type_cmp
{
private:
	typedef unsigned short id_type;
	id_type the_id;

	// getter member function
	id_type get(void) const
	{
		return the_id;
	}

	static id_type next_id(void)
	{
		static id_type id = 0;
		return ++id;
	}

	template <typename T>
	static id_type do_create(void)
	{
		static id_type id = next_id();
		return id;
	}
public:
	typedef int_seq_type_cmp type;

	template <typename T>
	static id_type create(void)
	{
		return do_create<typename std::remove_cv<T>::type>();
	}

	int_seq_type_cmp(id_type an_id)
	 : the_id(an_id)
	{ }

	/** @name Comparison operators */
	//@{
	/// Equality comparison
	friend bool operator == (
		const int_seq_type_cmp& a,
		const int_seq_type_cmp& b
	) { return a.get() == b.get(); }

	/// Nonequality comparison
	friend bool operator != (
		const int_seq_type_cmp& a,
		const int_seq_type_cmp& b
	) { return a.get() != b.get(); }

	/// Less-than comparison
	friend bool operator <  (
		const int_seq_type_cmp& a,
		const int_seq_type_cmp& b
	) { return a.get() <  b.get(); }

	/// Greater than comparison
	friend bool operator >  (
		const int_seq_type_cmp& a,
		const int_seq_type_cmp& b
	) { return a.get() >  b.get(); }

	/// Less-that or equal comparison
	friend bool operator <= (
		const int_seq_type_cmp& a,
		const int_seq_type_cmp& b
	) { return a.get() <= b.get(); }

	/// Greater-that or equal comparison
	friend bool operator >= (
		const int_seq_type_cmp& a,
		const int_seq_type_cmp& b
	) { return a.get() >= b.get(); }
	//@}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif // MIRROR_RAW_PTR_TYPE_INFO_CMP_HPP
