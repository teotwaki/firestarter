/**
 * .file lagoon/utils/sql_factory/suppliers.hpp
 * .brief SQL-based polymorphic factory suppliers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_SUPPLIERS_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_SUPPLIERS_1011291729_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN

template <class Helper>
class sql_fact_suppliers
 : public polymorph_factory_suppliers
{
public:
	sql_fact_suppliers(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{ }

	raw_ptr get(void)
	{
		assert(!"No external sources available");
		return raw_ptr();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

