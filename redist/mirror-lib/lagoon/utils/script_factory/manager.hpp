/**
 * .file lagoon/utils/script_factory/manager.hpp
 * .brief Script-parsing polymorphic factory manager
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_MANAGER_1011291729_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_MANAGER_1011291729_HPP

#include <list>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class script_fact_manager
 : public polymorph_factory_manager
 , public aux::script_fact_utils<Traits>
{
private:
	typedef mirror::aux::script_fact_param<Traits> param_type;
	typedef mirror::aux::script_fact_parser<Traits> parser_type;
	typedef mirror::aux::script_fact_manager_parser<Traits> manager_parser;

	std::shared_ptr<manager_parser> parser;

	std::list<std::shared_ptr<parser_type> > ctr_parsers;
	// internal list of parameters passed to the subparsers
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

	std::shared_ptr<parser_type> make_ctr_parser(
		const param_type& parent_data,
		const shared<meta_constructor>& constructor
	)
	{
		typedef mirror::aux::script_fact_ctr_parser<Traits>
			constructor_parser;
		auto params = constructor->parameters();
		return std::shared_ptr<parser_type>(
			new constructor_parser(
				parent_data,
				constructor->result_type()->full_name(),
				make_param_list(params),
				params.size()
			)
		);
	}

	param_type append_constructor(
		const param_type& parent_data,
		const std::shared_ptr<parser_type>& ctr_parser,
		bool backward_iteration
	)
	{
		if(backward_iteration)
		{
			ctr_parsers.push_front(ctr_parser);
			return param_type(
				ctr_parsers.front().get(),
				parent_data
			);
		}
		else
		{
			ctr_parsers.push_back(ctr_parser);
			return param_type(
				ctr_parsers.back().get(),
				parent_data
			);
		}
	}
public:
	script_fact_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	): parser(new manager_parser(this->deref(parent_data).shared_data()))
	{ }

	void finish(raw_ptr parent_data)
	{
		auto i = ctr_parsers.begin(), e = ctr_parsers.end();
		while(i != e)
		{
			parser->add_subparser(i->get());
			++i;
		}
		this->deref(parent_data).parser().add_subparser(parser.get());
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
			make_ctr_parser(this->deref(parent_data), constructor),
			backward_iteration
		));
		return raw_ptr(&ctr_params.back());
	}

	int index(void)
	{
		return parser->index();
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

