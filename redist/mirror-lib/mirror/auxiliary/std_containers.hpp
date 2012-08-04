/**
 * @file mirror/auxiliary/std_containers.hpp
 * @brief Internal implementation of standard container reflection
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_STD_CONTAINERS_1104181546_HPP
#define MIRROR_AUX_STD_CONTAINERS_1104181546_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/cntnr_op_param_tags.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/auxiliary/containers.hpp>
#include <mirror/ct_string/basic_string.hpp>

#include <cassert>
#include <tuple>

MIRROR_NAMESPACE_BEGIN
namespace _container {

template <typename Container>
struct _std_base
{
	static bool empty(const Container& container)
	{
		return container.empty();
	}

	static typename Container::size_type
	size_hint(const Container& container)
	{
		return container.size();
	}
};

} // namespace _container

namespace aux {

template <typename Iterator>
class it_val_locator
{
private:
	static it_val_locator& that(void);
	Iterator pos;
	bool valid;
public:
	it_val_locator(Iterator _pos, Iterator _end)
	 : pos(_pos)
	 , valid(_pos != _end)
	{ }

	typedef std::false_type safe;

	typedef typename std::is_const<
		typename std::remove_reference<
			typename std::iterator_traits<
				Iterator
			>::reference
		>::type
	>::type mutating;

	typedef Iterator position_type;

	inline bool dereferencable(void) const
	{
		return valid;
	}

	operator bool (void) const
	{
		return valid;
	}

	bool operator !(void) const
	{
		return !valid;
	}

	auto get(void) const ->
	decltype(*that().pos)
	{
		assert(dereferencable());
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

template <typename Container>
struct meta_std_cntnr_trav_fwd
{
	typedef meta_traversal_tag category;
	typedef mirror::mp::empty_range signature;

	static inline std::string base_name(void)
	{
		return std::string();
	}

	typedef typename mirror::cts::string<>::type static_name;

	typedef it_pair_trvrsl<
		typename Container::iterator
	> traversal_type;

	static traversal_type start(Container& container)
	{
		return traversal_type(
			container.begin(),
			container.end()
		);
	}

	typedef it_pair_trvrsl<
		typename Container::const_iterator
	> const_traversal_type;

	static const_traversal_type start(const Container& container)
	{
		return const_traversal_type(
			container.begin(),
			container.end()
		);
	}
};

template <typename Container>
struct meta_std_cntnr_trav_bwd
{
	typedef meta_traversal_tag category;
	typedef mirror::mp::empty_range signature;

	static inline std::string base_name(void)
	{
		return std::string("reverse", sizeof("reverse")-1);
	}

	typedef it_pair_trvrsl<
		typename Container::reverse_iterator
	> traversal_type;

	static traversal_type start(Container& container)
	{
		return traversal_type(
			container.rbegin(),
			container.rend()
		);
	}

	typedef it_pair_trvrsl<
		typename Container::const_reverse_iterator
	> const_traversal_type;

	static const_traversal_type start(const Container& container)
	{
		return const_traversal_type(
			container.rbegin(),
			container.rend()
		);
	}
};

#define MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(KIND, NAME, SPELLED) \
struct _std_## NAME ## _ ## KIND ## _base \
{ \
	typedef meta_ ## KIND ## _tag category; \
	static std::string base_name(void) \
	{ \
		return std::string( \
			#NAME, \
			sizeof(#NAME) - 1 \
		); \
	} \
	typedef typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type static_name; \
};

#define MIRROR_MAKE_SIMPLE_STD_CONTAINER_LOCATOR(NAME) \
template <class Container> \
struct meta_std_## NAME ## _locator \
 : public  _std_## NAME ## _locator_base \
{ \
	typedef mirror::mp::empty_range signature; \
 \
	typedef mirror::aux::it_val_locator< \
		typename Container::iterator \
	> locator_type; \
 \
	typedef std::tuple<> _param_tuple; \
 \
	static locator_type go_to( \
		Container& container, \
		_param_tuple = _param_tuple() \
	) \
	{ \
		return locator_type( \
			container.NAME(), \
			container.end() \
		); \
	} \
};

#define MIRROR_MAKE_STD_CONTAINER_VALUE_LOCATOR(NAME) \
template <class Container> \
struct meta_std_## NAME ## _val_locator \
 : public  _std_## NAME ## _locator_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_value_param \
	> signature; \
 \
	typedef mirror::aux::it_val_locator< \
		typename Container::iterator \
	> locator_type; \
 \
	typedef std::tuple< \
		const typename Container::value_type& \
	> _param_tuple; \
 \
	static locator_type go_to( \
		Container& container, \
		const _param_tuple& values \
	) \
	{ \
		return locator_type( \
			container.NAME(std::get<0>(values)), \
			container.end() \
		); \
	} \
};

MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	locator,
	begin,
	('b', 'e', 'g', 'i', 'n')
)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_LOCATOR(begin)

MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	locator,
	end,
	('e', 'n', 'd')
)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_LOCATOR(end)

MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	locator,
	lower_bound,
	('l', 'o', 'w', 'e', 'r', '_', 'b', 'o', 'u', 'n', 'd')
)
MIRROR_MAKE_STD_CONTAINER_VALUE_LOCATOR(lower_bound)

MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	locator,
	upper_bound,
	('u', 'p', 'p', 'e', 'r', '_', 'b', 'o', 'u', 'n', 'd')
)
MIRROR_MAKE_STD_CONTAINER_VALUE_LOCATOR(upper_bound)

#define MIRROR_MAKE_SIMPLE_STD_CONTAINER_INSERTER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _inserter \
 : public  _std_## NAME ## _inserter_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_value_param \
	> signature; \
 \
	typedef std::tuple< \
		const typename Container::value_type& \
	> _param_tuple; \
 \
	static void insert( \
		Container& container, \
		const _param_tuple& params \
	) \
	{ \
		container.NAME(std::get<0>(params)); \
	} \
};

#define MIRROR_MAKE_POSITIONAL_STD_CONTAINER_INSERTER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _pos_inserter \
 : public  _std_## NAME ## _inserter_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_location_param, \
		mirror::container_op_value_param \
	> signature; \
 \
	typedef std::tuple< \
		mirror::aux::it_val_locator< \
			typename Container::iterator \
		>, \
		const typename Container::value_type& \
	> _param_tuple; \
 \
	static void insert( \
		Container& container, \
		const _param_tuple& params \
	) \
	{ \
		container.NAME( \
			std::get<0>(params).position(), \
			std::get<1>(params) \
		); \
	} \
};

#define MIRROR_MAKE_POSITIONAL_STD_CONTAINER_MULTI_INSERTER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _pos_multi_inserter \
 : public  _std_## NAME ## _inserter_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_location_param, \
		mirror::container_op_count_param, \
		mirror::container_op_value_param \
	> signature; \
 \
	typedef std::tuple< \
		mirror::aux::it_val_locator< \
			typename Container::iterator \
		>, \
		std::size_t, \
		const typename Container::value_type& \
	> _param_tuple; \
 \
	static void insert( \
		Container& container, \
		const _param_tuple& params \
	) \
	{ \
		container.NAME( \
			std::get<0>(params).position(), \
			std::get<1>(params), \
			std::get<2>(params) \
		); \
	} \
};

MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	inserter,
	insert,
	('i', 'n', 's', 'e', 'r', 't')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	inserter,
	push,
	('p', 'u', 's', 'h')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	inserter,
	push_front,
	('p', 'u', 's', 'h', '_', 'f', 'r', 'o', 'n', 't')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	inserter,
	push_back,
	('p', 'u', 's', 'h', '_', 'b', 'a', 'c', 'k')
)

MIRROR_MAKE_SIMPLE_STD_CONTAINER_INSERTER(insert)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_INSERTER(push)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_INSERTER(push_back)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_INSERTER(push_front)

MIRROR_MAKE_POSITIONAL_STD_CONTAINER_INSERTER(insert)
MIRROR_MAKE_POSITIONAL_STD_CONTAINER_MULTI_INSERTER(insert)

#define MIRROR_MAKE_SIMPLE_STD_CONTAINER_ERASER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _eraser \
 : public  _std_## NAME ## _eraser_base \
{ \
	typedef mirror::mp::empty_range signature; \
 \
	typedef std::tuple<> _param_tuple; \
 \
	static void erase( \
		Container& container, \
		_param_tuple = _param_tuple() \
	) \
	{ \
		container.NAME(); \
	} \
};

#define MIRROR_MAKE_STD_CONTAINER_VALUE_ERASER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _val_eraser \
 : public  _std_## NAME ## _eraser_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_value_param \
	> signature; \
 \
	typedef std::tuple< \
		const typename Container::value_type& \
	> _param_tuple; \
 \
	static void erase( \
		Container& container, \
		const _param_tuple& params \
	) \
	{ \
		container.NAME( \
			std::get<0>(params) \
		); \
	} \
};

#define MIRROR_MAKE_POSITIONAL_STD_CONTAINER_ERASER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _pos_eraser \
 : public  _std_## NAME ## _eraser_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_location_param \
	> signature; \
 \
	typedef std::tuple< \
		mirror::aux::it_val_locator< \
			typename Container::iterator \
		> \
	> _param_tuple; \
 \
	static void erase( \
		Container& container, \
		const _param_tuple& params \
	) \
	{ \
		container.NAME( \
			std::get<0>(params).position() \
		); \
	} \
};

#define MIRROR_MAKE_STD_CONTAINER_RANGE_ERASER(NAME) \
template <class Container> \
struct meta_std_## NAME ## _range_eraser \
 : public  _std_## NAME ## _eraser_base \
{ \
	typedef mirror::mp::range< \
		mirror::container_op_location_param, \
		mirror::container_op_end_location_param \
	> signature; \
 \
	typedef std::tuple< \
		mirror::aux::it_val_locator< \
			typename Container::iterator \
		>, \
		mirror::aux::it_val_locator< \
			typename Container::iterator \
		> \
	> _param_tuple; \
 \
	static void erase( \
		Container& container, \
		const _param_tuple& params \
	) \
	{ \
		container.NAME( \
			std::get<0>(params).position(), \
			std::get<1>(params).position()  \
		); \
	} \
};

MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	eraser,
	clear,
	('c', 'l', 'e', 'a', 'r')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	eraser,
	pop,
	('p', 'o', 'p')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	eraser,
	pop_front,
	('p', 'o', 'p', '_', 'f', 'r', 'o', 'n', 't')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	eraser,
	pop_back,
	('p', 'o', 'p', '_', 'b', 'a', 'c', 'k')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	eraser,
	remove,
	('r', 'e', 'm', 'o', 'v', 'e')
)
MIRROR_HLPR_MAKE_CONTAINER_OPERATION_BASE(
	eraser,
	erase,
	('e', 'r', 'a', 's', 'e')
)

MIRROR_MAKE_SIMPLE_STD_CONTAINER_ERASER(clear)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_ERASER(pop)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_ERASER(pop_front)
MIRROR_MAKE_SIMPLE_STD_CONTAINER_ERASER(pop_back)

MIRROR_MAKE_STD_CONTAINER_VALUE_ERASER(remove)
MIRROR_MAKE_STD_CONTAINER_VALUE_ERASER(erase)

MIRROR_MAKE_POSITIONAL_STD_CONTAINER_ERASER(erase)

MIRROR_MAKE_STD_CONTAINER_RANGE_ERASER(erase)

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

