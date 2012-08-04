/**
 * .file lagoon/utils/sdn_factory/arrayer.hpp
 * .brief Script-parsing polymorphic factory arrayer
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SDN_FACTORY_ARRAYER_1103011404_HPP
#define LAGOON_UTILS_SDN_FACTORY_ARRAYER_1103011404_HPP

#include <cassert>
#include <memory>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sdn_fact_arrayer
 : public polymorph_factory_arrayer
 , public aux::sdn_fact_utils<Traits>
{
private:
	typedef mirror::aux::sdn_fact_param<Traits> param_type;
	typedef mirror::aux::sdn_fact_handler<Traits> handler_type;
	typedef mirror::aux::sdn_fact_sequence_handler<Traits> seq_handler;

	typedef typename polymorph_factory_arrayer::element_producer
		element_producer;

	std::shared_ptr<seq_handler> handler;
	param_type param;
public:
	sdn_fact_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): handler(new seq_handler(this->deref(parent_data)))
	 , param(handler.get(), this->deref(parent_data))
	{ }

	sdn_fact_arrayer(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): handler(new seq_handler(this->deref(parent_data)))
	 , param(handler.get(), this->deref(parent_data))
	{ }

	raw_ptr data(void)
	{
		return raw_ptr(&param);
	}

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).handler().add_subhandler(handler.get());
	}

	void create(element_producer& producer)
	{
		auto	elems_i = handler->elements_begin(),
			elems_e = handler->elements_end();
		while(elems_i != elems_e)
		{
			handler->reset(elems_i);
			producer.make_next();
			++elems_i;
		}
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

