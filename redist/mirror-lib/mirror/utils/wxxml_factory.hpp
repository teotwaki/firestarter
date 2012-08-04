/**
 * @file mirror/utils/wxxml_factory.hpp
 * @brief WxXml-based factory generator plugin
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_WXXML_FACTORY_1011291729_HPP
#define MIRROR_UTILS_WXXML_FACTORY_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sdn_factory.hpp>
#include <wx/xml/xml.h>

MIRROR_NAMESPACE_BEGIN

class sdn_fact_wxxml_property;
class sdn_fact_wxxml_element;
class sdn_fact_wxxml_range;

class sdn_fact_wxxml_property
{
private:
	typedef sdn_fact_wxxml_property property;
	typedef wxXmlProperty xml_attrib;
	xml_attrib* attrib;

	friend class sdn_fact_wxxml_element;

	sdn_fact_wxxml_property(xml_attrib* _attrib)
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
		return std::string(attrib->GetValue().mb_str(wxConvUTF8));
	}
};

class sdn_fact_wxxml_range
{
private:
	typedef sdn_fact_wxxml_element element;
	typedef wxXmlNode xml_node;
	xml_node* node;
public:
	sdn_fact_wxxml_range(xml_node* _node)
	 : node(_node)
	{
	}

	bool empty(void) const
	{
		return node == nullptr;
	}

	void step_front(void)
	{
		assert(!empty());
		node = node->GetNext();
	}

	element front(void) const;
};

class sdn_fact_wxxml_element
{
private:
	typedef sdn_fact_wxxml_range range;
	typedef sdn_fact_wxxml_element element;
	typedef sdn_fact_wxxml_property property;
	typedef wxXmlNode xml_node;
	xml_node* node;
public:
	sdn_fact_wxxml_element(wxXmlDocument& doc)
	 : node(doc.GetRoot())
	{
	}

	sdn_fact_wxxml_element(xml_node* _node)
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
		return range(node->GetChildren());
	}

	element child(const std::string& name)
	{
		assert(node != nullptr);
		wxXmlNode* child_node = node->GetChildren();
		wxString cmp_name(name.c_str(), wxConvUTF8, name.size());
		while(child_node != nullptr && !name.empty())
		{
			if(child_node->GetName() == cmp_name)
				break;
			child_node = child_node->GetNext();
		}
		return element(child_node);
	}

	property attribute(void)
	{
		assert(node != nullptr);
		return property(node->GetProperties());
	}

	property attribute(const std::string& name)
	{
		assert(!name.empty());
		assert(node != nullptr);
		wxXmlProperty* child_attr = node->GetProperties();
		wxString cmp_name(name.c_str(), wxConvUTF8, name.size());
		while(child_attr != nullptr)
		{
			if(child_attr->GetName() == cmp_name)
				break;
			child_attr = child_attr->GetNext();
		}
		return property(child_attr);
	}
};

inline sdn_fact_wxxml_element sdn_fact_wxxml_range::front(void) const
{
	assert(!empty());
	return sdn_fact_wxxml_element(node);
}

typedef sdn_fact_traits<
	sdn_fact_wxxml_range,
	sdn_fact_wxxml_element,
	sdn_fact_wxxml_property
> sdn_fact_wxxml_traits;

typedef factory_maker<
	sdn_fact_manuf,
	sdn_fact_suppl,
	sdn_fact_enum,
	sdn_fact_wxxml_traits
> wxxml_factory_maker;

typedef sdn_fact_data<
	sdn_fact_wxxml_traits
> wxxml_factory_input;

MIRROR_NAMESPACE_END

#endif //include guard

