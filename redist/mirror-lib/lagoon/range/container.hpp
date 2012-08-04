/**
 * @file lagoon/range/container.hpp
 * @brief A range that contains its elements used for example
 * as a base class in several adapters.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_CONTAINER_1011291729_HPP
#define LAGOON_RANGE_CONTAINER_1011291729_HPP

#include <lagoon/utils.hpp>
#include <cassert>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Base class for the container range wrapping the container storing
// the items
template <class Container>
class container_range_wrap
{
protected:
	Container container;

	container_range_wrap(void){ }

	container_range_wrap(container_range_wrap&& other)
	 : container(std::move(other.container))
	{ }

	container_range_wrap(Container&& cntr)
	 : container(std::forward<Container>(cntr))
	{ }

	template <class Iterator>
	container_range_wrap(const Iterator& b, const Iterator& e)
	  : container(b, e)
	{ }
};

// Forward declaration of the container range template
//
// MetaObject - the meta_object type traversed by the range
// ItemContainer - container type used for storing the items from the
//   adapted range in this range
// ContainerFiller - helper class that fills the container with the elements
//   from the adapted range during construction
// ContainerTransform - helper class that transforms the container during
//   construction of container_range
template <
	class Container,
	class ContainerFiller,
	class ContainerTransform
> class container_range;

template <
	typename MetaObject,
	template <class ...> class ContainerTempl,
	class ... P,
	class ContainerFiller,
	class ContainerTransform
> class container_range<
	ContainerTempl<shared<MetaObject>, P...>,
	ContainerFiller,
	ContainerTransform
>: private container_range_wrap<ContainerTempl<shared<MetaObject>, P...> >
 , private ContainerFiller
 , private ContainerTransform
 , public range_base<
	MetaObject,
	typename ContainerTempl<shared<MetaObject>, P...>::const_iterator
>
{
private:
	typedef ContainerTempl<shared<MetaObject>, P...> base_container;
	typedef container_range_wrap<base_container> base_wrapper;
	typedef ContainerFiller base_filler;
	typedef ContainerTransform base_transform;
	typedef range_base<
		MetaObject,
		typename base_container::const_iterator
	> base_range;
	friend class lagoon::aux::std_range_adapt<
		container_range<
			base_container,
			base_filler,
			base_transform
		>
	>;
public:
	template <typename Range, typename TransformParam>
	inline container_range(
		const Range& other_range,
		TransformParam transf_param
	): base_filler(other_range, this->container)
	 , base_transform(transf_param, this->container)
	 , base_range(this->container.begin(), this->container.end())
	{ }

	template <typename StdRange, typename TransformParam>
	inline container_range(
		int,
		const StdRange& other_range,
		TransformParam transf_param
	): base_wrapper(other_range.begin(), other_range.end())
	 , base_transform(transf_param, this->container)
	 , base_range(this->container.begin(), this->container.end())
	{ }
};

template <class Container>
class simple_container_range;

template <
	class MetaObject,
	template <class ...> class ContainerTempl,
	class ... P
> class simple_container_range<
	ContainerTempl<shared<MetaObject>, P...>
>: private container_range_wrap<ContainerTempl<shared<MetaObject>, P...> >
 , public range_base<
	MetaObject,
	typename ContainerTempl<shared<MetaObject>, P...>::const_iterator
>
{
private:
	typedef ContainerTempl<shared<MetaObject>, P...> base_container;
	typedef container_range_wrap<base_container> base_wrapper;
	typedef range_base<
		MetaObject,
		typename base_container::const_iterator
	> base_range;

	friend class lagoon::aux::std_range_adapt<
		simple_container_range<base_container>
	>;
public:
	simple_container_range(base_container&& cntr)
	 : base_wrapper(std::forward<base_container>(cntr))
	 , base_range(this->container.begin(), this->container.end())
	{ }

	simple_container_range(simple_container_range&& other)
	 : base_wrapper(std::move(other))
	 , base_range(this->container.begin(), this->container.end())
	{ }

	simple_container_range(const simple_container_range& other)
	 : base_wrapper(other)
	 , base_range(this->container.begin(), this->container.end())
	{ }
};

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

