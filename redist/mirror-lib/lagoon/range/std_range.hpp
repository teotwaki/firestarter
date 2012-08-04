/**
 * @file lagoon/range/std_range.hpp
 * @brief Adaptor turning a Lagoon range to a std range with begin/end
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_STD_RANGE_1011291729_HPP
#define LAGOON_RANGE_STD_RANGE_1011291729_HPP

#include <lagoon/utils.hpp>
#include <lagoon/range/fwd_decl.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN

namespace aux {

// Forward declaration of the std range adaptor
template <class Range>
class std_range_adapt;

// Forward declaration of the std range input iterator adaptor
template <class Range>
class std_input_iter_adapt;

// A helper class used in iterator postincrement and dereference
template <class Range, typename Value>
class std_range_iter_deref
{
private:
	// The value pointed to by the iterator before post-increment
	Value value;

	inline std_range_iter_deref(const Value& val)
	 : value(val)
	{ }

	friend class std_input_iter_adapt<Range>;
public:
	// The only thing this class allows to do is to dereference
	inline const Value& operator * (void) const
	{
		return value;
	}
};

// Generic std input iterator for interating over any Lagoon Range
// Specific cases are handled by specializations of this
// and the std_range_adapt template
template <class Range>
class std_input_iter_adapt
{
private:
	// Helper class holding references to the parent range
	// and to a local copy used for iteration
	class range_holder
	{
	private:
		// Local copy of the range used for iteration
		Range* range;
		// Points to the parent range that instantiated this iterator
		const Range* parent_ref;
	public:
		inline range_holder(void)
		 : range(nullptr)
		 , parent_ref(nullptr)
		{ }

		inline range_holder(const range_holder& other)
		 : range(other.range ? new Range(*other.range): nullptr)
		 , parent_ref(other.parent_ref)
		{ }

		inline range_holder(const Range& rng)
		 : range(new Range(rng))
		 , parent_ref(&rng)
		{ }

		inline range_holder(const Range& rng, int)
		 : range(nullptr)
		 , parent_ref(&rng)
		{ }

		inline ~range_holder(void)
		{
			if(range != nullptr) delete range;
		}

		inline range_holder& operator = (const range_holder& other)
		{
			if(range != nullptr) delete range;
			range = other.range? new Range(*other.range):nullptr;
			parent_ref = other.parent_ref;
		}

		inline bool at_end(void) const
		{
			return (parent_ref == nullptr) ||
				(range == nullptr) ||
				((range != nullptr) && range->empty());
		}

		// checks the range positions for equality
		friend bool equal(
			const range_holder& a,
			const range_holder& b
		)
		{
			// if the ranges don't have the same parent
			// then they can't be equal
			if(a.parent_ref != b.parent_ref) return false;
			// if both iterators are 'non-end'
			if(a.range && b.range)
				// compare their position
				return same_position(*a.range, *b.range);
			// otherwise one of the ranges is an end range
			// and the other must be empty
			else return
		 		(a.range && a.range->empty()) ||
			 	(b.range && b.range->empty());
		}

		inline Range* operator -> (void) const
		{
			return range;
		}
	} range;

	// initialization from a valid range
	inline std_input_iter_adapt(const Range& rng)
	 : range(rng)
	{ }

	// initialization an 'end' iterator from a valid range
	inline std_input_iter_adapt(const Range& rng, int)
	 : range(rng, 0)
	{ }

	// can be initialized only by the std range adaptor
	friend class std_range_adapt<Range>;

	// check if we can dereference the iterator
	inline bool is_dereferencable(void) const
	{
		return !range.at_end();
	}

	static Range& dummy_range(void);
public:
	typedef std::input_iterator_tag iterator_category;
	typedef decltype(dummy_range().front()) value_type;
	typedef const value_type* pointer;
	typedef const value_type& reference;
	typedef size_t difference_type;

	inline std_input_iter_adapt(void) { }

	friend inline bool operator == (
		const std_input_iter_adapt& a,
		const std_input_iter_adapt& b
	)
	{
		return equal(a.range, b.range);
	}

	friend inline bool operator != (
		const std_input_iter_adapt& a,
		const std_input_iter_adapt& b
	)
	{
		return !equal(a.range, b.range);
	}

	inline auto operator * (void) -> decltype(range->front())
	{
		assert(is_dereferencable());
		return range->front();
	}

	inline void operator ++ (void)
	{
		assert(is_dereferencable());
		range->step_front();
	}

	inline auto operator ++ (int) -> std_range_iter_deref<
		Range,
		decltype(range->front())
	>
	{
		assert(is_dereferencable());
		std_range_iter_deref<
			Range,
			decltype(range->front())
		> result(range->front());
		range->step_front();
		return result;
	}
};

template <class Range>
class std_range_adapt
{
private:
	Range range;
public:
	inline std_range_adapt(const Range& rng)
	 : range(rng)
	{ }

	typedef std_input_iter_adapt<Range> const_iterator;

	inline const_iterator begin(void) const
	{
		return const_iterator(range);
	}

	inline const_iterator end(void) const
	{
		return const_iterator(range, 0);
	}
};

template <class MetaObject>
class std_range_adapt<range<MetaObject> >
{
private:
	typedef range<MetaObject> Range;
	Range mo_range;
public:
	inline std_range_adapt(const Range& rng)
	 : mo_range(rng)
	{ }

	typedef typename Range::iterator iterator;

	inline iterator begin(void) const
	{
		return mo_range.current;
	}

	inline iterator end(void) const
	{
		return mo_range.end;
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Adapts a Lagoon's range to have a begin() and end() functions
/** This function transforms the interface of a Lagoon's range
 *  adding two member functions begin() and end() returning standard
 *  const input iterators usable by standard algorithms.
 *
 *  @param range the range to be adapted
 *
 *  @ingroup lagoon_ranges
 */
template <class Range> StdRange make_std_range(Range range);
#else
template <class Range>
inline aux::std_range_adapt<Range> make_std_range(Range range)
{
	return aux::std_range_adapt<Range>(range);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

