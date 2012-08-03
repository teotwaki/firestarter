/**
 * @file mirror/auxiliary/containers.hpp
 * @brief Internal implementation of container reflection helpers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_CONTAINERS_1104181546_HPP
#define MIRROR_AUX_CONTAINERS_1104181546_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/ct_string/basic_string.hpp>
#include <type_traits>
#include <iterator>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Container>
struct cntnr_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_container<
				Container,
				typename std::is_array<
					Container
				>::type,
				Indices
			>...
		> type;
	};
};

template <typename Iterator>
class it_locator
{
private:
	static it_locator& that(void);
	Iterator pos;
public:
	it_locator(Iterator _pos)
	 : pos(_pos)
	{ }

	typedef std::true_type safe;

	typedef typename std::is_const<
		typename std::remove_reference<
			typename std::iterator_traits<
				Iterator
			>::reference
		>::type
	>::type mutating;

	typedef Iterator position_type;

	// TODO: constexpr
	static inline bool dereferencable(void)
	{
		return true;
	}

	operator bool (void) const
	{
		return true;
	}

	bool operator !(void) const
	{
		return false;
	}

	auto get(void) const ->
	decltype(*that().pos)
	{
		return *pos;
	}
private:
	void do_set(
		const typename std::iterator_traits<
			Iterator
		>::value_type& value,
		std::false_type
	) const
	{ }

	void do_set(
		const typename std::iterator_traits<
			Iterator
		>::value_type& value,
		std::true_type
	) const
	{
		assert(dereferencable());
		*pos = value;
	}
public:
	void set(
		const typename std::iterator_traits<
			Iterator
		>::value_type& value
	) const
	{
		do_set(value, mutating());
	}

	Iterator position(void) const
	{
		return pos;
	}
};


template <typename Iterator>
class it_pair_trvrsl
{
private:
	Iterator cur;
	Iterator end;
public:
	it_pair_trvrsl(Iterator _bgn, Iterator _end)
	 : cur(_bgn)
	 , end(_end)
	{ }

	bool done(void) const
	{
		return cur == end;
	}

	bool empty(void) const
	{
		return done();
	}

	void step_front(void)
	{
		assert(!empty());
		++cur;
	}

	it_locator<Iterator> front(void) const
	{
		assert(!empty());
		return it_locator<Iterator>(cur);
	}
};


template <typename ElementType, size_t Size>
struct meta_array_trav_fwd
{
	typedef meta_traversal_tag category;
	typedef mirror::mp::empty_range signature;

	static inline std::string base_name(void)
	{
		return std::string();
	}

	typedef typename mirror::cts::string<>::type static_name;

	typedef it_pair_trvrsl<ElementType*> traversal_type;

	static traversal_type start(ElementType* array)
	{
		return traversal_type(array, array + Size);
	}

	typedef it_pair_trvrsl<ElementType const*> const_traversal_type;

	static const_traversal_type start(const ElementType* array)
	{
		return const_traversal_type(array, array + Size);
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

