/**
 * @file mirror/utils/native_types.hpp
 * @brief native types for the utilities provided by Mirror
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_NATIVE_TYPES_1011291729_HPP
#define MIRROR_UTILS_NATIVE_TYPES_1011291729_HPP

#include <mirror/preprocessor.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/contains.hpp>
#include <string>
#include <ctime>

MIRROR_NAMESPACE_BEGIN

// Macro which calls the passed MACRO for each of the native types
#define MIRROR_UTILS_FOR_EACH_NATIVE_TYPE(MACRO, DATA) \
	MACRO(bool, DATA) \
	MACRO(char, DATA) \
	MACRO(short int, DATA) \
	MACRO(int, DATA) \
	MACRO(long int, DATA) \
	MACRO(unsigned short int, DATA) \
	MACRO(unsigned int, DATA) \
	MACRO(unsigned long int, DATA) \
	MACRO(float, DATA) \
	MACRO(double, DATA) \
	MACRO(long double, DATA) \
	MACRO(std::string, DATA) \
	MACRO(std::tm, DATA)

// Macro which expands into a list of types that are considered
// native by the various utilities provided by Mirror including
// the factory generator plugins
#define MIRROR_UTILS_NATIVE_TYPE_LIST() \
	bool, \
	char, \
	short int, \
	int, \
	long int, \
	unsigned short int, \
	unsigned int, \
	unsigned long int, \
	float, \
	double, \
	long double, \
	std::string, \
	std::tm

// Range of types that are considered native by the various utilities
// provided by Mirror including the factory generator plugins
typedef mp::range<MIRROR_UTILS_NATIVE_TYPE_LIST()> util_native_types;

// Template meta-function checking if X is a utility native type
template <typename X>
struct is_util_native_type
 : public mp::contains<util_native_types, X>::type
{ };

MIRROR_NAMESPACE_END

#endif // include guard

