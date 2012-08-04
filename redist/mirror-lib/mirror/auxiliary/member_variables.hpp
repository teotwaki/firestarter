/**
 * @file mirror/auxiliary/member_variables.hpp
 * @brief Internal implementation of member variables
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_MEMBER_VARIABLES_1011291729_HPP
#define MIRROR_AUX_MEMBER_VARIABLES_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>


MIRROR_NAMESPACE_BEGIN
namespace aux {

// Helper template returning the access specifier for the member variable
template <class _, typename AccessType, size_t Index>
struct resof_member_access
{
	typedef AccessType type;
};

// Helper template returning the access specifier for the member variable
/* This is a specialization for the special default tag, which returns
 * the access specifier of the previous member variable
 */
template <class _, int Index>
struct resof_member_access<_, spec___tag, Index>
{
	typedef decltype(_::member_access(
		std::integral_constant<int, Index-1>()
	)) type;
};


// Declaration of the mem_var_helper template (see below)
template <class Class>
struct mem_var_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_member_variable<
				Class,
				Indices
			>...
		> type;
	};
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

