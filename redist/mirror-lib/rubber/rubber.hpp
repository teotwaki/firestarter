/**
 * @file rubber/rubber.hpp
 * @brief All in one include
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_RUBBER_1103151301_HPP
#define RUBBER_RUBBER_1103151301_HPP

#include <rubber/config.hpp>
#include <rubber/wrap.hpp>
#include <rubber/range.hpp>
#include <rubber/specifiers.hpp>
#include <rubber/meta_objects.hpp>
#include <puddle/auxiliary/fwd.hpp>

#include <functional>

/** @mainpage Mirror C++ reflection library - Rubber type-erasure utility
 *
 *  @image html  rubber.png
 *  @image latex rubber.png
 *
 *  Copyright &copy; 2006-2011 Matus Chochlik.
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 *  This is a reference manual generated by @em Doxygen, for the Rubber
 *  type-erasure utility, which is part of the Mirror reflection library.
 *
 *  Other resources and documentation for Mirror and the related libraries and utilities
 *  can also be found on the project <A HREF="../../../index.html">home page</A>.
 */

/// The Rubber type-erasure utility for the Mirror meta-objects is defined in this namespace
/** The Rubber is a type-erasure utility which is built on top of the compile-time
 *  meta-objects provided by Mirror.
 */
RUBBER_NAMESPACE_BEGIN

template <class MirrorMetaObject>
auto erase_type(void) ->
decltype(wrap<MirrorMetaObject>())
{
	return wrap<MirrorMetaObject>();
}

template <class MirrorMetaObject>
auto erase_type(const puddle::aux::meta_type<MirrorMetaObject>& mo) ->
decltype(wrap<MirrorMetaObject>())
{
	return wrap<MirrorMetaObject>();
}

namespace aux {

template <typename ... Wrappers>
class eraser_base;

template <>
class eraser_base<>
{
public:
	template <typename MetaObject>
	void call(const MetaObject& mo) const
	{ }
};

template <typename Wrapper, typename ... Wrappers>
class eraser_base<Wrapper, Wrappers ...>
{
private:
	typedef std::function<void(const Wrapper&)> functor_type;
	functor_type functor;
	eraser_base<Wrappers...> base;

	template <typename MetaObject>
	void do_call(const MetaObject& mo, std::false_type) const
	{
		base.call(mo);
	}

	template <typename MetaObject>
	void do_call(const MetaObject& mo, std::true_type) const
	{
		functor(wrap_into<Wrapper>(mo));
	}
public:
	eraser_base(
		const functor_type& func,
		const std::function<void(const Wrappers&)>& ... fns
	): functor(func)
	 , base(fns ...)
	{ }

	template <typename MetaObject>
	void call(const MetaObject& mo) const
	{
		typename mirror::mp::is_a<
			MetaObject,
			typename tag_by_wrapper<Wrapper>::type
		>::type selector;
		do_call(mo, selector);
	}
};

} // namespace aux

template <typename ... Wrappers>
class eraser
{
private:
	aux::eraser_base<Wrappers...> base;
public:
	eraser(const std::function<void(const Wrappers&)>& ... fns)
	 : base(fns...)
	{ }

	template <typename MetaObject>
	void operator()(MetaObject mo) const
	{
		base.call(mo);
	}
};

RUBBER_NAMESPACE_END

#endif //include guard
