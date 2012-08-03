/**
 * @file mirror/meta_container.hpp
 * @brief Implementation of container registering and reflection
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_CONTAINER_1104181551_HPP
#define MIRROR_META_CONTAINER_1104181551_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/cntnr_kind_tags.hpp>
#include <mirror/cntnr_op_param_tags.hpp>
#include <mirror/preprocessor.hpp>
#include <mirror/auxiliary/scoped_named.hpp>
#include <mirror/auxiliary/containers.hpp>
#include <string>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN

namespace _container {

template <typename Container>
struct _
{
	typedef std::integral_constant<int, 0> cntnr_count;
	typedef non_container_tag kind;
};

template <size_t Size>
struct _array_base
{
	static bool empty(const void*)
	{
		return Size == 0;
	}

	static size_t size_hint(const void*)
	{
		return Size;
	}

	typedef mirror::mp::empty_range inserters;
	typedef mirror::mp::empty_range erasers;
};

template <class ElementType, size_t Size>
struct _<ElementType[Size]>
{
	struct cntnr : _array_base<Size>
	{
		typedef ElementType elem_type;

		typedef mirror::aux::meta_array_trav_fwd<
			ElementType,
			Size
		> default_traversal;

		typedef mirror::mp::range<
			default_traversal
			// TODO: reverse traversal ?
		> traversals;

		typedef mirror::mp::range<
			// TODO: begin/end/at/... ?
		> locators;
	};
	static cntnr _cntnr(std::integral_constant<int, 0>);
	typedef std::integral_constant<int, 1> cntnr_count;
	typedef simple_container_tag kind;
};

} // namespace _container

template <class Container, typename IsSpecial, int Index>
struct meta_container
{
private:
	typedef decltype(
		mirror::_container::_<Container>::_cntnr(
			std::integral_constant<int, Index>()
		)
	) _base_cntnr;

	friend class default_traversal<meta_container>;
	friend class traversals<meta_container>;
	friend class locators<meta_container>;
	friend class inserters<meta_container>;
	friend class erasers<meta_container>;

	// gets the scope for regular (class) containers
	static typename mirror::reflected<Container>::type
	_get_scope(std::false_type);

	// scope for special container types (like arrays)
	static typename mirror::reflected<Container>::type::scope
	_get_scope(std::true_type);
public:
	typedef typename mirror::reflected<
		typename _base_cntnr::elem_type
	>::type element_type;

	typedef decltype(_get_scope(IsSpecial())) scope;

	static bool empty(const Container& container)
	{
		return _base_cntnr::empty(container);
	}

	static auto size_hint(const Container& container) ->
	decltype(_base_cntnr::size_hint(container))
	{
		return _base_cntnr::size_hint(container);
	}

	typedef typename _base_cntnr::default_traversal
		default_traversal;
};

template <class Container>
struct container_kind<meta_type<Container> >
{
	typedef typename mirror::_container::_<
		Container
	>::kind type;
};

template <class Container>
struct container_kind<meta_class<Container> >
 : public container_kind<meta_type<Container> >
{ };

template <class Container>
struct container_kind<meta_enum<Container> >
 : public container_kind<meta_type<Container> >
{ };

template <class Container>
struct containers<meta_type<Container> >
{
	typedef typename mirror::mp::apply_on_seq_pack<
		mirror::aux::cntnr_helper<Container>,
		typename mirror::_container::_<Container>::cntnr_count
	>::type type;
};

template <class Container>
struct containers<meta_class<Container> >
 : public containers<meta_type<Container> >
{ };

template <class Container>
struct containers<meta_enum<Container> >
{
	typedef mirror::mp::empty_range type;
};

template <class Container>
struct all_containers<meta_class<Container> >
{
	// TODO: inherited containers
	typedef typename mirror::mp::apply_on_seq_pack<
		mirror::aux::cntnr_helper<Container>,
		typename mirror::_container::_<Container>::cntnr_count
	>::type type;
};

template <class Container, class IsSpecial, int Index>
struct default_traversal<meta_container<Container, IsSpecial, Index> >
{
	typedef typename meta_container<
		Container,
		IsSpecial,
		Index
	>::_base_cntnr::default_traversal type;
};

template <class Container, class IsSpecial, int Index>
struct traversals<meta_container<Container, IsSpecial, Index> >
{
	typedef typename meta_container<
		Container,
		IsSpecial,
		Index
	>::_base_cntnr::traversals type;
};

template <class Container, class IsSpecial, int Index>
struct locators<meta_container<Container, IsSpecial, Index> >
{
	typedef typename meta_container<
		Container,
		IsSpecial,
		Index
	>::_base_cntnr::locators type;
};

template <class Container, class IsSpecial, int Index>
struct inserters<meta_container<Container, IsSpecial, Index> >
{
	typedef typename meta_container<
		Container,
		IsSpecial,
		Index
	>::_base_cntnr::inserters type;
};

template <class Container, class IsSpecial, int Index>
struct erasers<meta_container<Container, IsSpecial, Index> >
{
	typedef typename meta_container<
		Container,
		IsSpecial,
		Index
	>::_base_cntnr::erasers type;
};

// utilities

/// Structure providing info about a simplified element traversal
/** Instances of this structure are used to provide info about
 *  the currently traversed element to an application specified
 *  functor during a simplified element traversal by the
 *  @c traverse_elements function.
 *
 *  @see MetaType
 *  @see traverse_elements
 */
struct element_traversal_info
{
#ifndef MIRROR_DOCUMNETATION_ONLY
	// Implementation details use the member functions instead
	int _elem_done;
	int _cntnr_idx;
	int _elem_idx;
	bool _first_cntnr;
	bool _last_cntnr;
	bool _first_elem;
	bool _last_elem;
#endif
	/// Returns the count of already processed elements
	/** This function returns the count of element which
	 *  have already been processed during this traversal
	 *  not including the current element. i.e. for the first
	 *  element this value is zero
	 */
	const int elements_done(void) const
	{
		return _elem_done;
	}

	/// Returns the container index
	/** This function returns the index of the container
	 *  that the currently traversed element belongs to.
	 *  This function returns zero for the simple and
	 *  the single-value containers.
	 */
	const int container_index(void) const
	{
		return _cntnr_idx;
	}

	/// Returns true if the element belongs to the first container
	const bool first_container(void) const
	{
		return _first_cntnr;
	}

	/// Returns true if the element belongs to the last container
	const bool last_container(void) const
	{
		return _last_cntnr;
	}

	/// Returns the position of the current element in the container
	const int element_position(void) const
	{
		return _elem_idx;
	}

	/// Returns true if the current element is the first in the container
	const bool first_in_container(void) const
	{
		return _first_elem;
	}

	/// Returns true if the current element is the last in the container
	const bool last_in_container(void) const
	{
		return _last_elem;
	}
};

namespace aux {

template <
	typename Kind,
	typename Container,
	typename ElemFunctor,
	typename EmptyFunctor
>
struct trav_elems_hlpr
{
	Kind& kind_tag;
	int& done;
	int& cntnr_idx;
	Container& cntnr;
	ElemFunctor& elem_func;
	EmptyFunctor& empty_func;

	template <typename IterInfo>
	void operator()(IterInfo) const
	{
		typedef typename IterInfo::type meta_cntnr;
		if(meta_cntnr::empty(cntnr))
		{
			element_traversal_info info = {
				done,
				cntnr_idx,
				-1,
				bool(IterInfo::is_first::value),
				bool(IterInfo::is_last::value),
				false,
				false
			};
			empty_func(kind_tag, info);
		}
		else
		{
			auto tr = meta_cntnr::
				default_traversal::
				start(cntnr);
			int elem_idx = 0;
			while(!tr.done())
			{
				auto loc = tr.front();
				tr.step_front();
				element_traversal_info info = {
					done,
					cntnr_idx,
					elem_idx,
					bool(IterInfo::is_first::value),
					bool(IterInfo::is_last::value),
					bool(elem_idx == 0),
					bool(tr.done())
				};
				elem_func(kind_tag, info, loc.get());
				++elem_idx;
				++done;
			}
		}
		++cntnr_idx;
	}
};

template <
	template <class> class Getter,
	class MetaType,
	typename Container,
	typename ElemFunctor,
	typename EmptyFunctor
> int do_trav_elems(
	MetaType,
	Container& inst,
	ElemFunctor elem_func,
	EmptyFunctor empty_func
)
{
	int done = 0, cntnr_idx = 0;
	typedef typename mirror::container_kind<
		MetaType
	>::type Kind;
	Kind kind;
	trav_elems_hlpr<
		Kind,
		Container,
		ElemFunctor,
		EmptyFunctor
	> hlpr = {
		kind,
		done,
		cntnr_idx,
		inst,
		elem_func,
		empty_func
	};
	mirror::mp::for_each_ii<
		typename Getter<MetaType>::type
	>(hlpr);
	return done;
}

} // namespace aux

template <typename Container, typename ElemFunctor, typename EmptyFunctor>
int traverse_elements(
	meta_type<Container> mt,
	Container& inst,
	ElemFunctor elem_func,
	EmptyFunctor empty_func
)
{
	return mirror::aux::do_trav_elems<
		mirror::containers
	>(mt, inst, elem_func, empty_func);
}

template <typename Container, typename ElemFunctor, typename EmptyFunctor>
int traverse_elements(
	meta_class<Container> mc,
	Container& inst,
	ElemFunctor elem_func,
	EmptyFunctor empty_func
)
{
	return mirror::aux::do_trav_elems<
		mirror::all_containers
	>(mc, inst, elem_func, empty_func);
}

MIRROR_NAMESPACE_END

#endif //include guard

