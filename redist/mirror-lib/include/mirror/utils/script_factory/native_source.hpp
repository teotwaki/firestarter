/**
 * .file mirror/utils/script_factory/native_source.hpp
 * .brief Native manufacturer for the script-parsing factory generator plugin
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_NATIVE_SOURCE_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_NATIVE_SOURCE_1011291729_HPP

#include <cassert>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Product, class Traits>
class script_fact_native_parser
 : public script_fact_parser<Traits>
{
private:
	typedef script_fact_parser<Traits> base_intf;
	typedef script_fact_param<Traits> param_type;
	typedef typename Traits::iterator iterator;

	Traits traits;

	typename Traits::shared_data& shared_data;
	typename Traits::shared_string_proxy type_name;
	iterator literal_begin;
	iterator literal_end;
	int expression_id;

	template <typename T>
	double match_literal(iterator& begin, iterator end, T* x)
	{
		traits.skip_space(begin, end);
		double res = traits.match_lit(
			begin,
			end,
			literal_begin,
			literal_end,
			x
		);
		traits.skip_space(begin, end);
		return res;
	}

	template <typename T>
	double match_explicit(iterator& begin, iterator end, T* x)
	{
		traits.skip_space(begin, end);
		if(!traits.match(
			begin,
			end,
			traits.get_shared_string(shared_data, type_name)
		)) return 0.0;
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "(")) return 0.0;
		traits.skip_space(begin, end);
		if(match_literal(begin, end, x) == 0.0) return 0.0;
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, ")")) return 0.0;
		return 1.0;
	}

	template <typename T>
	double match_initializer(iterator& begin, iterator end, T* x)
	{
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "{")) return 0.0;
		traits.skip_space(begin, end);
		double res = match_literal(begin, end, x);
		if(res == 0.0) return 0.0;
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "}")) return 0.0;
		return res;
	}

	template <typename T>
	double match_expression(iterator& begin, iterator end, T* x)
	{
		traits.skip_space(begin, end);
		double res = traits.match_expr(
			begin,
			end,
			literal_begin,
			literal_end,
			expression_id,
			x
		);
		traits.skip_space(begin, end);
		return res;
		return 0.0;
	}

	template <typename T>
	double do_match(iterator& begin, iterator end, T* x)
	{
		iterator current = begin;
		expression_id = -1;
		double res = match_literal(current, end, x);
		if(res == 0.0)
		{
			current = begin;
			res = match_explicit(current, end, x);
			if(res == 0.0)
			{
				current = begin;
				res = match_initializer(current, end, x);
				if(res == 0.0)
				{
					current = begin;
					res = match_expression(current, end, x);
				}
			}
		}
		begin = current;
		traits.skip_space(begin, end);
		return res;
	}
public:
	script_fact_native_parser(const param_type& parent_data)
	 : shared_data(parent_data.shared_data())
	 , type_name(traits.share_string(
		shared_data,
		MIRRORED_TYPE(Product)::local_name()
	))
	{ }

	std::string expected(void) const
	{
		return traits.translate(
			shared_data,
			traits.get_shared_string(shared_data, type_name) +
			std::string(" literal")
		);
	}

	double match(iterator& begin, iterator end)
	{
		return do_match(begin, end, (Product*)nullptr);
	}

	inline iterator lit_begin(void) const
	{
		return literal_begin;
	}

	inline iterator lit_end(void) const
	{
		return literal_end;
	}

	inline bool is_literal(void) const
	{
		return expression_id < 0;
	}

	inline int expr_id(void) const
	{
		return expression_id;
	}
};

// Source for native types
template <class Product, class Traits>
class script_fact_source<
	Product,
	Traits,
	std::true_type
>
{
private:
	typedef script_fact_param<Traits> param_type;
	typedef script_fact_parser<Traits> parser_type;

	Traits traits;

	typedef script_fact_native_parser<Product, Traits> native_parser;
	std::shared_ptr<native_parser> parser;

	template <typename T>
	inline T create(T* selector)
	{
		return traits.convert(
			parser->lit_begin(),
			parser->lit_end(),
			selector
		);
	}

	template <typename T>
	inline T get(T* selector)
	{
		return traits.get(
			parser->lit_begin(),
			parser->lit_end(),
			parser->expr_id(),
			selector
		);
	}
public:
	template <class ConstructionInfo>
	script_fact_source(
		const param_type& parent_data,
		ConstructionInfo
	): parser(new native_parser(parent_data))
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.parser().add_subparser(parser.get());
	}

	inline Product operator()(void)
	{
		if(parser->is_literal())
			return create((Product*)nullptr);
		else return get((Product*)nullptr);
	}
};

} // namespace aux

MIRROR_NAMESPACE_END

#endif //include guard

