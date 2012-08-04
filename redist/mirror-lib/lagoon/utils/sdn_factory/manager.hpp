/**
 * .file lagoon/utils/sdn_factory/manager.hpp
 * .brief Script-parsing polymorphic factory manager
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SDN_FACTORY_MANAGER_1011291729_HPP
#define LAGOON_UTILS_SDN_FACTORY_MANAGER_1011291729_HPP

#include <list>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class sdn_fact_manager
 : public polymorph_factory_manager
 , public aux::sdn_fact_utils<Traits>
{
private:
	typedef mirror::aux::sdn_fact_param<Traits> param_type;
	typedef mirror::aux::sdn_fact_handler<Traits> handler_type;
	typedef mirror::aux::sdn_fact_manager_handler<Traits> manager_handler;

	std::shared_ptr<manager_handler> handler;

	std::list<std::shared_ptr<handler_type> > ctr_handlers;
	// internal list of parameters passed to the subhandlers
	std::list<param_type> ctr_params;

	template <typename Range>
	static std::string make_param_list(Range params)
	{
		std::string result;
		while(!params.empty())
		{
			if(!result.empty()) result.append(", ");
			result.append(params.front()->type()->full_name());
			params.step_front();
		}
		return result;
	}

	std::shared_ptr<handler_type> make_ctr_handler(
		const param_type& parent_data,
		const shared<meta_constructor>& constructor
	)
	{
		typedef mirror::aux::sdn_fact_ctr_handler<Traits>
			constructor_handler;
		auto params = constructor->parameters();
		return std::shared_ptr<handler_type>(
			new constructor_handler(
				parent_data,
				constructor->result_type()->full_name(),
				make_param_list(params),
				params.size()
			)
		);
	}

	param_type append_constructor(
		const param_type& parent_data,
		const std::shared_ptr<handler_type>& ctr_handler,
		bool backward_iteration
	)
	{
		if(backward_iteration)
		{
			ctr_handlers.push_front(ctr_handler);
			return param_type(
				ctr_handlers.front().get(),
				parent_data
			);
		}
		else
		{
			ctr_handlers.push_back(ctr_handler);
			return param_type(
				ctr_handlers.back().get(),
				parent_data
			);
		}
	}
public:
	sdn_fact_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	): handler(new manager_handler(this->deref(parent_data).shared_data()))
	{ }

	void finish(raw_ptr parent_data)
	{
		auto i = ctr_handlers.begin(), e = ctr_handlers.end();
		while(i != e)
		{
			handler->add_subhandler(i->get());
			++i;
		}
		this->deref(parent_data).handler().add_subhandler(handler.get());
	}

	raw_ptr add_constructor(
		raw_ptr parent_data,
		const shared<meta_constructor>& constructor,
		const polymorph_factory_context& context,
		bool backward_iteration
	)
	{
		ctr_params.push_back(append_constructor(
			this->deref(parent_data),
			make_ctr_handler(this->deref(parent_data), constructor),
			backward_iteration
		));
		return raw_ptr(&ctr_params.back());
	}

	int index(void)
	{
		return handler->index();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

