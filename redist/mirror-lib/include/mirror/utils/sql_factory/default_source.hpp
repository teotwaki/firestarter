/**
 * .file mirror/utils/sql_factory/default_source.hpp
 * .brief Default parameter source for the sql-based factory template
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_DEFAULT_SOURCE_1011291729_HPP
#define MIRROR_UTILS_SQL_FACTORY_DEFAULT_SOURCE_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/factory.hpp>
#include <mirror/utils/sql_factory/fwd_decl.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename Product, typename Traits>
class sql_fact_source
{
private:
	typename Traits::data_type data;
	typedef factory_maker<
		mirror::sql_fact_source,
		mirror::sql_fact_pool,
		mirror::sql_fact_enum,
		Traits
	> maker;
	typename maker::template factory<Product>::type fact;
public:
	template <class ConstructionInfo>
	sql_fact_source(
		const typename Traits::data_type& parent_data,
		ConstructionInfo construction_info
	): data(parent_data)
	 , fact(data, construction_info)
	{ }

	void finish(const typename Traits::data_type& parent_data)
	{ }

	inline Product operator()(void)
	{
		return fact();
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

