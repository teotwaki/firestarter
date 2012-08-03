/**
 * .file mirror/utils/sdn_factory/native_source.hpp
 * .brief Native manufacturer for the structured data factory plugin framework
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_NATIVE_SOURCE_1011291729_HPP
#define MIRROR_UTILS_SDN_NATIVE_SOURCE_1011291729_HPP

#include <mirror/utils/lex_match.hpp>
#include <mirror/utils/lex_conv.hpp>

#include <functional>
#include <cassert>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class sdn_fact_native_handler
 : public sdn_fact_handler<Traits>
{
private:
	typedef sdn_fact_handler<Traits> base_intf;
	typedef sdn_fact_param<Traits> param_type;
	typedef typename Traits::element element;
	typedef typename Traits::property property;

	typename Traits::shared_data& shared_data;
	typename Traits::shared_string_proxy type_name;
	typename Traits::shared_string_proxy object_name;
	std::function<double (const std::string&)> lex_matcher;

	std::string literal_str;
public:
	sdn_fact_native_handler(
		const param_type& parent_data,
		const std::string& type,
		const std::string& object,
		const std::function<double(const std::string&)>& matcher
	): shared_data(parent_data.shared_data())
	 , type_name(Traits::share_string(shared_data, type))
	 , object_name(Traits::share_string(shared_data, object))
	 , lex_matcher(matcher)
	{
		assert(lex_matcher);
	}

	std::string expected(void) const
	{
		return "{" + Traits::translate(
			shared_data,
			Traits::get_shared_string(shared_data, object_name)
		) + ": " + Traits::translate(
			shared_data,
			Traits::get_shared_string(shared_data, type_name)
		) + "}";
	}

	template <typename Attr>
	double match_leaf(Attr attr, const std::string& loc_type_name)
	{
		if(attr && attr.has_type(loc_type_name))
		{
			literal_str.assign(attr.text());
			double res = lex_matcher(literal_str);
			if(res > 0.0) return res;
			else literal_str.clear();
		}
		return 0.0;
	}

	double match(element elem)
	{
		try
		{
			std::string name = Traits::get_shared_string(
				shared_data,
				object_name
			);
			return match_leaf(
				name.empty()?
				elem.attribute():
				elem.attribute(
					Traits::translate(
						shared_data,
						name
					)
				),
				Traits::translate(
					shared_data,
					Traits::get_shared_string(
						shared_data,
						type_name
					)
				)
			);
		}
		catch(sdn_fact_node_not_found_error&)
		{
			return 0.0;
		}
	}

	inline const std::string& literal(void) const
	{
		return literal_str;
	}
};

// Source for native types
template <class Product, class Traits>
class sdn_fact_source<
	Product,
	Traits,
	std::true_type
>
{
private:
	typedef sdn_fact_param<Traits> param_type;
	typedef sdn_fact_handler<Traits> handler_type;

	typedef sdn_fact_native_handler<Traits> native_handler;
	lex_match<Product> matcher;
	std::shared_ptr<native_handler> handler;
public:
	template <class ConstructionInfo>
	sdn_fact_source(
		const param_type& parent_data,
		ConstructionInfo
	): handler(new native_handler(
		parent_data,
		ConstructionInfo::parameter::type::local_name(),
		ConstructionInfo::parameter::base_name(),
		std::ref(matcher)
	))
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.handler().add_subhandler(handler.get());
	}

	inline Product operator()(void)
	{
		lex_conv<std::string, Product> conv;
		return conv(handler->literal());
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

