/**
 *  @file mirror/auxiliary/calling_convention.hpp
 *  @brief Calling convention tag type
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_CALLING_CONVENTION_1011291729_HPP
#define MIRROR_AUX_CALLING_CONVENTION_1011291729_HPP

#include <string>

MIRROR_NAMESPACE_BEGIN
namespace calling_convention {

#define MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(CC) \
	struct __##CC##_ \
	{ \
		static std::string name(void) \
		{ \
			return std::string( \
				"__" #CC, \
				sizeof("__" #CC) - 1 \
			);\
		} \
	};

	MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(cdecl)
	MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(stdcall)
	MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER(pascal)

	struct __default_
	{
		static inline std::string name(void)
		{
			return std::string();
		}
	};

// undefine the helper macros
#undef MIRROR_IMPLEMENT_CALLING_CONVENTION_HELPER

} // namespace calling_convention
MIRROR_NAMESPACE_END

#endif //include guard

