/**
 * @file lagoon/range/chain.hpp
 * @brief Range modification chaining several ranges together
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_CHAIN_1011291729_HPP
#define LAGOON_RANGE_CHAIN_1011291729_HPP

#include <lagoon/range/utils.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN

namespace aux {

// Helper class containing one of the chained ranges
template <size_t Index, class Range>
struct chained_unit
{
	Range range;

	chained_unit(const Range& rng)
	 : range(rng)
	{ }
};

// Tree composing the individual units of a chained range
template <size_t RootIndex, class ValueType, class ... Ranges>
struct chained_unit_tree;

// Leaf of a chain unit tree
template <size_t RootIndex, class ValueType, class Range>
struct chained_unit_tree<RootIndex, ValueType, Range>
 : chained_unit<RootIndex, Range>
{
	enum {unit_count = RootIndex + 1};
	typedef chained_unit<RootIndex, Range> leaf;

	chained_unit_tree(const Range& rng)
	 : leaf(rng)
	{ }

	bool empty(size_t current) const
	{
		assert(current >= RootIndex);
		return leaf::range.empty();
	}

	void skip_empty(size_t& current)
	{
		assert(current >= RootIndex);
		// since this is the last one
		// we don't have to skip anything here
	}

	void step_front(size_t& current)
	{
		assert(current == RootIndex);
		leaf::range.step_front();
		if(leaf::range.empty()) ++current;
	}

	ValueType front(size_t current) const
	{
		assert(current == RootIndex);
		return leaf::range.front();
	}
};

// The implementation of the chained unit tree
template <size_t RootIndex, class ValueType, class Range, class ... Ranges>
struct chained_unit_tree<RootIndex, ValueType, Range, Ranges...>
 : chained_unit<RootIndex, Range>
 , chained_unit_tree<RootIndex + 1, ValueType, Ranges...>
{
	typedef chained_unit<RootIndex, Range> leaf;
	typedef chained_unit_tree<RootIndex + 1, ValueType, Ranges...> tree;

	chained_unit_tree(
		const Range& rng,
		const Ranges&... rngs
	): leaf(rng)
	 , tree(rngs...)
	{ }

	bool empty(size_t current) const
	{
		if(current == RootIndex) return leaf::range.empty();
		else return tree::empty(current);
	}

	void skip_empty(size_t& current)
	{
		assert(current == RootIndex);
		if(leaf::range.empty())
		{
			++current;
			tree::skip_empty(current);
		}
	}

	void step_front(size_t& current)
	{
		if(current == RootIndex)
		{
			leaf::range.step_front();
			if(leaf::range.empty())
			{
				++current;
				tree::skip_empty(current);
			}
		}
		else tree::step_front(current);
	}

	ValueType front(size_t current) const
	{
		if(current == RootIndex)
			return leaf::range.front();
		else return tree::front(current);
	}
};

template <typename ValueType, class ... Ranges>
class chained
 : private chained_unit_tree<0, ValueType, Ranges...>
 , public leaping_base
{
private:
	typedef chained_unit_tree<0, ValueType, Ranges...>
		base_class;

	// The current range unit to be used
	size_t current;
public:
	/// Constructor taking the pack of ranges to be chained
	chained(const Ranges&...rngs)
	 : base_class(rngs...)
	 , current(0)
	{
		base_class::skip_empty(current);
	}

	/// Returns true if the range is empty
	bool empty(void) const
	{
		return base_class::empty(current);
	}

	/// Moves the front of the range one step forward
	void step_front(void)
	{
		assert(!empty());
		base_class::step_front(current);
	}

	size_t leap_front(size_t leap)
	{
		return leaping_base::do_leap_front(*this, leap);
	}

	/// Returns the value at the front of the range
	ValueType front(void) const
	{
		assert(!empty());
		return base_class::front(current);
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Chains the @a ranges into a single range with a common @a ValueType
/** Returns a single range that contains a sequence of elements
 *  from the provided @a ranges. The elements in the individual
 *  ranges must be convertible to @c ValueType. The chained ranges
 *  may be of different types.
 *
 *  @tparam ValueType the common type to be used for the elements
 *          of the chained ranges
 *  @tparam Ranges the pack of range types to be chained
 *  @param ranges the pack of ranges to be chained
 *
 *  @ingroup lagoon_ranges
 */
template <typename ValueType, class ... Ranges>
UnspecifiedRange<ValueType> chain(const Ranges&...ranges);

/// Chains the @a ranges into a single range with a common @a Interface
/** Returns a single range that contains a sequence of shared meta-objects
 *  from the provided @a ranges. The elements in the individual
 *  ranges must have the specified @a Interface.
 *
 *  @tparam Interface the common meta-object iterface to be used for
 *          the elements of the chained ranges
 *  @tparam Ranges the pack of range types to be chained
 *  @param ranges the pack of ranges to be chained
 *
 *  @ingroup lagoon_ranges
 */
template <typename Interface, class ... Ranges>
UnspecifiedRange<shared<Interface> > chain_mo(const Ranges&...ranges);
#else

// chain with arbitrary value type
template <typename ValueType, class ... Ranges>
aux::chained<ValueType, Ranges...> chain(const Ranges&...ranges)
{
	return aux::chained<ValueType, Ranges...>(ranges...);
}

// chain of shared meta-object ranges
template <typename Interface, class ... Ranges>
aux::chained<shared<Interface>, Ranges...> chain_mo(const Ranges&...ranges)
{
	return aux::chained<shared<Interface>, Ranges...>(ranges...);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

