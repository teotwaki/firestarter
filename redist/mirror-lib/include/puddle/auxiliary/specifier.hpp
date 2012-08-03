/**
 * .file puddle/auxiliary/specifier.hpp
 * .brief The specifier wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_SPECIFIER_1103160725_HPP
#define PUDDLE_AUXILIARY_SPECIFIER_1103160725_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/tag_wrap.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename Specifier>
struct specifier
 : tag_wrapper<Specifier, mirror::spec__tag>
{
public:
#define PUDDLE_HELPER_MAKE_SPECIFIER_IS_X_FUNC(SPEC, KW, GRP, I, X) \
	static bool is_ ## SPEC (void) \
	{ \
		return tag_wrapper< \
			Specifier, \
			mirror::spec__tag \
		>::is_a(mirror::spec_ ## SPEC ## _tag()); \
	}

MIRROR_FOR_EACH_SPECIFIER(PUDDLE_HELPER_MAKE_SPECIFIER_IS_X_FUNC, _)

#undef PUDDLE_HELPER_MAKE_SPECIFIER_IS_X_FUNC

	static std::string keyword(void)
	{
		return Specifier::keyword();
	}
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

