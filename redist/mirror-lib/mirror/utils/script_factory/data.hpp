/**
 * .file mirror/utils/script_factory/data.hpp
 * .brief Input data for the script-parsing factory generator plugin
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_FACTORY_DATA_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_FACTORY_DATA_1011291729_HPP

#include <string>
#include <vector>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

template <typename Traits>
class script_fact_data
{
private:
	typename Traits::shared_data shared_data;
	aux::script_fact_manager_parser<Traits> root;

	typedef typename Traits::iterator iterator;
public:
	script_fact_data(void)
	 : root(shared_data)
	{ }

	aux::script_fact_param<Traits> data(void)
	{
		return aux::script_fact_param<Traits>(
			root,
			shared_data
		);
	}

	void print_parsers(std::ostream& out)
	{
		std::vector<bool> conns;
		root.print(conns, out, true);
	}

	void set(iterator begin, iterator end)
	{
		root.match(begin, end);
	}

	template <typename Container>
	void set(Container& container)
	{
		set(container.begin(), container.end());
	}
};

typedef script_fact_data<script_fact_def_traits<std::string::const_iterator> >
	script_factory_input;

typedef script_fact_data<script_fact_def_traits<const char*> >
	c_str_script_factory_input;

MIRROR_NAMESPACE_END

#endif //include guard

