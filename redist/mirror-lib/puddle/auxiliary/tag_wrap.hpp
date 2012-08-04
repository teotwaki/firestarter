/**
 * .file puddle/auxiliary/tag_wrap.hpp
 * .brief The base class for Mirror tag wrappers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_BASE_TAG_1103161035_HPP
#define PUDDLE_AUXILIARY_BASE_TAG_1103161035_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/integer.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename Tag, typename NoneTag>
struct tag_wrapper
{
public:
	template <typename OtherTag>
	static integer<std::integral_constant<
		bool,
		std::is_same<OtherTag, Tag>::value ||
		std::is_base_of<OtherTag, Tag >::value
	> > is_a(OtherTag)
	{
		return integer<std::integral_constant<
			bool,
			std::is_same<OtherTag, Tag>::value ||
			std::is_base_of<OtherTag, Tag >::value
		> >();
	}

	static auto is_none(void) ->
	decltype(is_a(NoneTag()))
	{
		return is_a(NoneTag());
	}
private:
	template <typename OtherTag>
	static auto equal(
		tag_wrapper,
		tag_wrapper<OtherTag, NoneTag>
	) -> decltype(is_a(OtherTag()))
	{
		return is_a(OtherTag());
	}

	static integer<std::false_type>
	less(tag_wrapper, tag_wrapper)
	{
		return integer<std::false_type>();
	}

	template <int A, int B>
	static std::integral_constant<bool, (A < B)>
	_less(
		std::integral_constant<int, A>,
		std::integral_constant<int, B>
	)
	{
		return integer<std::integral_constant<bool, (A < B)> >();
	}


	template <typename OtherTag>
	static auto less(
		tag_wrapper,
		tag_wrapper<OtherTag, NoneTag>
	) -> decltype(_less(Tag::_uid(), OtherTag::_uid()))
	{
		return _less(Tag::_uid(), OtherTag::_uid());
	}
public:
	template <typename X>
	friend auto operator == (tag_wrapper a, X b) ->
	decltype(equal(a, b))
	{
		return equal(a, b);
	}

	template <typename X>
	friend auto operator != (tag_wrapper a, X b) ->
	decltype(!equal(a, b))
	{
		return !equal(a, b);
	}

	template <typename X>
	friend auto operator <= (tag_wrapper a, X b) ->
	decltype(equal(a, b) || less(a, b))
	{
		return equal(a, b) || less(a, b);
	}

	template <typename X>
	friend auto operator >= (tag_wrapper a, X b) ->
	decltype(!less(a, b))
	{
		return !less(a, b);
	}

	template <typename X>
	friend auto operator <  (tag_wrapper a, X b) ->
	decltype(less(a, b))
	{
		return less(a, b);
	}

	template <typename X>
	friend auto operator >  (tag_wrapper a, X b) ->
	decltype(!equal(a, b) && !less(a, b))
	{
		return !equal(a, b) && !less(a, b);
	}

	operator bool (void) const
	{
		return !is_none();
	}

	bool operator ! (void) const
	{
		return is_none();
	}
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

