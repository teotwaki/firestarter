/**
 * .file puddle/auxiliary/meta_meta_instance.hpp
 * .brief Meta-instance implementation
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_META_INSTANCE_1103151330_HPP
#define PUDDLE_AUXILIARY_META_INSTANCE_1103151330_HPP

#include <puddle/meta_object_tags.hpp>
#include <puddle/auxiliary/base_wrap.hpp>
#include <puddle/auxiliary/wrap.hpp>
#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/is_a.hpp>
#include <mirror/meta_meta_object.hpp>
#include <string>
#include <type_traits>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <class ParentMetaInstance, class MetaMemberVariableRange>
struct meta_attribute_range;

struct meta_inst_base
{
	// TODO: override this in derived classes
	static meta_instance_tag category(void)
	{
		return meta_instance_tag();
	}

	struct _meta_meta_instance
	{
		static std::string base_name(void)
		{
			return "instance";
		}
	};

	static _meta_meta_instance self(void)
	{
		return _meta_meta_instance();
	}

	template <typename ConceptTag>
	static integer<std::false_type> is_a(ConceptTag)
	{
		return integer<std::false_type>();
	}

	static integer<std::true_type> is_a(meta_instance_tag)
	{
		return integer<std::true_type>();
	}

#define PUDDLE_HELPER_MAKE_MI_IS_OBJECT(OBJECT, I, X) \
	static integer<std::false_type> is_ ## OBJECT(void) \
	{ \
		return integer<std::false_type>(); \
	}

MIRROR_FOR_EACH_META_OBJECT(PUDDLE_HELPER_MAKE_MI_IS_OBJECT, _)
#undef PUDDLE_HELPER_MAKE_MI_IS_OBJECT

	static integer<std::true_type> is_instance(void)
	{
		return integer<std::true_type>();
	}
};

template <class MetaFreeVariable>
struct meta_free_var_inst_type : meta_inst_base
{
	static meta_free_variable<MetaFreeVariable> variable(void)
	{
		return meta_free_variable<MetaFreeVariable>();
	}

	static auto get(void) ->
	decltype(MetaFreeVariable::get())
	{
		return MetaFreeVariable::get();
	}

	static auto address(void) ->
	decltype(MetaFreeVariable::address())
	{
		return MetaFreeVariable::address();
	}

	template <typename T>
	static void set(T val)
	{
		MetaFreeVariable::set(val);
	}
};

template <class MetaFreeVariable>
struct meta_free_var_inst_class : meta_free_var_inst_type<MetaFreeVariable>
{
	meta_attribute_range<
		meta_free_var_inst_class,
		typename mirror::all_member_variables<
			typename MetaFreeVariable::type
		>::type
	> attributes(void) const;
};

template <class MetaFreeVariable>
struct meta_free_var_inst_enum : meta_free_var_inst_type<MetaFreeVariable>
{
	std::string value_name(void)
	{
		return MetaFreeVariable::type::name_by_value(this->get());
	}
};

template <class MetaFreeVariable>
meta_free_var_inst_type<MetaFreeVariable>
get_meta_free_var_inst(mirror::meta_type_tag)
{
	return meta_free_var_inst_type<MetaFreeVariable>();
}

template <class MetaFreeVariable>
meta_free_var_inst_class<MetaFreeVariable>
get_meta_free_var_inst(mirror::meta_class_tag)
{
	return meta_free_var_inst_class<MetaFreeVariable>();
}

template <class MetaFreeVariable>
meta_free_var_inst_enum<MetaFreeVariable>
get_meta_free_var_inst(mirror::meta_enum_tag)
{
	return meta_free_var_inst_enum<MetaFreeVariable>();
}

template <class MetaFreeVariable>
decltype(
	get_meta_free_var_inst<MetaFreeVariable>(
		typename mirror::meta_object_category<
			typename MetaFreeVariable::type
		>::type()
	)
) meta_free_variable_instance(void)
{
	return get_meta_free_var_inst<MetaFreeVariable>(
		typename mirror::meta_object_category<
			typename MetaFreeVariable::type
		>::type()
	);
}

namespace aux {

template <class ParentMetaInstance>
inline auto meta_member_var_inst_type_parent_inst(
	const ParentMetaInstance& parent,
	mirror::meta_variable_tag
) -> decltype(parent.get())
{
	return parent.get();
}

template <class ParentMetaInstance>
inline auto meta_member_var_inst_type_parent_inst(
	const ParentMetaInstance& parent,
	mirror::meta_plain_member_variable_tag
) -> decltype(*parent.address())
{
	return *parent.address();
}

} // namespace aux

template <class ParentMetaInstance, class MetaMemberVariable>
struct meta_member_var_inst_type : meta_inst_base
{
private:
	ParentMetaInstance parent;

	typedef decltype(
		std::declval<ParentMetaInstance>().variable().category()
	) _par_cat;
public:
	meta_member_var_inst_type(ParentMetaInstance _parent)
	 : parent(_parent)
	{ }

	static meta_member_variable<MetaMemberVariable> variable(void)
	{
		return meta_member_variable<MetaMemberVariable>();
	}

	auto get(void) const ->
	decltype(MetaMemberVariable::get(
		aux::meta_member_var_inst_type_parent_inst(
			std::declval<ParentMetaInstance>(),
			_par_cat()
		)
	))
	{
		return MetaMemberVariable::get(
			aux::meta_member_var_inst_type_parent_inst(
				parent,
				_par_cat()
			)
		);
	}

	auto address(void) const ->
	decltype(MetaMemberVariable::address(
		aux::meta_member_var_inst_type_parent_inst(
			std::declval<ParentMetaInstance>(),
			_par_cat()
		)
	))
	{
		return MetaMemberVariable::address(
			aux::meta_member_var_inst_type_parent_inst(
				parent,
				_par_cat()
			)
		);
	}
};

template <class ParentMetaInstance, class MetaMemberVariable>
struct meta_member_var_inst_class
 : meta_member_var_inst_type<ParentMetaInstance, MetaMemberVariable>
{
	meta_member_var_inst_class(ParentMetaInstance _parent)
	 : meta_member_var_inst_type<
		ParentMetaInstance,
		MetaMemberVariable
	>(_parent)
	{ }

	meta_attribute_range<
		meta_member_var_inst_class,
		typename mirror::all_member_variables<
			typename MetaMemberVariable::type
		>::type
	> attributes(void) const;
};

template <class ParentMetaInstance, class MetaMemberVariable>
struct meta_member_var_inst_enum
 : meta_member_var_inst_type<ParentMetaInstance, MetaMemberVariable>
{
	meta_member_var_inst_enum(ParentMetaInstance _parent)
	 : meta_member_var_inst_type<
		ParentMetaInstance,
		MetaMemberVariable
	>(_parent)
	{ }

	std::string value_name(void) const
	{
		return MetaMemberVariable::type::name_by_value(this->get());
	}
};

template <class ParentMetaInstance, class MetaMemberVariable>
meta_member_var_inst_type<ParentMetaInstance, MetaMemberVariable>
get_meta_member_var_inst(
	const ParentMetaInstance& parent,
	mirror::meta_type_tag
)
{
	return meta_member_var_inst_type<
		ParentMetaInstance,
		MetaMemberVariable
	>(parent);
}

template <class ParentMetaInstance, class MetaMemberVariable>
meta_member_var_inst_class<ParentMetaInstance, MetaMemberVariable>
get_meta_member_var_inst(
	const ParentMetaInstance& parent,
	mirror::meta_class_tag
)
{
	return meta_member_var_inst_class<
		ParentMetaInstance,
		MetaMemberVariable
	>(parent);
}

template <class ParentMetaInstance, class MetaMemberVariable>
meta_member_var_inst_enum<ParentMetaInstance, MetaMemberVariable>
get_meta_member_var_inst(
	const ParentMetaInstance& parent,
	mirror::meta_enum_tag
)
{
	return meta_member_var_inst_enum<
		ParentMetaInstance,
		MetaMemberVariable
	>(parent);
}

template <class MetaMemberVariable, class ParentMetaInstance>
auto meta_member_variable_instance(const ParentMetaInstance& parent) ->
decltype(
	get_meta_member_var_inst<ParentMetaInstance, MetaMemberVariable>(
		parent,
		typename mirror::meta_object_category<
			typename MetaMemberVariable::type
		>::type()
	)
)
{
	return get_meta_member_var_inst<ParentMetaInstance, MetaMemberVariable>(
		parent,
		typename mirror::meta_object_category<
			typename MetaMemberVariable::type
		>::type()
	);
}

template <class ParentMetaInstance, class ... MetaMemberVariables>
struct meta_attribute_range<
	ParentMetaInstance,
	mirror::mp::range<MetaMemberVariables ... >
>
{
private:
	typedef mirror::mp::range<MetaMemberVariables ... > base_rng;

	const ParentMetaInstance& parent;

	template <typename Func>
	void call_for_each(
		mirror::mp::empty_range,
		Func&,
		bool
	) const
	{
	}

	template <typename Func, typename MMV>
	void call_for_each(
		mirror::mp::range<MMV>,
		Func& func,
		bool first
	) const
	{
		auto inst = meta_member_variable_instance<MMV>(parent);
		func(inst, first, true);
	}

	template <typename Func, typename MMV1, typename MMV2, typename ... P>
	void call_for_each(
		mirror::mp::range<MMV1, MMV2, P...>,
		Func& func,
		bool first
	) const
	{
		auto inst = meta_member_variable_instance<MMV1>(parent);
		func(inst, first, false);
		this->call_for_each(
			mirror::mp::range<MMV2, P...>(),
			func,
			false
		);
	}
public:
	meta_attribute_range(const ParentMetaInstance& _parent)
	 : parent(_parent)
	{ }

	static integer<
		typename mirror::mp::empty<base_rng>::type
	> empty(void)
	{
		return integer<
			typename mirror::mp::empty<base_rng>::type
		>();
	}

	static integer<
		typename mirror::mp::size<base_rng>::type
	> size(void)
	{
		return integer<
			typename mirror::mp::size<base_rng>::type
		>();
	}

	decltype(
		meta_member_variable_instance<
			typename mirror::mp::front<base_rng>::type
		>(std::declval<ParentMetaInstance>())
	) front(void) const
	{
		return meta_member_variable_instance<
			typename mirror::mp::front<base_rng>::type
		>(parent);
	}

	template <typename Int, Int Index>
	decltype(
		meta_member_variable_instance<
			typename mirror::mp::at_c<base_rng, Index>::type
		>(*((ParentMetaInstance*)nullptr))
	) at(integer<std::integral_constant<Int, Index> >) const
	{
		return meta_member_variable_instance<
			typename mirror::mp::at_c<base_rng, Index>::type
		>(parent);
	}

	template <int Index>
	decltype(
		meta_member_variable_instance<
			typename mirror::mp::at_c<base_rng, Index>::type
		>(*((ParentMetaInstance*)nullptr))
	) at_c(void) const
	{
		return meta_member_variable_instance<
			typename mirror::mp::at_c<base_rng, Index>::type
		>(parent);
	}

	template <typename Functor>
	void for_each(Functor func) const
	{
		this->call_for_each(base_rng(), func, true);
	}

	template <typename ResultType, typename Functor, typename ValueType>
	ResultType accumulate(Functor, ValueType value) const
	{
		// TODO:
		return value;
	}
};

template <class MetaFreeVariable>
inline meta_attribute_range<
	meta_free_var_inst_class<MetaFreeVariable>,
	typename mirror::all_member_variables<
		typename MetaFreeVariable::type
	>::type
> meta_free_var_inst_class<MetaFreeVariable>::attributes(void) const
{
	return meta_attribute_range<
		meta_free_var_inst_class<MetaFreeVariable>,
		typename mirror::all_member_variables<
			typename MetaFreeVariable::type
		>::type
	>(*this);
}

template <class ParentMetaInstance, class MetaMemberVariable>
inline meta_attribute_range<
	meta_member_var_inst_class<
		ParentMetaInstance,
		MetaMemberVariable
	>,
	typename mirror::all_member_variables<
		typename MetaMemberVariable::type
	>::type
> meta_member_var_inst_class<
	ParentMetaInstance,
	MetaMemberVariable
>::attributes(void) const
{
	return meta_attribute_range<
		meta_member_var_inst_class<
			ParentMetaInstance,
			MetaMemberVariable
		>,
		typename mirror::all_member_variables<
			typename MetaMemberVariable::type
		>::type
	>(*this);
}

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

