/**
 * .file mirror/utils/sql_factory/manager.hpp
 * .brief Manager for the sql-based factory template
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SQL_FACTORY_MANAGER_1011291729_HPP
#define MIRROR_UTILS_SQL_FACTORY_MANAGER_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sql_factory/fwd_decl.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename Traits>
class sql_fact_source<void, Traits>
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
	template <class Context>
	inline sql_fact_source(
		const typename Traits::data_type& parent_data,
		Context context
	): data(parent_data)
	 , ctx_data(helper.save_context(context))
	{ }

	void finish(const typename Traits::data_type& parent_data)
	{ }

	template <class ConstructorInfo>
	const typename Traits::data_type& add_constructor(
		const typename Traits::data_type& parent_data,
		ConstructorInfo ctr_info
	)
	{
		// let the helper preview the constructor
		// and update the index pick data as required
		helper.preview_constructor(
			typename ConstructorInfo::product(),
			typename ConstructorInfo::constructor_index(),
			ctx_data,
			idx_pick_data,
			ctr_info.enumeration_info.going_backward()
		);
		return data;
	}

	// Tries to find the best constructor for the current row
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

MIRROR_NAMESPACE_END

#endif //include guard

