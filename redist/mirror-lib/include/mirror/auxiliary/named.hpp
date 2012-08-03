/**
 * @file mirror/auxiliary/named.hpp
 * @brief Internal implementation details of named objects
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_NAMED_1011291729_HPP
#define MIRROR_AUX_NAMED_1011291729_HPP

#include <mirror/mirror_fwd.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// Macro expanding into the overloads of name functions
#define MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(NAME_STR) \
	static const char* name(void) { return NAME_STR; } \
	static size_t name_length(void) { return sizeof(NAME_STR) - 1; }

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

