/**
 * .file puddle/auxiliary/range.hpp
 * .brief The Mirror range wrapper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_RANGE_1103151315_HPP
#define PUDDLE_AUXILIARY_RANGE_1103151315_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/base_wrap.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/for_each.hpp>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename Range>
struct range : base_wrapper<Range>
{
	static integer<
		typename mirror::mp::empty<Range>::type
	> empty(void)
	{
		return integer<
			typename mirror::mp::empty<Range>::type
		>();
	}

	static integer<
		typename mirror::mp::size<Range>::type
	> size(void)
	{
		return integer<
			typename mirror::mp::size<Range>::type
		>();
	}

	static range<
		typename mirror::mp::step_front<Range>::type
	> step_front(void)
	{
		return range<
			typename mirror::mp::step_front<Range>::type
		>();
	}

	static typename wrap<
		typename mirror::mp::front<Range>::type
	>::type front(void)
	{
		return typename wrap<
			typename mirror::mp::front<Range>::type
		>::type();
	}

	template <typename Int, Int Index>
	static typename wrap<
		typename mirror::mp::at_c<Range, Index>::type
	>::type at(integer<std::integral_constant<Int, Index> >)
	{
		return typename wrap<
			typename mirror::mp::at_c<Range, Index>::type
		>::type();
	}

	template <int Index>
	static typename wrap<
		typename mirror::mp::at_c<Range, Index>::type
	>::type at_c(void)
	{
		return typename wrap<
			typename mirror::mp::at_c<Range, Index>::type
		>::type();
	}

	template <template <class> class Wrapper, class MetaObject>
	static auto find(Wrapper<MetaObject>) ->
	range<typename mirror::mp::find<Range, MetaObject>::type>
	{
		return range<
			typename mirror::mp::find<Range, MetaObject>::type
		>();
	}
private:
	template <typename UnaryPredicate>
	struct pred_maker
	{
		template <typename T>
		struct pred
		{
			typedef decltype(UnaryPredicate()(T())) type;
		};
	};
public:
	template <typename UnaryPredicate>
	static auto only_if_(void) ->
	decltype(
		range<
			typename mirror::mp::only_if<
				Range,
				UnaryPredicate
			>::type
		>()
	)
	{
		return range<
			typename mirror::mp::only_if<
				Range,
				UnaryPredicate
			>::type
		>();
	}

	template <typename UnaryPredicate>
	static auto only_if(UnaryPredicate) ->
	decltype(
		range<
			typename mirror::mp::only_if<
				Range,
				typename pred_maker<
					UnaryPredicate
				>::template pred<
					mirror::mp::arg<1>
				>
			>::type
		>()
	)
	{
		return range<
			typename mirror::mp::only_if<
				Range,
				typename pred_maker<
					UnaryPredicate
				>::template pred<
					mirror::mp::arg<1>
				>
			>::type
		>();
	}
private:
	template <typename Functor>
	struct uf_wrapper
	{
		Functor& func;

		template <typename IterInfo>
		void operator()(IterInfo ii) const
		{
			func(
				typename aux::wrap<
					typename IterInfo::type
				>::type(),
				IterInfo::is_first::value,
				IterInfo::is_last::value
			);
		}
	};
public:
	template <typename Functor>
	static void for_each(Functor func)
	{
		uf_wrapper<Functor> helper = {func};
		mirror::mp::for_each_ii<Range>(helper);
	}

private:
	template <typename Functor>
	struct bf_wrapper
	{
		Functor& func;

		bf_wrapper(Functor& _func)
		 : func(_func)
		{ }

		template <typename ValueType, typename IterInfo>
		auto operator()(ValueType value, IterInfo ii) const ->
		decltype(
			func(
				value,
				typename aux::wrap<
					typename IterInfo::type
				>::type(),
				IterInfo::is_first::value,
				IterInfo::is_last::value
			)
		)
		{
			return func(
				value,
				typename aux::wrap<
					typename IterInfo::type
				>::type(),
				IterInfo::is_first::value,
				IterInfo::is_last::value
			);
		}
	};
public:
	template <
		typename ResultType,
		typename Functor,
		typename ValueType = ResultType
	> static ResultType accumulate(
		Functor func,
		ValueType initial = ResultType()
	)
	{
		return mirror::mp::accumulate_ii<Range, ResultType>(
			bf_wrapper<Functor>(func),
			initial
		);
	}
};

template <>
struct range<mirror::mp::empty_range>
 : base_wrapper<mirror::mp::empty_range>
{
	static integer<std::true_type> empty(void)
	{
		return integer<std::true_type>();
	}

	static integer<std::integral_constant<int, 0> > size(void)
	{
		return integer<std::integral_constant<int, 0> >();
	}

	template <template <class> class Wrapper, class MetaObject>
	static range<mirror::mp::empty_range> find(Wrapper<MetaObject>)
	{
		return range<mirror::mp::empty_range>();
	}

	template <typename UnaryPredicate>
	static range only_if(UnaryPredicate)
	{
		return range();
	}

	template <typename Functor>
	static void for_each(Functor func)
	{
	}

	template <typename ResultType, typename Functor, typename ValueType>
	static ResultType accumulate(Functor, ValueType value)
	{
		return value;
	}
};

typedef range<mirror::mp::empty_range> empty_range;

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

