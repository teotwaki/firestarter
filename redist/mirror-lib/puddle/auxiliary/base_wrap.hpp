/**
 * .file puddle/auxiliary/base_wrap.hpp
 * .brief The base class for Mirror meta-object wrappers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_BASE_WRAP_1103161035_HPP
#define PUDDLE_AUXILIARY_BASE_WRAP_1103161035_HPP

#include <puddle/auxiliary/fwd.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename MirrorMetaObject>
struct base_wrapper
{
	static MirrorMetaObject mirrored(void)
	{
		return MirrorMetaObject();
	}
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

