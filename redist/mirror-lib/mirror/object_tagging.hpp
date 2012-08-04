/**
 * @file mirror/object_tagging.hpp
 * @brief Implementation of reflected object tagging
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_OBJECT_TAGGING_1011291729_HPP
#define MIRROR_OBJECT_TAGGING_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/auxiliary/named.hpp>
#include <mirror/meta_prog/is_a.hpp>

MIRROR_NAMESPACE_BEGIN

namespace aux {

// Helper class used in the implementation of the tag template class
/* Currently the only purpose of this class is to provide a sequence
 * of identifiers to the derived tag template class instantiations
 */
class tag_base
{
protected:
	static int& id_sequence(void)
	{
		static int current = 0;
		return current;
	}
};

} // namespace aux

/// Template for creating tag types which can be used for object tagging
/** Instantiations of this template have a locally unique integral
 *  identifier, can provide the name of the tag and instances are comparable.
 *  Do not use this class directly, use the @c #MIRROR_REG_OBJECT_TAG
 *  and the @c #MIRROR_OBJECT_TAG macros instead.
 *
 *  @see MIRROR_REG_OBJECT_TAG
 *  @see MIRROR_OBJECT_TAG
 */
template <typename Selector>
class tag : public aux::tag_base
{
public:
	typedef tag type;

	/// Returns the identifier for the instantiation of this template
	/** The identifiers can be used to distinguish between different
	 *  instantiations of this template but are not persistent. This
	 *  meanst that one instantiation can have different identifiers
	 *  in different instances (runs) of the application.
	 */
	static int id(void)
	{
		static const int _id = ++id_sequence();
		return _id;
	}

	/// Returns the name of the object tag
	static std::string name(void)
	{
		return std::string(
			Selector::name(),
			Selector::name_length()
		);
	}

	template <typename OtherSelector>
	friend inline bool operator == (
		const tag<Selector>& a,
		const tag<OtherSelector>& b
	)
	{
		return a.id() == b.id();
	}

	template <typename OtherSelector>
	friend inline bool operator != (
		const tag<Selector>& a,
		const tag<OtherSelector>& b
	)
	{
		return a.id() != b.id();
	}

	template <typename OtherSelector>
	friend inline bool operator <  (
		const tag<Selector>& a,
		const tag<OtherSelector>& b
	)
	{
		return a.id() <  b.id();
	}

	template <typename OtherSelector>
	friend inline bool operator <= (
		const tag<Selector>& a,
		const tag<OtherSelector>& b
	)
	{
		return a.id() <= b.id();
	}

	template <typename OtherSelector>
	friend inline bool operator >  (
		const tag<Selector>& a,
		const tag<OtherSelector>& b
	)
	{
		return a.id() >  b.id();
	}

	template <typename OtherSelector>
	friend inline bool operator >= (
		const tag<Selector>& a,
		const tag<OtherSelector>& b
	)
	{
		return a.id() >= b.id();
	}
};

// helper namespace containing the tag-related declarations
namespace tagsel {

// template class for assigning tags to types
template <typename Type>
struct type_tags
{
	typedef mp::range<> type;
};

} // namespace tagsel

namespace aux {

template <typename Expr, typename IsMetaObject>
struct tags_helper;

} // namespace aux

template <typename Expr>
struct tags : aux::tags_helper<
	Expr,
	typename mp::is_a<
		Expr,
		meta_object_tag
	>::type
>{ };

namespace aux {

template <typename Expr>
struct tags_helper<Expr, std::true_type>
{
	typedef mp::range<> type;
};

template <typename Expr>
struct tags_helper<Expr, std::false_type>
 : public tags<typename Expr::type>
{ };

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// This macro registers a new tag for tagging reflected objects
/** Once registered tags can be used to tag the objects reflected
 *  by Mirror. For example one can assign a single_instance tag
 *  to a class or a unique tag to a class member variable, etc.
 *
 *  @see MIRROR_OBJECT_TAG
 */
#define MIRROR_REG_OBJECT_TAG(TAG)
#else
#define MIRROR_REG_OBJECT_TAG(TAG) \
namespace tagsel { \
struct TAG \
{ \
	MIRROR_IMPLEMENT_META_OBJECT_NAME_FUNCTIONS(#TAG) \
};\
} /* namespace tagsel */
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Expands into an instantiation of the tag template for the TAG
/** This macro expands into an instantiation of the tag template
 *  class for the @a TAG previously registered with the
 *  @c #MIRROR_REG_OBJECT_TAG macro.
 *
 *  @see MIRROR_REG_OBJECT_TAG
 *  @see MIRROR_TAG_TYPE
 */
#define MIRROR_OBJECT_TAG(TAG)
#else
#define MIRROR_OBJECT_TAG(TAG) \
mirror::tag< mirror::tagsel::TAG >
#endif

namespace aux {

// the implementation of the tags template for meta-types
template <typename Type>
struct tags_helper<meta_type<Type>, std::true_type>
{
	typedef typename mp::fold<
		tagsel::type_tags<Type>,
		mp::range<>,
		mp::push_back<
			mp::arg<1>,
			tag<mp::arg<2> >
		>
	>::type type;
};

// implementation of the tags template for meta-classes
template <typename Class>
struct tags_helper<meta_class<Class>, std::true_type>
 : tags_helper<meta_type<Class>, std::true_type>
{ };

} // namespace aux

/// Assigns the list of @a TAGS to the @a TYPE
/** This macro assigns the comma separated list of @a TAGS (enclosed
 *  in parentheses) to the type specified by the @a TYPE argument.
 *
 *  All the tags must be registered with the
 *  @c #MIRROR_REG_OBJECT_TAG macro before they can be used.
 *
 *  @see MIRROR_REG_OBJECT_TAG
 *  @see MIRROR_OBJECT_TAG
 */
#define MIRROR_TAG_TYPE(TYPE, TAGS) \
namespace tagsel { \
template <> \
struct type_tags< TYPE > \
{ \
	typedef mirror::mp::range< \
		MIRROR_PP_EXPAND_ARGS TAGS \
	> type; \
}; \
} /* namespace tagsel */

MIRROR_NAMESPACE_END

#endif //include guard

