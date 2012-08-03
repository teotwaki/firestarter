/**
 * @file mirror/meta_prog.hpp
 * @brief All-in-one inclusion of all meta-programming utilities
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_1011291729_HPP
#define MIRROR_META_PROG_1011291729_HPP

#include <mirror/meta_prog/forward_decl.hpp>
#include <mirror/meta_prog/nil_type.hpp>
#include <mirror/meta_prog/identity.hpp>
#include <mirror/meta_prog/optional.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/if.hpp>
#include <mirror/meta_prog/not.hpp>
#include <mirror/meta_prog/or.hpp>
#include <mirror/meta_prog/and.hpp>
#include <mirror/meta_prog/lambda.hpp>
#include <mirror/meta_prog/fold.hpp>
#include <mirror/meta_prog/find.hpp>
#include <mirror/meta_prog/before.hpp>
#include <mirror/meta_prog/contains.hpp>
#include <mirror/meta_prog/lacks.hpp>
#include <mirror/meta_prog/concat.hpp>
#include <mirror/meta_prog/append.hpp>
#include <mirror/meta_prog/prepend.hpp>
#include <mirror/meta_prog/transform.hpp>
#include <mirror/meta_prog/only_if.hpp>
#include <mirror/meta_prog/unique.hpp>
#include <mirror/meta_prog/flatten.hpp>
#include <mirror/meta_prog/apply_on_seq_pack.hpp>
#include <mirror/meta_prog/make_int_seq.hpp>
#include <mirror/meta_prog/is_a.hpp>
#include <mirror/meta_prog/is_one_of.hpp>
#include <mirror/meta_prog/as_a.hpp>
//
#include <mirror/meta_prog/for_each.hpp>
#include <mirror/meta_prog/accumulate.hpp>
//
#include <mirror/meta_prog/nil_term_pack.hpp>


MIRROR_NAMESPACE_BEGIN

/** @defgroup meta_programming Mirror - Meta-programming utilities
 *
 *  Mirror provides an extensive set of meta-programming utilities which
 *  can be combined with the meta-objects and intrinsic meta-functions
 *  into complex compile-time algorithms.
 */

/// Contains the meta-programming utilities
namespace mp {

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

