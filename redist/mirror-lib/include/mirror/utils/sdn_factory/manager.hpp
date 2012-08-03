/**
 * .file mirror/utils/sdn_factory/manager.hpp
 * .brief Manager for the structured data notation factory plugin framework
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_MANAGER_1011291729_HPP
#define MIRROR_UTILS_SDN_MANAGER_1011291729_HPP

#include <memory>
#include <vector>
#include <list>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class sdn_fact_ctr_handler
 : public sdn_fact_handler_node<Traits>
{
private:
	typedef sdn_fact_param<Traits> param_type;

	typename Traits::shared_data& shared_data;
	typename Traits::shared_string_proxy type_name;
	typename Traits::shared_string_proxy object_name;
	std::size_t param_count;

	typedef sdn_fact_handler<Traits> base_intf;
	typedef sdn_fact_handler_node<Traits> base_class;
	typedef typename Traits::element element;

public:
	sdn_fact_ctr_handler(
		const param_type& parent_data,
		const std::string& type,
		const std::string& object,
		std::size_t count
	): shared_data(parent_data.shared_data())
	 , type_name(Traits::share_string(shared_data, type))
	 , object_name(Traits::share_string(shared_data, object))
	 , param_count(count)
	{ }

	double match(element elem)
	{
		double result = 1.0;
		auto i = this->subhandlers.begin(), e = this->subhandlers.end();
		std::size_t n = 1;
		while(i != e)
		{
			result *= (*i)->match(elem);
			if(result == 0.0) return 0.0;
			++i;
			++n;
		}
		return result * n;
	}
};

// manager's handler
template <class Traits>
class sdn_fact_manager_handler
 : public sdn_fact_handler_node<Traits>
{
private:
	typedef sdn_fact_handler<Traits> base_intf;
	typedef typename Traits::element element;
	typedef sdn_fact_param<Traits> param_type;

	typename Traits::shared_data& shared_data;

	// indices of constructors that match the input
	std::vector<std::size_t> match_indices;
public:
	sdn_fact_manager_handler(const param_type& parent_data)
	 : shared_data(parent_data.shared_data())
	{ }

	sdn_fact_manager_handler(typename Traits::shared_data& data)
	 : shared_data(data)
	{ }

	std::string expected(void) const
	{
		std::string result;
		auto i = this->subhandlers.begin(), e = this->subhandlers.end();
		while(i != e)
		{
			result.append((*i)->expected());
			++i;
			if(i != e)
			{
				result.append(" ");
				result.append(Traits::translate(
					shared_data,
					"OR"
				));
				result.append(" ");
			}
		}
		return result;
	}

	double match(element elem)
	{
		// reset the current indices
		match_indices.clear();
		// the current best match
		double best_match = 0.0;
		int n = 0;
		auto i = this->subhandlers.begin(), e = this->subhandlers.end();
		while(i != e)
		{
			// get the match result from each constructor handler
			double this_match = (*i)->match(elem);
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
			}
			++i;
			++n;
		}
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
				Traits::translate(
					shared_data,
					"No matching constructor for input"
				) + ". " +
				Traits::translate(shared_data, "Expecting:") +
				" " + this->expected() + "."
			);
		}
		// if we have multiple matching constructors
		if(match_indices.size() > 1)
		{
			std::string msg;
			msg.append(Traits::translate(
				shared_data,
				"Ambiguous input."
			));
			msg.append(" ");
			msg.append(Traits::translate(
				shared_data,
				"Candidates are:"
			));
			msg.append(" ");
			auto i = match_indices.begin(), e = match_indices.end();
			while(i != e)
			{
				msg.append(this->subhandlers[*i]->expected());
				++i;
				if(i != e)
				{
					msg.append(" ");
					msg.append(Traits::translate(
						shared_data,
						"OR"
					));
					msg.append(" ");
				}
			}
			msg.append(". ");
			msg.append(Traits::translate(
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
class sdn_fact_manuf<void, Traits>
{
private:
	typedef aux::sdn_fact_handler<Traits> handler_type;
	typedef aux::sdn_fact_param<Traits> param_type;

	// the manager's handler
	typedef aux::sdn_fact_manager_handler<Traits> manager_handler;
	std::shared_ptr<manager_handler> handler;

	// the constructor handlers
	std::list<std::shared_ptr<handler_type> > ctr_handlers;

	// helper used for constructing the parameter list string
	// for a constructor handler
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
	// for a constructor handler
	template <typename MetaParams>
	static std::string make_param_list(MetaParams)
	{
		std::string result;
		mp::for_each<MetaParams>(param_str_maker(result));
		return result;
	}

	template <class MetaClass, class CtrIndex>
	std::shared_ptr<handler_type> make_ctr_handler(
		const param_type& parent_data,
		MetaClass,
		CtrIndex
	)
	{
		typedef aux::sdn_fact_ctr_handler<Traits> constructor_handler;
		typedef parameters<
			typename mp::at<
				constructors<MetaClass>,
				CtrIndex
			>::type
		> params;
		return std::shared_ptr<handler_type>(
			new constructor_handler(
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
		const std::shared_ptr<handler_type>& ctr_handler,
		EnumInfo& enum_info
	)
	{
		if(enum_info.going_backward())
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
	template <typename Context>
	sdn_fact_manuf(const param_type& parent_data, Context)
	 : handler(new manager_handler(parent_data))
	{ }

	void finish(const param_type& parent_data)
	{
		auto i = ctr_handlers.begin(), e = ctr_handlers.end();
		while(i != e)
		{
			handler->add_subhandler(i->get());
			++i;
		}
		parent_data.handler().add_subhandler(handler.get());
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
			make_ctr_handler(parent_data, meta_product, ctr_index),
			constructor_info.enumeration_info
		);
	}

	inline int index(void)
	{
		return handler->index();
	}
};

MIRROR_NAMESPACE_END

#endif //include guard

