/**
 * @file mirror/auxiliary/enum_values.hpp
 * @brief Internal implementation of enumeration values reflection
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_ENUM_VALUES_1101121507_HPP
#define MIRROR_AUX_ENUM_VALUES_1101121507_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Enum>
struct enum_val_helper
{
	template <int ... Indices>
	struct apply
	{
		typedef mirror::mp::range<
			mirror::meta_enum_value<
				Enum,
				Indices
			>...
		> type;
	};
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

