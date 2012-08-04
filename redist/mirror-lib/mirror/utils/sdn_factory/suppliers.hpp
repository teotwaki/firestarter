/**
 * .file mirror/utils/sdn_factory/suppliers.hpp
 * .brief Default implementation of suppliers structured data factory framework
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_FACTORY_SUPPLIERS_1011291729_HPP
#define MIRROR_UTILS_SDN_FACTORY_SUPPLIERS_1011291729_HPP

#include <mirror/utils/unused_type.hpp>
#include <stdexcept>
#include <memory>

MIRROR_NAMESPACE_BEGIN

template <typename Product, class Traits>
class sdn_fact_suppl
{
private:
	typedef aux::sdn_fact_param<Traits> param_type;
	typename Traits::shared_data& shared_data;
public:
	inline sdn_fact_suppl(const param_type& parent_data, unused_type)
	 : shared_data(parent_data.shared_data())
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.handler().skip_subhandler();
	}

	inline Product operator()(void) const
	{
		throw std::runtime_error(
			Traits::translate(
				shared_data,
				"Copy construction not supported"
			)
		);
		return *((Product*)nullptr);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

