/**
 * @file rubber/wrap.hpp
 * @brief Compile-time meta-object wrappers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_WRAP_1103151301_HPP
#define RUBBER_WRAP_1103151301_HPP

#include <rubber/config.hpp>
#include <puddle/categories.hpp>

RUBBER_NAMESPACE_BEGIN

template <typename Tag>
struct wrapper_by_tag;

template <typename Wrapper>
struct tag_by_wrapper;

class meta_meta_object;

template <>
struct wrapper_by_tag<mirror::meta_meta_object_tag>
{
	typedef meta_meta_object type;
};

template <>
struct tag_by_wrapper<meta_meta_object>
{
	typedef mirror::meta_meta_object_tag type;
};

#define RUBBER_HELPER_MAKE_TAG_TO_META_OBJECT_MAP(OBJECT, I, X) \
class meta_ ## OBJECT; \
template <> \
struct wrapper_by_tag<mirror::meta_ ## OBJECT ## _tag> \
{ \
	typedef meta_ ## OBJECT type; \
}; \
template <> \
struct tag_by_wrapper<meta_ ## OBJECT> \
{ \
	typedef mirror::meta_ ## OBJECT ## _tag type; \
}; \

MIRROR_FOR_EACH_META_OBJECT(RUBBER_HELPER_MAKE_TAG_TO_META_OBJECT_MAP, _)
#undef RUBBER_HELPER_MAKE_TAG_TO_META_OBJECT_MAP

template <typename Wrapper>
puddle::meta_object_category categorize_wrapper(Wrapper*)
{
	return puddle::get_category(
		typename tag_by_wrapper<Wrapper>::type()
	);
}

template <class TypeErased, class MirrorMetaObject>
inline typename aux::return_value<TypeErased>::type
wrap_into(const MirrorMetaObject& mo)
{
#if RUBBER_POLYMORPHIC_META_OBJECT
	static TypeErased wrapped(mo);
	return wrapped;
#else
	return TypeErased(mo);
#endif
}

template <class MetaObjectTag, class MirrorMetaObject>
inline typename aux::return_value<
	typename wrapper_by_tag<
		MetaObjectTag
	>::type
>::type wrap_by_tag(const MirrorMetaObject& mo)
{
	return wrap_into<
		typename wrapper_by_tag<
			MetaObjectTag
		>::type
	>(mo);
}

template <class MirrorMetaObject>
inline typename aux::return_value<
	typename wrapper_by_tag<
		typename mirror::meta_object_category<
			MirrorMetaObject
		>::type
	>::type
>::type wrap(void)
{
	return wrap_by_tag<
		typename mirror::meta_object_category<
			MirrorMetaObject
		>::type
	>(MirrorMetaObject());
}

template <class MirrorMetaObject, class TypeErased>
inline typename aux::return_value<TypeErased>::type wrap_shaped(void)
{
#if RUBBER_POLYMORPHIC_META_OBJECT
	return wrap<MirrorMetaObject>();
#else
	return wrap_into<TypeErased>(MirrorMetaObject());
#endif
}

RUBBER_NAMESPACE_END

#endif //include guard

