/**
 * @file mirror/meta_prog/can_be_called.hpp
 * @brief Boolean meta-function checking if an intrinsic meta-function can
 * be called on a specific MetaObject
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_CAN_BE_CALLED_1102211340_HPP
#define MIRROR_META_PROG_CAN_BE_CALLED_1102211340_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/is_a.hpp>

MIRROR_NAMESPACE_BEGIN

template <template <class> class Getter>
struct required_param_of;

namespace mp {

template <typename Expr>
struct can_be_called
{
	typedef typename can_be_called<
		typename Expr::type
	>::type type;
};

template <template <class> class MetaFn, typename Param>
struct can_be_called<MetaFn<Param> >
{
	typedef typename is_a<
		Param,
		typename mirror::required_param_of<MetaFn>::type
	>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

