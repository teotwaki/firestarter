/**
 * .file lagoon/utils/sql_factory/manager.hpp
 * .brief SQL-based polymorphic factory manager
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_MANAGER_1011291729_HPP
#define LAGOON_UTILS_SQL_FACTORY_MANAGER_1011291729_HPP

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sql_fact_manager : public polymorph_factory_manager
{
private:
	typename Traits::helper_type helper;
	typename Traits::data_type data;
	// the data describing the Manager context
	typename Traits::helper_type::context_data ctx_data;
	// the data used by the helper to pick the
	// appropriate constructor
	typename Traits::helper_type::index_pick_data idx_pick_data;
public:
	sql_fact_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	): data(*raw_cast<typename Traits::data_type*>(parent_data))
	 , ctx_data(helper.save_context(context))
	{ }

	void finish(raw_ptr parent_data)
	{ }

	raw_ptr add_constructor(
		raw_ptr parent_data,
		const shared<meta_constructor>& constructor,
		const polymorph_factory_context& context,
		bool backward_iteration
	)
	{
		// Let the helper take a look at the current constructor
		// and to update the index pick data
		helper.preview_constructor(
			constructor,
			ctx_data,
			idx_pick_data,
			backward_iteration
		);
		return raw_ptr(&data);
	}

	int index(void)
	{
		// Let the helper look at the sql result data
		// and the local index pick data and choose
		// the most appropriate constructor
		// for object created from the data in the current row
		return helper.pick_constructor(
			data,
			ctx_data,
			idx_pick_data
		);
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

