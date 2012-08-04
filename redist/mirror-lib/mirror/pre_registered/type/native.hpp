/**
 * @file mirror/pre_registered/type/native.hpp
 * @brief Pre-registration of the native C++ types with Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_TYPE_NATIVE_1011291729_HPP
#define MIRROR_PRE_REGISTERED_TYPE_NATIVE_1011291729_HPP

#include <mirror/meta_namespace.hpp>
#include <mirror/meta_class.hpp>
// pointers and references
#include <mirror/pre_registered/type/_ptr_ref.hpp>
// const and volatile types
#include <mirror/pre_registered/type/_const_volatile.hpp>
// array types
#include <mirror/pre_registered/type/_array.hpp>
// function types
#include <mirror/pre_registered/type/_free_fn.hpp>

MIRROR_REG_BEGIN

MIRROR_REG_GLOBAL_SCOPE_TYPE_CTS(void, ('v','o','i','d'))

// helper macro for registering the native C++ types
// besides the basic type information is registered
// the default and copy constructors are also registered
#define MIRROR_REG_NATIVE_TYPE_HELPER(TYPE, SPELLED) \
MIRROR_REG_GLOBAL_SCOPE_TYPE_CTS(TYPE, SPELLED) \
namespace _class { \
template <> \
struct _< TYPE > : _defaults_native< TYPE, mirror::spec_type_tag > \
{ }; \
}

MIRROR_REG_NATIVE_TYPE_HELPER(
	bool,
	('b','o','o','l')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	char,
	('c','h','a','r')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	unsigned char,
	('u','n','s','i','g','n','e','d',' ','c','h','a','r')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	wchar_t,
	('w','c','h','a','r','_','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	short int,
	('s','h','o','r','t',' ','i','n','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	int,
	('i','n','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	long int,
	('l','o','n','g',' ','i','n','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	unsigned short int,
	('u','n','s','i','g','n','e','d',' ','s','h','o','r','t',' ','i','n','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	unsigned int,
	('u','n','s','i','g','n','e','d',' ','i','n','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	unsigned long int,
	('u','n','s','i','g','n','e','d',' ','l','o','n','g',' ','i','n','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	float,
	('f','l','o','a','t')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	double,
	('d','o','u','b','l','e')
)
MIRROR_REG_NATIVE_TYPE_HELPER(
	long double,
	('l','o','n','g',' ','d','o','u','b','l','e')
)

#undef MIRROR_REG_NATIVE_TYPE_HELPER

MIRROR_REG_END

#endif //include guard

