/**
 *  @file mirror/auxiliary/class_layout.hpp
 *  @brief Helpers for class layout generating meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_CLASS_LAYOUT_1011291729_HPP
#define MIRROR_AUX_CLASS_LAYOUT_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/concat.hpp>
#include <mirror/meta_prog/append.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Declaration of the class layout helper template
template <
	class BaseClasses,
	class MetaClass,
	class InheritanceType
> struct class_layout_helper;

// Specialization for virtually inherited classes without any ancestors
template <class MetaClass>
struct class_layout_helper<
	mp::range<>,
	MetaClass,
	spec_virtual_tag
>
{
	// only one virtually inherited class
	typedef mp::range<MetaClass> virt;
	// no classes with regular inheritance
	typedef mp::range<> reg;
};

// Specialization for regularly inherited classes without any ancestors
template <class MetaClass>
struct class_layout_helper<
	mp::range<>,
	MetaClass,
	spec_non_virtual_tag
>
{
	// no classes with virtual inheritance
	typedef mp::range<> virt;
	// only one class with regular inheritance
	typedef mp::range<MetaClass> reg;
};

// Specialization for virtually inherited classes with some ancestors
template <typename ... MetaInheritances, class MetaClass>
struct class_layout_helper<
	mp::range<MetaInheritances...>,
	MetaClass,
	spec_virtual_tag
>
{
	// the virtually inherited base classes
	typedef typename mp::append<
			typename mp::concat<
			typename class_layout_helper<
				typename base_classes<
					typename MetaInheritances::base_class
				>::type,
				typename MetaInheritances::base_class,
				typename MetaInheritances::inheritance_type
			>::virt ...,
			typename class_layout_helper<
				typename base_classes<
					typename MetaInheritances::base_class
				>::type,
				typename MetaInheritances::base_class,
				typename MetaInheritances::inheritance_type
			>::reg ...
		>::type,
		MetaClass
	>::type virt;
	// no base classes with regular inheritance
	typedef mp::range<> reg;
};

// Specialization for classes with regular inheritance and some ancestors
template <typename ... MetaInheritances, class MetaClass>
struct class_layout_helper<
	mp::range<MetaInheritances...>,
	MetaClass,
	spec_non_virtual_tag
>
{
	// the virtually inherited bases from the ancestors
	typedef typename mp::concat<
		typename class_layout_helper<
			typename base_classes<
				typename MetaInheritances::base_class
			>::type,
			typename MetaInheritances::base_class,
			typename MetaInheritances::inheritance_type
		>::virt ...
	>::type virt;

	// the classes with non-virtual inheritance
	typedef typename mp::append<
		typename mp::concat<
			typename class_layout_helper<
				typename base_classes<
					typename MetaInheritances::base_class
				>::type,
				typename MetaInheritances::base_class,
				typename MetaInheritances::inheritance_type
			>::reg ...
		>::type,
		MetaClass
	>::type reg;
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

