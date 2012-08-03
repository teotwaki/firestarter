/**
 * @file mirror/meta_prog/nil_term_pack.hpp
 * @brief Utilities for making ranges out of nil-type-terminated
 *  parameter packs.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_NIL_TERM_PACK_1105050854_HPP
#define MIRROR_META_PROG_NIL_TERM_PACK_1105050854_HPP

#include <mirror/meta_prog/not.hpp>
#include <mirror/meta_prog/only_if.hpp>
#include <mirror/meta_prog/lambda.hpp>
#include <mirror/meta_prog/nil_type.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {

template <typename T>
struct is_pack_nil_type
{
	typedef std::false_type type;
};

template <>
struct is_pack_nil_type<nil_t>
{
	typedef std::true_type type;
};

template <typename Range>
struct remove_nil_types
{
	typedef typename mirror::mp::only_if<
		Range,
		not_<is_pack_nil_type<arg<1> > >
	>::type type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

