/**
 * @file mirror/meta_meta_object.hpp
 * @brief Reflection of meta-objects
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_META_OBJECT_1011291729_HPP
#define MIRROR_META_META_OBJECT_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_object_tags.hpp>
#include <mirror/preprocessor.hpp>
#include <mirror/auxiliary/scoped_named.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/ct_string/basic_string.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <typename Tag>
struct meta_meta_object_base;

#define MIRROR_HLPR_MAKE_META_OBJECT_NICE_NAME(OBJECT, PREFIX) \
	PREFIX #OBJECT

#define MIRROR_HLPR_META_OBJECT_TAG(OBJECT, X) \
	meta_ ## OBJECT ## _tag

#define MIRROR_HLPR_IMPLEMENT_MMO_NAMES(OBJECT, I, X) \
template <> \
struct meta_meta_object_base<meta_## OBJECT ##_tag> \
{ \
protected: \
	struct ctrt \
	{ \
		MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#OBJECT) \
	}; \
	struct base \
	{ \
		MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS( \
			MIRROR_META_OBJECT_NICE_NAME( \
				OBJECT, \
				MIRROR_HLPR_MAKE_META_OBJECT_NICE_NAME, \
				"Meta" \
			) \
		) \
	};\
public: \
	typedef mirror::mp::range< \
		MIRROR_FOR_EACH_META_OBJECT_BASE( \
			OBJECT, \
			MIRROR_HLPR_META_OBJECT_TAG, \
			_, \
			MIRROR_PP_COMMA \
		) \
	> gens; \
};

MIRROR_FOR_EACH_META_OBJECT(MIRROR_HLPR_IMPLEMENT_MMO_NAMES, _)
#undef MIRROR_HLPR_MAKE_META_OBJECT_NICE_NAME
#undef MIRROR_HLPR_IMPLEMENT_MMO_NAMES

template <typename Range>
struct wrap_mmo_gens;

template <typename ... T>
struct wrap_mmo_gens<mirror::mp::range<T...> >
{
	typedef mirror::mp::range<
		mirror::meta_meta_object<T>...
	> type;
};

} // namespace aux

template <typename MetaObjectCategory>
struct meta_meta_object : aux::meta_meta_object_base<
	MetaObjectCategory
>
{
public:
	// The category of the reflected meta-object
	typedef MetaObjectCategory category;
private:
	typedef aux::meta_meta_object_base< category > base_class;

	static std::string make_ctrt_name(std::string res)
	{
		std::string::iterator i = res.begin(), e = res.end();
		while(i != e)
		{
			if(*i == '_') *i = ' ';
			++i;
		}
		return res;
	}
public:
	typedef typename mirror::cts::string<>::type static_name;

	// Returns the base name of the meta-object
	static std::string base_name(void)
	{
		return std::string(
			base_class::base::name(),
			base_class::base::name_length()
		);
	}

	// Returns the name of the construct reflected by the meta-object
	static std::string construct_name(void)
	{
		static std::string res = make_ctrt_name(
			std::string(
				base_class::ctrt::name(),
				base_class::ctrt::name_length()
			)
		);
		return res;
	}
};

template <typename MetaObjectCategory>
struct generalizations<meta_meta_object<MetaObjectCategory> >
{
	typedef typename mirror::aux::wrap_mmo_gens<
		typename mirror::aux::meta_meta_object_base<
			MetaObjectCategory
		>::gens
	>::type type;
};

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the meta_object passed as the @a META_OBJECT argument
/**
 *  This macro expands into a type conforming to the MetaMetaObject
 *  concept, which provides meta-data about the given Mirror's MetaObject.
 *  The type name passed as the @a META_TYPE argument
 *  must by a valid Mirror's meta-object.
 *
 *  @see mirror::MetaObject
 *  @see mirror::MetaMetaObject
 *  @see lagoon::meta_meta_object
 *  @see lagoon::meta_object::self
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_META_OBJECT(META_OBJECT) mirror::MetaMetaObject
#else
#define MIRRORED_META_OBJECT(META_OBJECT) \
	mirror::meta_meta_object< \
		typename mirror::meta_object_category< META_OBJECT >::type \
	>
#endif


MIRROR_NAMESPACE_END

#endif //include guard

