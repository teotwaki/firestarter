/**
 * @file mirror/auxiliary/dyn_library.hpp
 * @brief Dynamic library wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_DYN_LIBRARY_1101272106_HPP
#define MIRROR_AUX_DYN_LIBRARY_1101272106_HPP

#include <stdexcept>
#include <cassert>
#include <dlfcn.h>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// TODO: implementations for non-POSIX systems
// or find and use a portable wrapper
class dyn_library
{
private:
	void* handle;
public:
	dyn_library(const dyn_library&) = delete;

	dyn_library(const char* path)
	 : handle(::dlopen(path, RTLD_NOW))
	{
		if(!handle) throw std::runtime_error(::dlerror());
	}

	dyn_library(dyn_library&& tmp)
	 : handle(tmp.handle)
	{
		assert(handle);
		tmp.handle = 0;
	}

	~dyn_library(void)
	{
		if(handle) ::dlclose(handle);
	}

	template <typename Func>
	Func* function(const char* name) const
	{
		assert(handle);
		Func* result = (Func*) ::dlsym(handle, name);
		if(!result) throw std::runtime_error(::dlerror());
		return result;
	}
};


} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

