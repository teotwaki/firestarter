/**
 * .file mirror/utils/script_factory/enumerator.hpp
 * .brief Enumerator for the script-parsing factory generator plugin
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_ENUMERATOR_1101180728_HPP
#define MIRROR_UTILS_SCRIPT_ENUMERATOR_1101180728_HPP

#include <mirror/utils/lex_match.hpp>
#include <mirror/utils/lex_conv.hpp>
#include <functional>
#include <cassert>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class script_fact_enum_parser
 : public script_fact_parser<Traits>
{
private:
	typedef script_fact_parser<Traits> base_intf;
	typedef script_fact_param<Traits> param_type;

	Traits traits;

	typedef typename Traits::iterator iterator;

	typename Traits::shared_data& shared_data;
	typename Traits::shared_string_proxy type_name;
	int found_index;

	const int value_count;
	std::function<std::string (int)> value_name;

	double match_literal(iterator& begin, iterator end)
	{
		traits.skip_space(begin, end);
		found_index = -1;
		double max = 0.0;
		for(int i=0; i!=value_count; ++i)
		{
			double res = traits.match(
				begin,
				end,
				value_name(i)
			);
			if(max < res)
			{
				max = res;
				found_index = i;
			}
		}
		traits.skip_space(begin, end);
		return max;
	}

	double match_explicit(iterator& begin, iterator end)
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
		if(match_literal(begin, end) == 0.0) return 0.0;
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, ")")) return 0.0;
		return 1.0;
	}

	double match_initializer(iterator& begin, iterator end)
	{
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "{")) return 0.0;
		traits.skip_space(begin, end);
		double res = match_literal(begin, end);
		if(res == 0.0) return 0.0;
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "}")) return 0.0;
		return res;
	}

	double do_match(iterator& begin, iterator end)
	{
		iterator current = begin;
		double res = match_literal(current, end);
		if(res == 0.0)
		{
			current = begin;
			res = match_explicit(current, end);
			if(res == 0.0)
			{
				current = begin;
				res = match_initializer(current, end);
			}
		}
		begin = current;
		traits.skip_space(begin, end);
		return res;
	}
public:
	script_fact_enum_parser(
		const param_type& parent_data,
		const std::string& local_name,
		const int count,
		const std::function<std::string (int)>& namer
	): shared_data(parent_data.shared_data())
	 , type_name(
		traits.share_string(
			shared_data,
			local_name
		)
	), found_index(-1)
	 , value_count(count)
	 , value_name(namer)
	{ }

	std::string expected(void) const
	{
		std::string result;
		for(int i=0; i!=value_count; ++i)
		{
			if(!result.empty())
			{
				result.append(" ");
				result.append(
					traits.translate(
						shared_data,
						"OR"
					)
				);
				result.append(" ");
			}
			result.append(
				traits.translate(
					shared_data,
					value_name(i)
				)
			);
		}
		return result;
	}

	double match(iterator& begin, iterator end)
	{
		return do_match(begin, end);
	}

	inline int value_index(void) const
	{
		return found_index;
	}
};

} // namespace aux

// Source for enumerated types
template <class Product, class Traits>
class script_fact_enum
{
private:
	typedef aux::script_fact_param<Traits> param_type;
	typedef aux::script_fact_parser<Traits> parser_type;

	typedef aux::script_fact_enum_parser<Traits> enum_parser;
	struct
	{
		std::string operator()(int index) const
		{
			return MIRRORED_ENUM(Product)::value_name(index);
		}
	} value_name;

	std::shared_ptr<enum_parser> parser;
public:
	template <class ConstructionInfo>
	script_fact_enum(
		const param_type& parent_data,
		ConstructionInfo
	): parser(new enum_parser(
		parent_data,
		MIRRORED_TYPE(Product)::local_name(),
		MIRRORED_ENUM(Product)::size::value,
		std::ref(value_name)
	))
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.parser().add_subparser(parser.get());
	}

	inline Product operator()(void)
	{
		return MIRRORED_ENUM(Product)::value(
			parser->value_index()
		);
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

