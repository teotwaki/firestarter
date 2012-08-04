/**
 * .file lagoon/utils/sdn_factory/enumerator.hpp
 * .brief Script-parsing polymorphic factory enumerators
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SDN_FACTORY_ENUMERATOR_1101191723_HPP
#define LAGOON_UTILS_SDN_FACTORY_ENUMERATOR_1101191723_HPP

#include <cassert>

LAGOON_NAMESPACE_BEGIN

// Implementation of the polymorph factory manufacturer interface
template <class Traits>
class sdn_fact_enumerator
 : public polymorph_factory_enumerator
 , public aux::sdn_fact_utils<Traits>
{
private:
	typedef mirror::aux::sdn_fact_param<Traits> param_type;
	typedef mirror::aux::sdn_fact_handler<Traits> handler_type;
	typedef mirror::aux::sdn_fact_native_handler<Traits> native_handler;

	struct wrapper_type
	{
		shared<meta_enum> enumeration;

		wrapper_type(const shared<meta_enum>& e)
		 : enumeration(e)
		{
			assert(enumeration);
		}

		double operator()(const std::string& str)
		{
			return enumeration->has_value_name(str) ? 1.0 : 0.0;
		}
	} wrapper;
	std::shared_ptr<native_handler> handler;
public:
	sdn_fact_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): wrapper(param->type().as<meta_enum>())
	 , handler(
		new native_handler(
			this->deref(parent_data),
			param->type()->full_name(),
			param->base_name(),
			std::ref(wrapper)
		)
	)
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).handler().add_subhandler(handler.get());
	}

	int create(void)
	{
		return wrapper.enumeration->value_by_name(handler->literal());
	}
};

LAGOON_NAMESPACE_END

#endif //include guard

