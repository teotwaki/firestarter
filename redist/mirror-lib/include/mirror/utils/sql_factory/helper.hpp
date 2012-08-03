/**
 * .file mirror/utils/sql_factory/helper.hpp
 * .brief Default helper class for the sql-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_HELPER_1011291729_HPP
#define MIRROR_UTILS_SQL_FACTORY_HELPER_1011291729_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/meta_prog/for_each.hpp>
#include <cassert>
#include <string>
#include <set>
#include <list>

MIRROR_NAMESPACE_BEGIN

class sql_fact_helper_base
{
public:
	typedef std::string context_data;
	typedef std::list<std::set<std::string> > index_pick_data;
private:
	struct context_saver
	{
		std::string& ctx_data;

		context_saver(std::string& data_ref)
		 : ctx_data(data_ref)
		{ }

		template <typename IterInfo>
		void operator()(IterInfo) const
		{
			typedef typename IterInfo::type meta_named;
			ctx_data.append(meta_named::base_name());
			if(!IterInfo::is_last::value) ctx_data.append("__");
		}
	};

	struct name_getter
	{
		std::set<std::string>& names;
		const std::string& ctx_data;

		name_getter(std::set<std::string>& ns, const std::string& ctx)
		 : names(ns)
		 , ctx_data(ctx)
		{ }

		template <typename MetaParam>
		void operator()(MetaParam) const
		{
			if(ctx_data.empty())
				names.insert(MetaParam::base_name());
			else names.insert(ctx_data+"__"+MetaParam::base_name());
		}
	};
public:
	template <class Context>
	static context_data save_context(Context)
	{
		context_data result;
		mp::for_each_ii<Context>(context_saver(result));
		return result;
	}

	template <class MetaClass, class ConstructorIndex>
	static void preview_constructor(
		MetaClass product,
		ConstructorIndex,
		context_data& ctx_data,
		index_pick_data& ctr_param_names,
		bool backward_iteration
	)
	{
		std::set<std::string> col_names;
		typedef typename mp::at<
			constructors<MetaClass>,
			ConstructorIndex
		>::type meta_ctr;
		mp::for_each<parameters<meta_ctr> >(
			name_getter(col_names, ctx_data)
		);
		if(backward_iteration)
			ctr_param_names.push_front(col_names);
		else ctr_param_names.push_back(col_names);
	}

	template <class Data, class ConstructionInfo>
	static int find_column_number(const Data& data, ConstructionInfo)
	{
		return data.find_column(
			save_context(typename ConstructionInfo::context()),
			ConstructionInfo::parameter_index::value
		);
	}
};

class sql_fact_ctr_picker
{
private:
	typedef sql_fact_helper_base::context_data context_data;
	typedef sql_fact_helper_base::index_pick_data index_pick_data;
protected:
	// Tries to find the best constructor for the current row
	template <typename Data>
	static int do_pick_constructor(
		const Data& data,
		const context_data& ctx_data,
		const index_pick_data& ctr_param_names
	)
	{
		// the resulting and the current index
		int result = 0, current = 0;
		// the number of matching columns for
		// the best constructor found so far
		size_t best_match = 0;
		// go through the constructors
		auto i = ctr_param_names.begin(), e = ctr_param_names.end();
		while(i != e)
		{
			// the number of matching columns for
			// the current constructor
			size_t match = 0;
			const std::set<std::string>& names = *i;
			// go through the parameter names
			auto j = names.begin(), f = names.end();
			while(j != f)
			{
				// try to find column by name and
				// check if is not null
				std::size_t k=0, n=data.col_count();
				while(k != n)
				{
					std::string name = data.get_name(k);
					if(name.find(*j) == 0)
						if(data.is_ok(k)) ++match;
					++k;
				}
				// go to the next parameter
				++j;
			}
			// if this constructor is doing better
			// then the previous best match
			if(best_match <= match)
			{
				// or it is a perfect match
				if(best_match < match || match == names.size())
				{
					// remember the match and the index
					best_match = match;
					result = current;
				}
			}
			// go to the next constructor
			++i;
			// increment the constructor index
			++current;
		}
		return result;
	}
};

class sql_fact_helper
 : public sql_fact_ctr_picker
 , public sql_fact_helper_base
{
public:
	typedef sql_fact_helper_base::context_data context_data;
	typedef sql_fact_helper_base::index_pick_data index_pick_data;

	// Tries to find the best constructor for the current row
	template <typename Data>
	static int pick_constructor(
		const Data& data,
		const context_data& ctx_data,
		const index_pick_data& ctr_param_names
	)
	{
		return do_pick_constructor(data, ctx_data, ctr_param_names);
	}
};

class sql_fact_quick_helper
 : public sql_fact_ctr_picker
 , public sql_fact_helper_base
{
private:
	int picked_constructor;
public:
	typedef sql_fact_helper_base::context_data context_data;
	typedef sql_fact_helper_base::index_pick_data index_pick_data;

	sql_fact_quick_helper(void)
	 : picked_constructor(-1)
	{ }

	// Tries to find the best constructor for the current row
	template <typename Data>
	int pick_constructor(
		const Data& data,
		const context_data& ctx_data,
		const index_pick_data& ctr_param_names
	)
	{
		if(picked_constructor < 0)
		{
			picked_constructor = do_pick_constructor(
				data,
				ctx_data,
				ctr_param_names
			);
		}
		return picked_constructor;
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

