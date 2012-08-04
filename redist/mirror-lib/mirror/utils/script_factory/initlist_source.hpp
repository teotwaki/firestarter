/**
 * .file mirror/utils/script_factory/initlist_source.hpp
 * .brief Container manufacturer for the script-parsing factory generator plugin
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_INITLIST_SOURCE_1102071229_HPP
#define MIRROR_UTILS_SCRIPT_INITLIST_SOURCE_1102071229_HPP

#include <list>
#include <vector>
#include <type_traits>
#include <mirror/meta_prog/if.hpp>
#include <mirror/type_traits/is_default_constructible.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class script_fact_sequence_parser
 : public script_fact_parser<Traits>
{
private:
	typedef script_fact_parser<Traits> base_intf;
	typedef script_fact_param<Traits> param_type;

	Traits traits;

	typedef typename Traits::iterator iterator;

	typename Traits::shared_data& shared_data;
	base_intf* subparser;

	typedef std::pair<iterator, iterator> iterator_pair;
	typedef std::vector<iterator_pair> iterator_seq;
	iterator_seq literals;
public:
	script_fact_sequence_parser(const param_type& parent_data)
	 : shared_data(parent_data.shared_data())
	 , subparser(nullptr)
	{ }

	std::string expected(void) const
	{
		return traits.translate(shared_data, "sequence");
	}

	double match(iterator& begin, iterator end)
	{
		literals.clear();
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "{")) return 0.0;
		traits.skip_space(begin, end);
		double res = 1.0;
		assert(subparser != nullptr);
		iterator temp = begin;
		while((res *= subparser->match(temp, end)) > 0.0)
		{
			literals.push_back(iterator_pair(begin, temp));
			traits.skip_space(temp, end);
			begin = temp;
			if(!traits.match(temp, end, ",")) break;
			traits.skip_space(temp, end);
			begin = temp;
		}
		if(res == 0.0) return 0.0;
		traits.skip_space(begin, end);
		if(!traits.match(begin, end, "}")) return 0.0;
		return res;
	}

	void add_subparser(base_intf* parser)
	{
		assert(parser != nullptr);
		assert(subparser == nullptr);
		subparser = parser;
	}

	typename iterator_seq::const_iterator
	literals_begin(void) const
	{
		return literals.begin();
	}

	typename iterator_seq::const_iterator
	literals_end(void) const
	{
		return literals.end();
	}

	void reset(typename iterator_seq::const_iterator iters)
	{
		assert(subparser != nullptr);
		assert(iters != literals.end());
		iterator begin = iters->first;
		iterator end = iters->second;
		double res = subparser->match(begin, end);
		assert(res > 0.0);
	}
protected:
	void print(
		std::vector<bool>& conns,
		std::ostream& out,
		bool last
	)
	{
		assert(subparser != nullptr);
		subparser->print(conns, out, last);
	}
};

template <class Element, class Traits>
class script_fact_seq_source
{
private:
	typedef script_fact_param<Traits> param_type;

	typedef script_fact_sequence_parser<Traits> sequence_parser;
	std::shared_ptr<sequence_parser> parser;

	typedef typename mirror::factory_maker<
		script_fact_manuf,
		script_fact_suppl,
		script_fact_enum,
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
	inline script_fact_seq_source(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): parser(new sequence_parser(parent_data))
	 , fact(param_type(parser.get(), parent_data), construction_info)
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.parser().add_subparser(parser.get());
	}

	inline element_seq operator()(void)
	{
		element_seq result;
		auto	lits_i = parser->literals_begin(),
			lits_e = parser->literals_end();
		while(lits_i != lits_e)
		{
			parser->reset(lits_i);
			result.push_back(fact());
			++lits_i;
		}
		return result;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

