/**
 * .file mirror/utils/script_factory/manager.hpp
 * .brief Manager for the script-parsing factory generator plugin
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_MANAGER_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_MANAGER_1011291729_HPP

#include <memory>
#include <vector>
#include <list>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class script_fact_ctr_parser
 : public script_fact_parser_node<Traits>
{
private:
	typedef script_fact_param<Traits> param_type;

	Traits traits;

	typename Traits::shared_data& shared_data;
	typename Traits::shared_string_proxy type_name;
	typename Traits::shared_string_proxy param_typelist;
	std::size_t param_count;

	typedef script_fact_parser<Traits> base_intf;
	typedef typename Traits::iterator iterator;

	double match_params(iterator& current, iterator end)
	{
		double result = 1.0;
		auto i = this->subparsers.begin(), e = this->subparsers.end();
		while(i != e)
		{
			traits.skip_space(current, end);
			result *= (*i)->match(current, end);
			if(result == 0.0) return 0.0;
			traits.skip_space(current, end);
			++i;
			if(i != e)
			{
				if(!traits.match(current, end, ","))
					return 0.0;
				traits.skip_space(current, end);
			}
		}
		return result;
	}

	double match_explicit(iterator& current, iterator end)
	{
		traits.skip_space(current, end);
		if(!traits.match_skip_space(
			current,
			end,
			traits.get_shared_string(shared_data, type_name)
		)) return 0.0;
		traits.skip_space(current, end);
		if(!traits.match(current, end, "(")) return 0.0;
		if(match_params(current, end) == 0.0) return 0.0;
		traits.skip_space(current, end);
		if(!traits.match(current, end, ")")) return 0.0;
		traits.skip_space(current, end);
		return 1.0;
	}

	double match_initializer(iterator& current, iterator end)
	{
		traits.skip_space(current, end);
		if(!traits.match(current, end, "{")) return 0.0;
		double res = match_params(current, end);
		if(res == 0.0) return 0.0;
		traits.skip_space(current, end);
		if(!traits.match(current, end, "}")) return 0.0;
		traits.skip_space(current, end);
		return res;
	}

public:
	script_fact_ctr_parser(
		const param_type& parent_data,
		const std::string& name,
		const std::string& params,
		std::size_t count
	): shared_data(parent_data.shared_data())
	 , type_name(traits.share_string(shared_data, name))
	 , param_typelist(traits.share_string(shared_data, params))
	 , param_count(count)
	{ }

	std::string expected(void) const
	{
		return traits.get_shared_string(shared_data, type_name) +
		"(" +
			traits.get_shared_string(shared_data, param_typelist) +
		")";
	}

	double match(iterator& begin, iterator end)
	{
		iterator current = begin;
		double result = match_explicit(current, end);
		if(result == 0.0)
		{
			current = begin;
			result = match_initializer(current, end);
			if(result == 0.0)
			{
				if(this->subparsers.size() == 1)
				{
					current = begin;
					result = match_params(current, end);
				}
				if(result == 0.0) return 0.0;
			}
		}
		traits.skip_space(current, end);
		begin = current;
		return result;
	}
};

// manager's parser
template <class Traits>
class script_fact_manager_parser
 : public script_fact_parser_node<Traits>
{
private:
	typedef script_fact_parser<Traits> base_intf;
	typedef typename Traits::iterator iterator;
	typedef script_fact_param<Traits> param_type;

	Traits traits;

	typename Traits::shared_data& shared_data;

	// indices of constructors that match the input
	std::vector<std::size_t> match_indices;
	std::string input_string;
public:
	script_fact_manager_parser(const param_type& parent_data)
	 : shared_data(parent_data.shared_data())
	{ }

	script_fact_manager_parser(typename Traits::shared_data& data)
	 : shared_data(data)
	{ }

	std::string expected(void) const
	{
		std::string result;
		auto i = this->subparsers.begin(), e = this->subparsers.end();
		while(i != e)
		{
			result.append((*i)->expected());
			++i;
			if(i != e)
			{
				result.append(" ");
				result.append(traits.translate(
					shared_data,
					"OR"
				));
				result.append(" ");
			}
		}
		return result;
	}

	double match(iterator& begin, iterator end)
	{
		// reset the current indices
		match_indices.clear();
		input_string.assign(begin, end);
		// the current best match
		double best_match = 0.0;
		iterator best_pos = begin;
		int n = 0;
		auto i = this->subparsers.begin(), e = this->subparsers.end();
		while(i != e)
		{
			iterator current = begin;
			// get the match result from each constructor parser
			double this_match = (*i)->match(current, end);
			// if we found a constructor with the same match result
			if((best_match == this_match) && (best_match > 0.0))
			{
				// remember its index
				match_indices.push_back(n);
			}
			// if we found a better constructor;
			else if(best_match < this_match)
			{
				// forget about the previous and remember
				// the current
				match_indices.clear();
				match_indices.push_back(n);
				best_match = this_match;
				best_pos = current;
			}
			++i;
			++n;
		}
		begin = best_pos;
		return best_match;
	}

	int index(void) const
	{
		// if we have exactly one constructor index
		if(match_indices.size() == 1)
		{
			// then return it
			return match_indices.front();
		}
		// if we have nothing
		if(match_indices.empty())
		{
			throw std::runtime_error(
				traits.translate(
					shared_data,
					"No matching constructor for input"
				) + " '" + input_string + "'. " +
				traits.translate(
					shared_data,
					"Expecting:"
				) + " " + this->expected() + "."
			);
		}
		// if we have multiple matching constructors
		if(match_indices.size() > 1)
		{
			std::string msg;
			msg.append(traits.translate(
				shared_data,
				"Ambiguous input."
			));
			msg.append(" ");
			msg.append(traits.translate(
				shared_data,
				"Candidates are:"
			));
			msg.append(" ");
			auto i = match_indices.begin(), e = match_indices.end();
			while(i != e)
			{
				msg.append(this->subparsers[*i]->expected());
				++i;
				if(i != e)
				{
					msg.append(" ");
					msg.append(traits.translate(
						shared_data,
						"OR"
					));
					msg.append(" ");
				}
			}
			msg.append(". ");
			msg.append(traits.translate(
				shared_data,
				"You may need to add explicit casts "
				"to disambiguate."
			));
			throw std::runtime_error(msg);
		}
		assert(!"Never should get here!");
		return 0;
	}
};

} // namespace aux

template <class Traits>
class script_fact_manuf<void, Traits>
{
private:
	typedef aux::script_fact_parser<Traits> parser_type;
	typedef aux::script_fact_param<Traits> param_type;

	// the manager's parser
	typedef aux::script_fact_manager_parser<Traits> manager_parser;
	std::shared_ptr<manager_parser> parser;

	// the constructor parsers
	std::list<std::shared_ptr<parser_type> > ctr_parsers;

	// helper used for constructing the parameter list string
	// for a constructor parser
	struct param_str_maker
	{
		std::string& result;

		param_str_maker(std::string& res)
		 : result(res)
		{ }

		template <typename MetaParam>
		void operator()(MetaParam) const
		{
			if(!result.empty()) result.append(", ");
			result.append(MetaParam::type::local_name());
		}
	};

	// helper function constructing the parameter list string
	// for a constructor parser
	template <typename MetaParams>
	static std::string make_param_list(MetaParams)
	{
		std::string result;
		mp::for_each<MetaParams>(param_str_maker(result));
		return result;
	}

	template <class MetaClass, class CtrIndex>
	std::shared_ptr<parser_type> make_ctr_parser(
		const param_type& parent_data,
		MetaClass,
		CtrIndex
	)
	{
		typedef aux::script_fact_ctr_parser<Traits> constructor_parser;
		typedef parameters<
			typename mp::at<
				constructors<MetaClass>,
				CtrIndex
			>::type
		> params;
		return std::shared_ptr<parser_type>(
			new constructor_parser(
				parent_data,
				MetaClass::local_name(),
				make_param_list(params()),
				mp::size<params>::value
			)
		);
	}

	template <class EnumInfo>
	param_type append_constructor(
		const param_type& parent_data,
		const std::shared_ptr<parser_type>& ctr_parser,
		EnumInfo& enum_info
	)
	{
		if(enum_info.going_backward())
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
	template <typename Context>
	script_fact_manuf(const param_type& parent_data, Context)
	 : parser(new manager_parser(parent_data))
	{ }

	void finish(const param_type& parent_data)
	{
		auto i = ctr_parsers.begin(), e = ctr_parsers.end();
		while(i != e)
		{
			parser->add_subparser(i->get());
			++i;
		}
		parent_data.parser().add_subparser(parser.get());
	}

	template <class ConstructorInfo>
	inline param_type add_constructor(
		const param_type& parent_data,
		ConstructorInfo constructor_info
	)
	{
		typename ConstructorInfo::product meta_product;
		typename ConstructorInfo::constructor_index ctr_index;
		return append_constructor(
			parent_data,
			make_ctr_parser(parent_data, meta_product, ctr_index),
			constructor_info.enumeration_info
		);
	}

	inline int index(void)
	{
		return parser->index();
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

