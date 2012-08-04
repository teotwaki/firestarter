/**
 * .file lagoon/utils/sql_factory/manufacturer.hpp
 * .brief SQL-based polymorphic factory manufacturers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_MANUFACTURER_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_MANUFACTURER_1011291729_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Implementation of the polymorph factory manufacturer interface
template <class Product, class Traits>
class sql_fact_manuf_native
 : public polymorph_factory_manufacturer<Product>
{
private:
	typedef typename Traits::helper_type helper;
	typename Traits::data_type data;
	int column_number;

	template <typename T>
	static inline T get_default(void)
	{
		return T();
	}
public:
	sql_fact_manuf_native(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): data(*raw_cast<typename Traits::data_type*>(parent_data))
	 , column_number(helper::find_column_number(data, param, context))
	{
	}

	void finish(raw_ptr parent_data)
	{
	}

	Product create(void)
	{
		if(column_number < 0) return get_default<Product>();
		assert(column_number < data.col_count());
		if(!data.is_ok(column_number))
			return get_default<Product>();
		return data.template get_as<Product>(column_number);
	}
};

} // namespace aux

template <class Product, class Traits>
class sql_fact_manufacturer
 : public aux::sql_fact_manuf_native <Product, Traits>
{
public:
	sql_fact_manufacturer(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): aux::sql_fact_manuf_native<Product, Traits>(
		parent_data,
		param,
		context
	){ }
};

LAGOON_NAMESPACE_END

#endif //include guard

