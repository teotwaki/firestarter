/**
 * @file rubber/range.hpp
 * @brief Type erasure for Mirror's compile-time ranges
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_RANGE_1103151301_HPP
#define RUBBER_RANGE_1103151301_HPP

#include <rubber/config.hpp>
#include <rubber/wrap.hpp>

#include <mirror/meta_prog/range.hpp>

#include <vector>
#include <functional>

RUBBER_NAMESPACE_BEGIN

template <typename TypeErased>
class range;

/// Helper structure used by the range::for_each_ii member function
/** This structure is used by the for_each_ii member function of
 *  the range class to pass information about the progress of the
 *  element traversal to the functor.
 *
 */
struct iteration_info
{
	/// True if this is the first element in the range traversal
	bool is_first;
	/// True if this is the last element in the range traversal
	bool is_last;
};

/// Class wrapping the Mirror's compile time Ranges
/**
 *  @see mirror::Range
 *  @see mirror::mp::range
 *  @ingroup rubber_type_erased
 */
template <typename TypeErased>
class range
{
private:
	typedef typename aux::return_value<TypeErased>::type
	(*member_getter)(void);

	const std::vector<member_getter>& getters;
	typename std::vector<member_getter>::const_iterator pos;
public:
	range(const std::vector<member_getter>& _getters)
	 : getters(_getters)
	 , pos(getters.begin())
	{ }

	/// Returns true if the range is empty
	inline bool empty(void) const
	{
		return getters.end() == pos;
	}

	/// Returns the number of elements in the range
	inline size_t size(void) const
	{
		return getters.end() - pos;
	}

	#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns the element at the front of the range
	TypeErased front(void) const;
	#else
	inline typename aux::return_value<TypeErased>::type front(void) const
	{
		assert(!empty());
		return (*pos)();
	}
	#endif

	#ifdef MIRROR_DOCUMENTATION_ONLY
	/// returns the element at the specifier index in the range
	TypeErased at(size_t index) const;
	#else
	inline typename aux::return_value<TypeErased>::type at(size_t idx) const
	{
		assert(idx < size());
		return (*(pos + idx))();
	}
	#endif

	/// Moves the front of the range one element ahead
	inline void step_front(void)
	{
		assert(!empty());
		++pos;
	}

	/// Executes the specified functor on each element in the range
	void for_each(const std::function<void(const TypeErased&)>& func) const
	{
		if(!empty())
		{
			range tmp = *this;
			while(!tmp.empty())
			{
				func(tmp.front());
				tmp.step_front();
			}
		}
	}

	/// Executes the specified functor on each element with additional info
	/**
	 *  @see iteration_info
	 *  @see for_each
	 */
	void for_each_ii(
		const std::function<
			void(const TypeErased&, const iteration_info&)
		>& func
	) const
	{
		if(!empty())
		{
			auto tmp_pos = pos;
			auto tmp_get = *tmp_pos;
			++tmp_pos;
			struct iteration_info iter_info = {
				true,
				tmp_pos == getters.end()
			};
			func(tmp_get(), iter_info);
			iter_info.is_first = false;
			while(tmp_pos != getters.end())
			{
				tmp_get = *tmp_pos;
				++tmp_pos;
				iter_info.is_last = tmp_pos == getters.end();
				func(tmp_get(), iter_info);
			}
		}
	}
};

template <class TypeErased>
class range_holder
{
private:
	typedef typename aux::return_value<TypeErased>::type
	(*getter)(void);

	std::vector<getter> _getters;

	template <typename ... MetaObjects>
	static std::vector<getter> init_getters(
		mirror::mp::range<MetaObjects...>
	)
	{
		const getter _init[]={&wrap_shaped<MetaObjects, TypeErased>...};
		return std::vector<getter>(_init, _init + sizeof ... (MetaObjects));
	}
public:
	template <template <class> class Getter, class MetaObject>
	range_holder(Getter<MetaObject>)
	 : _getters(
		init_getters(
			typename mirror::mp::as_range<
				Getter<MetaObject>
			>::type()
		)
	)
	{ }

	range_holder(mirror::mp::empty_range)
	{ }

	range<TypeErased> all(void) const
	{
		return range<TypeErased>(_getters);
	}
};

RUBBER_NAMESPACE_END

#endif //include guard

