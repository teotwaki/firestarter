/**
 * .file puddle/auxiliary/meta_object.hpp
 * .brief The Mirror meta-object wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_META_OBJECT_1103151330_HPP
#define PUDDLE_AUXILIARY_META_OBJECT_1103151330_HPP

#include <puddle/auxiliary/base_wrap.hpp>
#include <puddle/auxiliary/wrap.hpp>
#include <puddle/auxiliary/integer.hpp>
#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/is_a.hpp>
#include <mirror/meta_meta_object.hpp>
#include <string>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename MetaObject>
struct meta_object : base_wrapper<MetaObject>
{
	friend integer<std::true_type>
	operator == (meta_object, meta_object)
	{
		return integer<std::true_type>();
	}

	template <typename X>
	friend integer<std::false_type>
	operator == (meta_object, meta_object<X>)
	{
		return integer<std::false_type>();
	}

	friend integer<std::false_type>
	operator != (meta_object, meta_object)
	{
		return integer<std::false_type>();
	}

	template <typename X>
	friend integer<std::true_type>
	operator != (meta_object, meta_object<X>)
	{
		return integer<std::true_type>();
	}

	static decltype(mirror::categorize_meta_object(MetaObject()))
	category(void)
	{
		return mirror::categorize_meta_object(MetaObject());
	}

	static typename wrap<
		MIRRORED_META_OBJECT(MetaObject)
	>::type self(void)
	{
		return typename wrap<
			MIRRORED_META_OBJECT(MetaObject)
		>::type();
	}

	template <typename ConceptTag>
	static integer<
		typename mirror::mp:: template is_a<
			MetaObject,
			ConceptTag
		>::type
	> is_a(ConceptTag)
	{
		return integer<
			typename mirror::mp:: template is_a<
				MetaObject,
				ConceptTag
			>::type
		>();
	}

#define PUDDLE_HELPER_MAKE_MO_IS_OBJECT(OBJECT, I, X) \
	static auto is_ ## OBJECT(void) -> \
	decltype(is_a(mirror::meta_## OBJECT ## _tag())) \
	{ \
		return is_a(mirror::meta_## OBJECT ## _tag()); \
	}

MIRROR_FOR_EACH_META_OBJECT(PUDDLE_HELPER_MAKE_MO_IS_OBJECT, _)
#undef PUDDLE_HELPER_MAKE_MO_IS_OBJECT

	static integer<std::false_type> is_instance(void)
	{
		return integer<std::false_type>();
	}
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

