/**
 * @file mirror/stream/base.hpp
 * @brief Basic definitions for the Mirror's stream facility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_STREAM_BASE_1011291729_HPP
#define MIRROR_STREAM_BASE_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/mirror_fwd.hpp>
#include <iostream>
#include <functional>

MIRROR_NAMESPACE_BEGIN
namespace stream {

template <typename T, typename OutputTag>
class _writer;

template <typename OutputTag>
struct _target;

typedef std::function<void (std::ostream&)> name_proc;

template <typename T, typename OutputTag>
class _wrap
{
private:
	const T& value;
	name_proc namer;
	int level;

	_wrap(const _wrap& other)
	 : value(other.value)
	 , level(other.level)
	{ }
public:
	_wrap(const T& src)
	 : value(src)
	 , level(0)
	{ }

	_wrap(const T& src, const name_proc& src_namer)
	 : value(src)
	 , namer(src_namer)
	 , level(0)
	{ }

	template <typename X>
	_wrap(const T& src, const _wrap<X, OutputTag>& parent)
	 : value(src)
	 , level(parent.depth() + 1)
	{ }

	friend class _target<OutputTag>;

	friend class _writer<T, OutputTag>;

	int depth(void) const
	{
		return level;
	}

	friend inline std::ostream& operator << (std::ostream& out, const _wrap& obj)
	{
		_writer<T, OutputTag> to_stream;
		return obj.namer ?
			to_stream(out, obj, obj.namer) :
			to_stream(out, obj) ;
	}
};

template <typename OutputTag>
struct _target
{
	template <typename T>
	static inline _wrap<T, OutputTag> from(const T& src)
	{
		return _wrap<T, OutputTag>(src);
	}

	template <typename T>
	static inline _wrap<T, OutputTag> from(
		const T& src,
		const name_proc& name
	)
	{
		return _wrap<T, OutputTag>(src, name);
	}

	template <typename T, typename X>
	static inline _wrap<T, OutputTag> from(
		const T& src,
		const _wrap<X, OutputTag>& parent
	)
	{
		return _wrap<T, OutputTag>(src, parent);
	}
};

} // namespace stream
MIRROR_NAMESPACE_END

#endif //include guard

