/**
 * .file lagoon/utils/sql_factory/composite.hpp
 * .brief SQL-based polymorphic factory composite
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_COMPOSITE_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_COMPOSITE_1011291729_HPP

#include <cassert>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sql_fact_composite
 : public polymorph_factory_composite
{
private:
	typename Traits::helper_type helper;
	typename Traits::data_type unit_data;
public:
	sql_fact_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): unit_data(*raw_cast<typename Traits::data_type*>(parent_data))
	{
	}

	sql_fact_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): unit_data(*raw_cast<typename Traits::data_type*>(parent_data))
	{
	}

	void finish(raw_ptr parent_data)
	{
	}

	raw_ptr data(void)
	{
		return raw_ptr(&unit_data);
	}

	void on_create(void)
	{
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

