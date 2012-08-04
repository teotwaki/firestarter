/**
 * .file mirror/utils/sql_factory/enumerator.hpp
 * .brief Enumerator for sql-based factory template
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_NUMERATOR_1101181500_HPP
#define MIRROR_UTILS_SQL_FACTORY_NUMERATOR_1101181500_HPP

#include <mirror/utils/native_types.hpp>
#include <mirror/utils/enum_val_by_name.hpp>
#include <mirror/utils/sql_factory/fwd_decl.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename Product, typename Traits>
class sql_fact_enum
{
private:
	typename Traits::helper_type helper;
	typename Traits::data_type data;
	int column_number;

	template <typename T>
	static inline T get_default(void)
	{
		return T();
	}
public:
	template <class ConstructionInfo>
	sql_fact_enum(
		const typename Traits::data_type& parent_data,
		ConstructionInfo ctr_info
	): data(parent_data)
	 , column_number(helper.find_column_number(data, ctr_info))
	{ }

	void finish(const typename Traits::data_type& parent_data)
	{ }

	inline Product operator()(void)
	{
		// if we didn't find the column by name
		// return a default value
		if(column_number < 0) return get_default<Product>();
		// check the the col number
		assert(column_number < data.col_count());
		// if the value is NULL return the default value
		if(!data.is_ok(column_number))
			return get_default<Product>();
		// get the current string-value from the column
		// and get the respective enumerated value
		auto opt_val = enum_value_by_name<Product>::get_opt(
			data.template get_as<std::string>(column_number)
		);
		if(opt_val.first) return opt_val.second;
		else return get_default<Product>();
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

