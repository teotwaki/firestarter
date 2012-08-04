/**
 * @file mirror/utils/rapidxml_factory.hpp
 * @brief RapidXML-based factory generator plugin
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_RAPIDXML_FACTORY_1011291729_HPP
#define MIRROR_UTILS_RAPIDXML_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sdn_factory.hpp>
#include <rapidxml.hpp>

MIRROR_NAMESPACE_BEGIN

class sdn_fact_rapidxml_property;
class sdn_fact_rapidxml_element;
class sdn_fact_rapidxml_range;

class sdn_fact_rapidxml_property
{
private:
	typedef sdn_fact_rapidxml_property property;
	typedef rapidxml::xml_attribute<> xml_attrib;
	xml_attrib* attrib;

	friend class sdn_fact_rapidxml_element;

	sdn_fact_rapidxml_property(xml_attrib* _attrib)
	 : attrib(_attrib)
	{ }
public:
	operator bool (void) const
	{
		return attrib != nullptr;
	}

	bool operator ! (void) const
	{
		return attrib == nullptr;
	}

	bool has_type(const std::string& type_name)
	{
		return true;
	}

	std::string text(void)
	{
		assert(attrib != nullptr);
		return std::string(
			attrib->value(),
			attrib->value() +
			attrib->value_size()
		);
	}
};

class sdn_fact_rapidxml_range
{
private:
	typedef sdn_fact_rapidxml_element element;
	typedef rapidxml::xml_node<> xml_node;
	xml_node* node;
public:
	sdn_fact_rapidxml_range(xml_node* _node)
	 : node(_node)
	{ }

	bool empty(void) const
	{
		return node == nullptr;
	}

	void step_front(void)
	{
		assert(!empty());
		node = node->next_sibling();
	}

	element front(void) const;
};

class sdn_fact_rapidxml_element
{
private:
	typedef sdn_fact_rapidxml_range range;
	typedef sdn_fact_rapidxml_element element;
	typedef sdn_fact_rapidxml_property property;
	typedef rapidxml::xml_node<> xml_node;
	xml_node* node;
public:
	sdn_fact_rapidxml_element(rapidxml::xml_document<>& doc)
	 : node(doc.first_node())
	{
	}

	sdn_fact_rapidxml_element(xml_node* _node)
	 : node(_node)
	{
	}

	operator bool (void) const
	{
		return node != nullptr;
	}

	bool operator ! (void) const
	{
		return node == nullptr;
	}

	bool has_type(const std::string& type_name)
	{
		assert(node != nullptr);
		// TODO: check a type attribute if present ?
		return true;
	}

	range elements(void) const
	{
		assert(node != nullptr);
		return range(node->first_node());
	}

	element child(const std::string& name)
	{
		assert(node != nullptr);
		return element(node->first_node(name.c_str()));
	}

	property attribute(void)
	{
		assert(node != nullptr);
		return property(node->first_attribute());
	}

	property attribute(const std::string& name)
	{
		assert(node != nullptr);
		assert(!name.empty());
		return property(node->first_attribute(name.c_str()));
	}
};

inline sdn_fact_rapidxml_element sdn_fact_rapidxml_range::front(void) const
{
	assert(!empty());
	return sdn_fact_rapidxml_element(node);
}

typedef sdn_fact_traits<
	sdn_fact_rapidxml_range,
	sdn_fact_rapidxml_element,
	sdn_fact_rapidxml_property
> sdn_fact_rapidxml_traits;

typedef factory_maker<
	sdn_fact_manuf,
	sdn_fact_suppl,
	sdn_fact_enum,
	sdn_fact_rapidxml_traits
> rapidxml_factory_maker;

typedef sdn_fact_data<
	sdn_fact_rapidxml_traits
> rapidxml_factory_input;

MIRROR_NAMESPACE_END

#endif //include guard

