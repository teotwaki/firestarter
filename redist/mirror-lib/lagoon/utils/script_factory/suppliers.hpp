/**
 * .file lagoon/utils/script_factory/suppliers.hpp
 * .brief Script-parsing polymorphic factory suppliers
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_SUPPLIERS_1011291729_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_SUPPLIERS_1011291729_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class script_fact_suppliers
 : public polymorph_factory_suppliers
 , public aux::script_fact_utils<Traits>
{
private:
	typedef mirror::aux::script_fact_param<Traits> param_type;
	typedef mirror::aux::script_fact_parser<Traits> parser_type;
	typename Traits::shared_data& shared_data;
public:
	script_fact_suppliers(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): shared_data(this->deref(parent_data).shared_data())
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).parser().skip_subparser();
	}

	raw_ptr get(void)
	{
		throw std::runtime_error(
			Traits::translate(
				shared_data,
				"Copy construction not supported"
			)
		);
		return raw_ptr();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

