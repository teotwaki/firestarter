/**
 * .file lagoon/utils/sql_factory/helper.hpp
 * .brief SQL-based polymorphic factory helper
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_HELPER_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_HELPER_1011291729_HPP

#include <mirror/utils/sql_factory/helper.hpp>
#include <lagoon/polymorph_factory.hpp>
#include <string>
#include <set>
#include <list>

LAGOON_NAMESPACE_BEGIN

class sql_fact_helper : public mirror::sql_fact_helper
{
public:
	static context_data save_context(
		const polymorph_factory_context& context
	)
	{
		context_data result;
		auto rng = context.all();
		while(!rng.empty())
		{
			if(!result.empty()) result.append("__");
			result.append(rng.front()->base_name());
			rng.step_front();
		}
		return result;
	}

	static void preview_constructor(
		const shared<meta_constructor>& constructor,
		context_data& ctx_data,
		index_pick_data& ctr_param_names,
		bool backward_iteration
	)
	{
		std::set<std::string> col_names;
		auto params = constructor->parameters();
		while(!params.empty())
		{
			if(ctx_data.empty())
				col_names.insert(params.front()->base_name());
			else col_names.insert(
				ctx_data +
				"__" +
				params.front()->base_name()
			);
			params.step_front();
		}
		if(backward_iteration)
			ctr_param_names.push_front(col_names);
		else ctr_param_names.push_back(col_names);
	}

	template <typename Data>
	static int find_column_number(
		const Data& data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return data.find_column(
			save_context(context),
			param->position()
		);
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

