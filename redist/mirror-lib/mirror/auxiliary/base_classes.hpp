/**
 * @file mirror/auxiliary/base_classes.hpp
 * @brief Internal implementation of inheritance reflection
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_BASE_CLASSES_1011291729_HPP
#define MIRROR_AUX_BASE_CLASSES_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/range.hpp>


MIRROR_NAMESPACE_BEGIN
namespace aux {

// Declaration of the base_classes_helper template (see below)
template <class Class, class Range>
struct base_classes_helper;

// This is a helper used in the implementation of the base_classes template
template <class Class, class ... BaseMetaInheritances>
struct base_classes_helper<Class, mp::range<BaseMetaInheritances...> >
{
	// unpack the instantiations of meta_inheritance
	// into the range
	typedef mirror::mp::range<
		mirror::meta_inheritance<
			Class,
			BaseMetaInheritances
		>...
	> type;
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

