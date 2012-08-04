/**
 * @file mirror/utils/mijson_factory.hpp
 * @brief mijson-parser-based factory generator plugin
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_MIJSON_FACTORY_1011291729_HPP
#define MIRROR_UTILS_MIJSON_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sdn_factory.hpp>
#include <mirror/utils/mijson.hpp>

MIRROR_NAMESPACE_BEGIN

template <typename Iterator>
class sdn_fact_mijson_property;

template <typename Iterator>
class sdn_fact_mijson_element;

template <typename Iterator>
class sdn_fact_mijson_range;

template <typename Iterator>
class sdn_fact_mijson_property
{
private:
	typedef sdn_fact_mijson_property property;

	friend class sdn_fact_mijson_element<Iterator>;

	const mijson_element<Iterator>& attrib;

	sdn_fact_mijson_property(const mijson_element<Iterator>& _attrib)
	 : attrib(_attrib)
	{ }
public:
	operator bool (void) const
	{
		return true;
	}

	bool operator ! (void) const
	{
		return false;
	}

	bool has_type(const std::string& type_name)
	{
		return true;
	}

	inline std::string text(void)
	{
		return attrib.value();
	}
};

template <typename Iterator>
class sdn_fact_mijson_range
{
private:
	typedef sdn_fact_mijson_element<Iterator> element;
	typedef typename mijson_element<Iterator>::element_list element_list;
	typename element_list::const_iterator cur,end;
public:
	sdn_fact_mijson_range(const element_list& list)
	 : cur(list.begin())
	 , end(list.end())
	{ }

	bool empty(void) const
	{
		return cur == end;
	}

	void step_front(void)
	{
		assert(!empty());
		++cur;
	}

	element front(void) const;
};

template <typename Iterator>
class sdn_fact_mijson_element
{
private:
	typedef sdn_fact_mijson_range<Iterator> range;
	typedef sdn_fact_mijson_element<Iterator> element;
	typedef sdn_fact_mijson_property<Iterator> property;

	const mijson_element<Iterator>& node;
public:
	sdn_fact_mijson_element(const mijson_element<Iterator>& _node)
	 : node(_node)
	{ }

	operator bool (void) const
	{
		return true;
	}

	bool operator ! (void) const
	{
		return false;
	}

	bool has_type(const std::string& type_name)
	{
		return true;
	}

	range elements(void)
	{
		return range(node.elements());
	}

	element child(const std::string& name)
	{
		try
		{
			return element(node.member(name));
		}
		catch(std::runtime_error& e)
		{
			throw sdn_fact_node_not_found_error(e.what());
		}
	}

	property attribute(void)
	{
		try
		{
			assert(node.has_value());
			return property(node);
		}
		catch(std::runtime_error& e)
		{
			throw sdn_fact_node_not_found_error(e.what());
		}
	}

	property attribute(const std::string& name)
	{
		try
		{
			assert(!name.empty());
			return property(node.member(name));
		}
		catch(std::runtime_error& e)
		{
			throw sdn_fact_node_not_found_error(e.what());
		}
	}
};

template <typename Iterator>
sdn_fact_mijson_element<Iterator>
sdn_fact_mijson_range<Iterator>::front(void) const
{
	assert(!empty());
	return sdn_fact_mijson_element<Iterator>(*cur);
}

typedef sdn_fact_traits<
	sdn_fact_mijson_range<const char*>,
	sdn_fact_mijson_element<const char*>,
	sdn_fact_mijson_property<const char*>
> sdn_fact_mijson_traits;

typedef factory_maker<
	sdn_fact_manuf,
	sdn_fact_suppl,
	sdn_fact_enum,
	sdn_fact_mijson_traits
> mijson_factory_maker;

typedef sdn_fact_data<
	sdn_fact_mijson_traits
> mijson_factory_input;

MIRROR_NAMESPACE_END

#endif //include guard

