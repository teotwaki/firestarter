/**
 * @file mirror/meta_prog/flatten.hpp
 * @brief The flatten algorithm
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_FLATTEN_1104130956_HPP
#define MIRROR_META_PROG_FLATTEN_1104130956_HPP

#include <mirror/config.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/concat.hpp>
#include <mirror/meta_prog/prepend.hpp>
#include <mirror/meta_prog/can_be_called.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

template <class MetaObject, template <class> class Getter>
struct flatten;

namespace aux {
template <typename Range, template <class> class Getter>
struct fltn_hlpr;

template <typename ... MetaObject, template <class> class Getter>
struct fltn_hlpr<range<MetaObject...>, Getter>
{
	typedef typename mirror::mp::concat<
		typename mirror::mp::flatten<
			MetaObject,
			Getter
		>::type...
	>::type type;
};

template <typename X, template <class> class Getter, typename Bool>
struct fltn_get;

template <typename X, template <class> class Getter>
struct fltn_get<X, Getter, std::false_type>
{
	typedef mirror::mp::empty_range type;
};

template <typename MetaObject, template <class> class Getter>
struct fltn_get<MetaObject, Getter, std::true_type>
{
	typedef typename Getter<MetaObject>::type type;
};

} // namespace aux

template <class MetaObject, template <class> class Getter>
struct flatten
{
	typedef typename prepend<
		MetaObject,
		typename mirror::mp::aux::fltn_hlpr<
			typename mirror::mp::aux::fltn_get<
				MetaObject,
				Getter,
				typename can_be_called<Getter<MetaObject> >::type
			>::type,
			Getter
		>::type
	>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

