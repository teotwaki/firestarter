/**
 * .file lagoon/utils/sql_factory/enumerator.hpp
 * .brief SQL-based polymorphic factory enumerators
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_ENUMERATOR_1101191310_HPP
#define LAGOON_UTILS_SQL_FACTORY_ENUMERATOR_1101191310_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN

// Implementation of the polymorph factory manufacturer interface
template <class Traits>
class sql_fact_enumerator
 : public polymorph_factory_enumerator
{
private:
	typedef typename Traits::helper_type helper;
	typename Traits::data_type data;
	int column_number;
	shared<meta_enum> enumeration;
public:
	sql_fact_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): data(*raw_cast<typename Traits::data_type*>(parent_data))
	 , column_number(helper::find_column_number(data, param, context))
	 , enumeration(param->type().as<meta_enum>())
	{
		assert(enumeration);
	}

	void finish(raw_ptr parent_data)
	{
	}

	int create(void)
	{
		if(column_number < 0) return -1;
		assert(column_number < data.col_count());
		if(!data.is_ok(column_number)) return -1;
		return int(enumeration->value_by_name(
			data.template get_as<std::string>(column_number)
		));
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

