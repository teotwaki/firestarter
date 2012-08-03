/**
 * @file lagoon/range/flatten.hpp
 * @brief Functions returning all members of a scope including nested members
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_RANGE_FLATTEN_1011291729_HPP
#define LAGOON_RANGE_FLATTEN_1011291729_HPP

#include <lagoon/range/extract.hpp>
#include <lagoon/range/link.hpp>
#include <lagoon/range/container.hpp>
#include <lagoon/range/for_each.hpp>
#include <list>

LAGOON_NAMESPACE_BEGIN


namespace aux {

void flatten_scope(
	linked<range<meta_named_scoped_object> >& dest,
	const shared<meta_scope>& scope
)
{
	dest.append(scope->members());
	for_each(
		extract<meta_scope>(scope->members()),
		[&dest](const shared<meta_scope>& member_scope) -> void
		{
			flatten_scope(dest, member_scope);
		}
	);
}

void flatten_in_order(
	std::list<shared<meta_named_scoped_object> >& dest,
	const shared<meta_scope>& scope
)
{
	for_each(
		scope->members(),
		[&dest](const shared<meta_named_scoped_object>& member) -> void
		{
			dest.push_back(member);
			if(member.is<meta_scope>())
				flatten_in_order(dest,member.as<meta_scope>());
		}
	);
}

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Get a range of all members of a scope including nested members
/** This function returns a range of all members, including nested, of the
 *  scope passed as argument. No particular order of traversal is to be
 *  expected.
 *
 *  @param scope the scope to be flattened
 *
 *  @ingroup lagoon_ranges
 */
UnspecifiedRange<shared<meta_named_scoped_object> >
flatten(const shared<meta_scope>& scope);

/// Get a range of all members of a scope including nested members
/** This function returns a range of all members, including nested, of the
 *  scope passed as argument. The members are traversed in the same order
 *  in which they were registered and nested members of a scope are
 *  ordered right after their parent scope.
 *
 *  @param scope the scope to be flattened
 *
 *  @ingroup lagoon_ranges
 */
UnspecifiedRange<shared<meta_named_scoped_object> >
flatten_in_order(const shared<meta_scope>& scope);
#else
inline aux::linked<range<meta_named_scoped_object> >
flatten(const shared<meta_scope>& scope)
{
	aux::linked<range<meta_named_scoped_object> > result;
	aux::flatten_scope(result, scope);
	return result;
}

inline aux::simple_container_range<
	std::list<shared<meta_named_scoped_object> >
> flatten_in_order(const shared<meta_scope>& scope)
{
	std::list<shared<meta_named_scoped_object> > members;
	aux::flatten_in_order(members, scope);
	return aux::simple_container_range<
		std::list<shared<meta_named_scoped_object> >
	>(std::move(members));
}
#endif

LAGOON_NAMESPACE_END

#endif //include guard

