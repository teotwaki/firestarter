/**
 * .file puddle/auxiliary/wrap.hpp
 * .brief Implementation of the wrap helper function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_WRAP_1103151308_HPP
#define PUDDLE_AUXILIARY_WRAP_1103151308_HPP

#include <puddle/config.hpp>
#include <puddle/auxiliary/fwd.hpp>
#include <mirror/meta_object_tags.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename MetaObject, typename Tag>
struct do_wrap;

template <typename MetaMetaObject>
struct do_wrap<MetaMetaObject, mirror::meta_meta_object_tag>
{
	typedef meta_meta_object<MetaMetaObject> type;
};

#define PUDDLE_HELPER_SPEC_DO_WRAP(OBJECT, I, X) \
template <typename Meta ## OBJECT> \
struct do_wrap<Meta ## OBJECT, mirror::meta_## OBJECT ##_tag> \
{ \
	typedef meta_## OBJECT<Meta ## OBJECT> type; \
};

MIRROR_FOR_EACH_META_OBJECT(PUDDLE_HELPER_SPEC_DO_WRAP, _)
#undef PUDDLE_HELPER_SPEC_DO_WRAP

template <typename MetaObject>
struct wrap
{
	wrap(void) = delete;
	wrap(const wrap&) = delete;

	typedef typename do_wrap<
		MetaObject,
		typename mirror::meta_object_category<MetaObject>::type
	>::type type;
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

