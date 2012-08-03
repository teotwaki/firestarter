/**
 * .file lagoon/utils/sql_factory/arrayer.hpp
 * .brief SQL-based polymorphic factory arrayer
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SQL_FACTORY_ARRAYER_1103101411_HPP
#define LAGOON_UTILS_SQL_FACTORY_ARRAYER_1103101411_HPP

#include <cassert>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sql_fact_arrayer
 : public polymorph_factory_arrayer
{
private:
	typename Traits::helper_type helper;
	typename Traits::data_type unit_data;

	typedef typename polymorph_factory_arrayer::element_producer
		element_producer;
public:
	sql_fact_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): unit_data(*raw_cast<typename Traits::data_type*>(parent_data))
	{
		//TODO: finish the arrayer when it is implemented in Mirror
		assert(!"Initializer-list constructors not supported yet");
	}

	raw_ptr data(void)
	{
		return raw_ptr(&unit_data);
	}

	void finish(raw_ptr parent_data)
	{
	}

	void create(element_producer&)
	{
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

