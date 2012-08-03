/**
 * @file mirror/meta_prog/identity.hpp
 * @brief Identity template meta-function returning the type passed
 * as argument. Also used in tag dispatching.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_IDENTITY_1011291729_HPP
#define MIRROR_META_PROG_IDENTITY_1011291729_HPP


MIRROR_NAMESPACE_BEGIN
namespace mp {

/// Meta-function returning T unchanged
/** This meta-function is also useful in tag dispatching between
 *  different function overloads.
 *  @tparam T the type to be returned.
 *  @ingroup meta_programming
 */
template <typename T>
struct identity
{
	/// The retulting type
	typedef T type;
};

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

