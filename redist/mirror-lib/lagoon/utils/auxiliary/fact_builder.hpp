/**
 * .file lagoon/utils/auxiliary/fact_builder.hpp
 * .brief Common implementation of polymorphic factory builder
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_AUX_FACT_BUIDLER_1011291729_HPP
#define LAGOON_UTILS_AUX_FACT_BUIDLER_1011291729_HPP

#include <lagoon/polymorph_factory.hpp>

LAGOON_NAMESPACE_BEGIN
namespace aux {

template <
	class Traits,
	template <class> class Manager,
	template <class> class Composite,
	template <class> class Arrayer,
	template <class> class Suppliers,
	template <class> class Enumerator
>
class common_fact_builder_base
{
public:
	Manager<Traits>* make_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	)
	{
		return new Manager<Traits>(
			parent_data,
			context
		);
	}

	Composite<Traits>* make_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	)
	{
		return new Composite<Traits>(
			parent_data,
			product,
			context
		);
	}

	Composite<Traits>* make_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return new Composite<Traits>(
			parent_data,
			param,
			context
		);
	}

	Arrayer<Traits>* make_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& element,
		const polymorph_factory_context& context
	)
	{
		return new Arrayer<Traits>(
			parent_data,
			element,
			context
		);
	}

	Suppliers<Traits>* make_suppliers(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return new Suppliers<Traits>(
			parent_data,
			param,
			context
		);
	}

	Enumerator<Traits>* make_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return new Enumerator<Traits>(
			parent_data,
			param,
			context
		);
	}
};


template <
	class Traits,
	template <class, class> class Manufacturer
>
struct common_fact_builder_unit
{
	template <class Product>
	struct maker
	{
		Manufacturer<Product, Traits>*
		make_manufacturer(
			raw_ptr parent_data,
			const shared<meta_parameter>& param,
			const polymorph_factory_context& context
		)
		{
			return new Manufacturer<
				Product,
				Traits
			>(parent_data, param, context);
		}
	};
};


template <
	class Traits,
	template <class> class Manager,
	template <class> class Composite,
	template <class> class Arrayer,
	template <class> class Suppliers,
	template <class> class Enumerator,
	template <class, class> class Manufacturer
>
class common_fact_builder_templ
 : public concrete_polymorph_factory_builder<
	common_fact_builder_base<
		Traits,
		Manager,
		Composite,
		Arrayer,
		Suppliers,
		Enumerator
	>,
	common_fact_builder_unit<Traits, Manufacturer>::template maker
>{ };

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

