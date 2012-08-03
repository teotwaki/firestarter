/**
 * .file mirror/utils/sdn_factory/initlist_source.hpp
 * .brief Container manufacturer for the SDN factory generator plugin
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_INITLIST_SOURCE_1102081307_HPP
#define MIRROR_UTILS_SDN_INITLIST_SOURCE_1102081307_HPP

#include <list>
#include <vector>
#include <type_traits>
#include <mirror/meta_prog/if.hpp>
#include <mirror/type_traits/is_default_constructible.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class sdn_fact_sequence_handler
 : public sdn_fact_handler<Traits>
{
private:
	typedef sdn_fact_handler<Traits> base_intf;
	typedef sdn_fact_param<Traits> param_type;
	typedef typename Traits::element element;
	typedef typename Traits::property property;

	typename Traits::shared_data& shared_data;
	base_intf* subhandler;

	typedef std::list<element> element_seq;
	element_seq elements;
public:
	sdn_fact_sequence_handler(const param_type& parent_data)
	 : shared_data(parent_data.shared_data())
	 , subhandler(nullptr)
	{ }

	std::string expected(void) const
	{
		return Traits::translate(shared_data, "sequence");
	}

	double match(element elem)
	{
		elements.clear();
		auto elems = elem.elements();
		double res = 1.0;
		assert(subhandler != nullptr);
		while(!elems.empty())
		{
			res *= subhandler->match(elems.front());
			if(res > 0.0)
			{
				elements.push_back(elems.front());
				elems.step_front();
			}
			else return 0.0;
		}
		return res;
	}

	void add_subhandler(base_intf* handler)
	{
		assert(handler != nullptr);
		assert(subhandler == nullptr);
		subhandler = handler;
	}

	typename element_seq::const_iterator
	elements_begin(void) const
	{
		return elements.begin();
	}

	typename element_seq::const_iterator
	elements_end(void) const
	{
		return elements.end();
	}

	void reset(typename element_seq::const_iterator elems)
	{
		assert(subhandler != nullptr);
		assert(elems != elements.end());
		double res = subhandler->match(*elems);
		assert(res > 0.0);
	}
};

template <class Element, class Traits>
class sdn_fact_seq_source
{
private:
	typedef sdn_fact_param<Traits> param_type;

	typedef sdn_fact_sequence_handler<Traits> sequence_handler;
	std::shared_ptr<sequence_handler> handler;

	typedef typename mirror::factory_maker<
		sdn_fact_manuf,
		sdn_fact_suppl,
		sdn_fact_enum,
		Traits
	> maker;
	typename maker::template factory< Element >::type fact;

	typedef typename mp::if_c<
		(std::is_pod<Element>::value || sizeof(Element) < 16) &&
		mirror::is_default_constructible<Element>::value,
		std::vector<Element>,
		std::list<Element>
	>::type element_seq;
public:
	template <class ConstructionInfo>
	inline sdn_fact_seq_source(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): handler(new sequence_handler(parent_data))
	 , fact(param_type(handler.get(), parent_data), construction_info)
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.handler().add_subhandler(handler.get());
	}

	inline element_seq operator()(void)
	{
		element_seq result;
		auto	elems_i = handler->elements_begin(),
			elems_e = handler->elements_end();
		while(elems_i != elems_e)
		{
			handler->reset(elems_i);
			result.push_back(fact());
			++elems_i;
		}
		return result;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

