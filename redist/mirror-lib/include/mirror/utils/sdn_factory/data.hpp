/**
 * .file mirror/utils/sdn_factory/data.hpp
 * .brief Input data for the structured data notation factory plugin framework
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_FACTORY_DATA_1011291729_HPP
#define MIRROR_UTILS_SDN_FACTORY_DATA_1011291729_HPP

#include <string>
#include <vector>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

template <typename Traits>
class sdn_fact_data
{
private:
	typename Traits::shared_data shared_data;
	aux::sdn_fact_manager_handler<Traits> root;

	typedef typename Traits::element element;
public:
	sdn_fact_data(void)
	 : root(shared_data)
	{ }

	aux::sdn_fact_param<Traits> data(void)
	{
		return aux::sdn_fact_param<Traits>(
			root,
			shared_data
		);
	}

	void print_handlers(std::ostream& out)
	{
		std::vector<bool> conns;
		root.print(conns, out, true);
	}

	void set(element elem)
	{
		root.match(elem);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

