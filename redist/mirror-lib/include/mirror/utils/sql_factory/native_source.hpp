/**
 * .file mirror/utils/sql_factory/native_source.hpp
 * .brief Data converters for sql-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_NATIVE_SOURCE_1011291729_HPP
#define MIRROR_UTILS_SQL_FACTORY_NATIVE_SOURCE_1011291729_HPP

#include <mirror/utils/native_types.hpp>
#include <mirror/utils/sql_factory/fwd_decl.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename Product, typename Traits>
class native_sql_source
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
	native_sql_source(
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
		// get the current value from the column
		return data.template get_as<Product>(column_number);
	}
};

#define MIRROR_SPECIALIZE_FACTORY_SQL_SOURCE(TYPE, X)\
template <typename Traits> \
class sql_fact_source< TYPE, Traits>\
 : public native_sql_source< TYPE, Traits> \
{ \
public: \
	template < class ConstructionInfo >\
	sql_fact_source( \
		const typename Traits::data_type& data, \
		ConstructionInfo ctr_info \
	): native_sql_source< TYPE, Traits>( data, ctr_info ) \
	{ } \
};

MIRROR_UTILS_FOR_EACH_NATIVE_TYPE(
	MIRROR_SPECIALIZE_FACTORY_SQL_SOURCE,
	_
)

#undef MIRROR_SPECIALIZE_FACTORY_SQL_SOURCE

MIRROR_NAMESPACE_END

#endif //include guard

