/**
 * .file lagoon/utils/script_factory/manufacturer.hpp
 * .brief Script-parsing polymorphic factory manufacturers
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SCRIPT_FACTORY_MANUFACTURER_1011291729_HPP
#define LAGOON_UTILS_SCRIPT_FACTORY_MANUFACTURER_1011291729_HPP

#include <cassert>
#include <mirror/utils/string/unescape.hpp>

LAGOON_NAMESPACE_BEGIN
namespace aux {

// Implementation of the polymorph factory manufacturer interface
template <class Product, class Traits>
class script_fact_manuf_native
 : public polymorph_factory_manufacturer<Product>
 , public aux::script_fact_utils<Traits>
{
private:
	typedef mirror::aux::script_fact_param<Traits> param_type;
	typedef mirror::aux::script_fact_parser<Traits> parser_type;
	typedef mirror::aux::script_fact_native_parser<Product, Traits>
		native_parser;

	std::shared_ptr<native_parser> parser;

	template <typename T>
	inline T do_create(T*)
	{
		typedef typename Traits::iterator iterator;
		iterator i = parser->lit_begin(), e = parser->lit_end();
		mirror::lex_conv<std::string, Product> conv;
		return conv.convert(i, e);
	}

	std::string do_create(std::string*)
	{
		return mirror::aux::unescape(
			parser->lit_begin(),
			parser->lit_end()
		);
	}
public:
	script_fact_manuf_native(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): parser(new native_parser(this->deref(parent_data)))
	{ }

	void finish(raw_ptr parent_data)
	{
		this->deref(parent_data).parser().add_subparser(parser.get());
	}

	Product create(void)
	{
		return do_create((Product*)nullptr);
	}
};

} // namespace aux

template <class Product, class Traits>
class script_fact_manufacturer
 : public aux::script_fact_manuf_native <Product, Traits>
{
public:
	script_fact_manufacturer(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	): aux::script_fact_manuf_native<Product, Traits>(
		parent_data,
		param,
		context
	){ }
};

LAGOON_NAMESPACE_END

#endif //include guard

