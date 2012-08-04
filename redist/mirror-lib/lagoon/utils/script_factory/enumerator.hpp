/**
 * .file lagoon/utils/script_factory/enumerator.hpp
 * .brief Script-parsing polymorphic factory enumerator
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_ENUMERATOR_1101181725_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_ENUMERATOR_1101181725_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN

// Implementation of the polymorph factory manufacturer interface
template <class Traits>
class script_fact_enumerator
 : public polymorph_factory_enumerator
 , public aux::script_fact_utils<Traits>
{
private:
	typedef mirror::aux::script_fact_param<Traits> param_type;
	typedef mirror::aux::script_fact_parser<Traits> parser_type;
	typedef mirror::aux::script_fact_enum_parser<Traits> enum_parser;

	struct wrapper_type
	{
		shared<meta_enum> enumeration;

		wrapper_type(const shared<meta_enum>& e)
		 : enumeration(e)
		{
			assert(enumeration);
		}

		std::string operator()(int index)
		{
			return enumeration->value_name(index);
		}
	} wrapper;
	std::shared_ptr<enum_parser> parser;
public:
	script_fact_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): wrapper(param->type().as<meta_enum>())
	 , parser(new enum_parser(
		this->deref(parent_data),
		wrapper.enumeration->full_name(),
		wrapper.enumeration->size(),
		std::ref(wrapper)
	))
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).parser().add_subparser(parser.get());
	}

	int create(void)
	{
		return int(wrapper.enumeration->value(parser->value_index()));
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

