/**
 * @file lagoon/range/extract.hpp
 * @brief Extracting only meta-objects with a requested interface
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_EXTRACT_1011291729_HPP
#define LAGOON_RANGE_EXTRACT_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/range/only_if.hpp>
#include <lagoon/range/transform.hpp>

LAGOON_NAMESPACE_BEGIN
namespace aux {

template <class Interface>
struct mo_has_intf
{
	template <class MetaObject>
	inline bool operator()(const MetaObject& metaobject) const
	{
		return metaobject.template is<Interface>();
	}
};

template <class Interface>
struct mo_query_intf
{
	template <class MetaObject>
	inline auto operator()(const MetaObject& metaobject) const ->
		decltype(metaobject.template as<Interface>())
	{
		return metaobject.template as<Interface>();
	}
};

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Extracts only meta-objects with a particular interface
/** Adapts the range so that it contains only meta-objects
 *  implementing the desired @a Interface. The interface of
 *  the meta-objects in the resulting range is cast to @c Interface.
 *
 *  @tparam Interface the desired interface
 *  @param metaobjects the range or shared metaobjects to extract from
 *
 *  @ingroup lagoon_ranges
 */
template <class Interface, class Range>
UnspecifiedRange<Interface> extract(const Range& metaobjects);
#else
template <class Interface, class Range>
auto extract(const Range& metaobjects) ->
decltype(
	transform(
		only_if(
			metaobjects,
			aux::mo_has_intf<Interface>()
		),
		aux::mo_query_intf<Interface>()
	)
)
{
	return transform(
		only_if(
			metaobjects,
			aux::mo_has_intf<Interface>()
		),
		aux::mo_query_intf<Interface>()
	);
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

