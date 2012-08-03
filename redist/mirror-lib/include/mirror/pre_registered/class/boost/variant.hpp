/**
 * @file mirror/pre_registered/class/boost/variant.hpp
 * @brief Pre-registration of the boost::variant template class with Mirror
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_BOOST_VARIANT_1105050939_HPP
#define MIRROR_PRE_REGISTERED_CLASS_BOOST_VARIANT_1105050939_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/nil_term_pack.hpp>
#include <mirror/pre_registered/namespace/boost.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <type_traits>

MIRROR_REG_BEGIN
namespace mp {

template <>
struct is_pack_nil_type<boost::detail::variant::void_>
{
	typedef std::true_type type;
};

} // namespace mp

MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(BOOST_VARIANT_ENUM_PARAMS(typename T)),
	struct, boost, variant, (BOOST_VARIANT_ENUM_PARAMS(T))
)
/* TODO: if/when boost variant starts to support variadic templates add
   some CT switch here between the above and the macro below.
MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(typename ...Types),
	struct, boost, variant, (Types...)
)
*/
	typedef typename mirror::mp::remove_nil_types<
		mirror::mp::range<
			BOOST_VARIANT_ENUM_PARAMS(T)
			// TODO: Add a CT switch here when variadic
			//  templates are supported by variant
			// Types ...
		>
	>::type var_params;

MIRROR_REG_CONSTRUCTORS_BEGIN
	ctr_index_init;
	template <int I>
	struct ctr_params_init
	{
		static typename mp::at_c<var_params, I>::type
		type(std::integral_constant<int, 0>);

		static const char* name(std::integral_constant<int, 0>)
		{
			return "value";
		}

		static size_t name_length(std::integral_constant<int, 0>)
		{
			return sizeof("value") - 1;
		}

		typedef std::integral_constant<int, 1> size;
	};

	template <int I>
	static ctr_params_init<I>
	ctr_params(std::integral_constant<int, I>);

	template <int I>
	static spec_public_tag
	ctr_access(std::integral_constant<int, I>);

	typedef typename mirror::mp::size<var_params>::type
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

namespace aux {

template <class Variant, typename Type, int Index>
class boost_var_locator
{
private:
	Variant& var;
public:
	boost_var_locator(Variant& _var)
	 : var(_var)
	{ }

	typedef std::false_type safe;

	inline bool dereferencable(void) const
	{
		return var.which() == Index;
	}

	operator bool (void) const
	{
		return dereferencable();
	}

	bool operator !(void) const
	{
		return !dereferencable();
	}

	Type& get(void) const
	{
		assert(dereferencable());
		return boost::get<Type>(var);
	}
};


template <typename Variant, typename Type, int Index>
class boost_var_trvrsl
{
private:
	Variant& var;
	bool eoc;
public:
	boost_var_trvrsl(Variant& _var)
	 : var(_var)
	 , eoc(false)
	{ }

	bool done(void) const
	{
		return eoc || (var.which() != Index);
	}

	bool empty(void) const
	{
		return done();
	}

	void step_front(void)
	{
		assert(!empty());
		eoc = true;
	}

	boost_var_locator<Variant, Type, Index> front(void) const
	{
		assert(!empty());
		return boost_var_locator<
			Variant,
			Type,
			Index
		>(var);
	}
};


template <typename Variant, typename Type, int Index>
struct meta_boost_var_trav
{
	typedef meta_traversal_tag category;
	typedef mirror::mp::empty_range signature;

	static inline std::string base_name(void)
	{
		return MIRRORED_TYPE(Type)::base_name();
	}

	typedef boost_var_trvrsl<Variant, Type, Index>
		traversal_type;

	static traversal_type start(Variant& var)
	{
		return traversal_type(var);
	}

	typedef boost_var_trvrsl<const Variant, const Type, Index>
		const_traversal_type;

	static const_traversal_type start(const Variant& var)
	{
		return const_traversal_type(var);
	}
};

template <class Variant, typename Type>
struct meta_boost_var_assign_inserter
{
	typedef meta_inserter_tag category;

	static std::string base_name(void)
	{
		return std::string(
			"assign",
			sizeof("assign") - 1
		);
	}

	typedef mirror::mp::range<
		mirror::container_op_value_param
	> signature;

	static void insert(
		Variant& var,
		const Type& value
	)
	{
		var = value;
	}
};

} // namespace aux

namespace _container {

template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
struct _<boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >
/* TODO: add a CT switch here
template <class ... Types>
struct _<boost::variant<Types ...> >
*/
{
	typedef boost::variant<
		BOOST_VARIANT_ENUM_PARAMS(T)
		// TODO: add a CT switch here
		// Types ...
	> _variant;

	typedef typename mirror::mp::remove_nil_types<
		mirror::mp::range<
			BOOST_VARIANT_ENUM_PARAMS(T)
			// TODO: Add a CT switch here when variadic
			//  templates are supported by variant
			// Types ...
		>
	>::type var_params;

	template <int Index>
	struct cntnr
	{
		typedef typename mirror::mp::at_c<
			var_params, Index
		>::type elem_type;

		static bool empty(const _variant& var)
		{
			return var.which() != Index;
		}

		static size_t size_hint(const _variant& var)
		{
			return empty() ? 0 : 1;
		}

		typedef mirror::aux::meta_boost_var_trav<
			_variant,
			elem_type,
			Index
		> default_traversal;

		typedef mirror::mp::range<
			default_traversal
		> traversals;

		typedef mirror::mp::empty_range locators;

		typedef mirror::mp::range<
			mirror::aux::meta_boost_var_assign_inserter<
				_variant,
				elem_type
			>
		> inserters;

		typedef mirror::mp::empty_range erasers;
	};
	template <int I>
	static cntnr<I> _cntnr(std::integral_constant<int, I>);

	typedef typename mirror::mp::size<var_params>::type
		cntnr_count;

	typedef variant_container_tag kind;
};

} // namespace container

MIRROR_REG_END

#endif //include guard

