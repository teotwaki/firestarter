/**
 * .file lagoon/utils/script_factory/arrayer.hpp
 * .brief Script-parsing polymorphic factory arrayer
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_ARRAYER_1103101220_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_ARRAYER_1103101220_HPP

#include <cassert>
#include <memory>
#include <stdexcept>

LAGOON_NAMESPACE_BEGIN

template <class Traits>
class script_fact_arrayer
 : public polymorph_factory_arrayer
 , public aux::script_fact_utils<Traits>
{
private:
	typedef mirror::aux::script_fact_param<Traits> param_type;
	typedef mirror::aux::script_fact_parser<Traits> parser_type;
	typedef mirror::aux::script_fact_sequence_parser<Traits> seq_parser;

	typedef typename polymorph_factory_arrayer::element_producer
		element_producer;

	std::shared_ptr<seq_parser> parser;
	param_type param;
public:
	script_fact_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	): parser(new seq_parser(this->deref(parent_data)))
	 , param(parser.get(), this->deref(parent_data))
	{ }

	raw_ptr data(void)
	{
		return raw_ptr(&param);
	}

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).parser().add_subparser(parser.get());
	}

	void create(element_producer& producer)
	{
		auto	lits_i = parser->literals_begin(),
			lits_e = parser->literals_end();
		while(lits_i != lits_e)
		{
			parser->reset(lits_i);
			producer.make_next();
			++lits_i;
		}
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

