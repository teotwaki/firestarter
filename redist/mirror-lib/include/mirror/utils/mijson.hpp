/**
 * @file mirror/utils/mijson.hpp
 * @brief Simple in-place non-modifying JSON parser
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_MIJSON_1011291729_HPP
#define MIRROR_UTILS_MIJSON_1011291729_HPP

#include <mirror/config.hpp>
#include <mirror/utils/string/unescape.hpp>

#include <utility>
#include <cctype>
#include <list>
#include <map>


MIRROR_NAMESPACE_BEGIN

template <typename Iterator>
class mijson_fragment;

template <typename Iterator>
class mijson_element
{
public:
	enum class element_type
	{
		json_unknown,
		json_string,
		json_number,
		json_object,
		json_array,
		json_keyword
	};

	static const char* element_type_name(element_type val)
	{
		switch(val)
		{
			case element_type::json_string: return "string";
			case element_type::json_number: return "number";
			case element_type::json_object: return "object";
			case element_type::json_array:  return "array";
			case element_type::json_keyword:return "keyword";
			default:;
		}
		return "unknown";
	}

	typedef Iterator iterator;
	typedef std::list<mijson_element> element_list;
private:
	element_type elem_type;

	Iterator elem_name_begin;
	Iterator elem_name_end;

	Iterator elem_value_begin;
	Iterator elem_value_end;

	element_list elem_elements;
	element_list elem_members;

	std::map<
		std::string,
		typename element_list::const_iterator
	> elem_member_map;

	void init_elems(element_list& elems)
	{
		if(elem_type == element_type::json_array)
			std::swap(elem_elements, elems);
		else if(elem_type == element_type::json_object)
			std::swap(elem_members, elems);
		typename element_list::const_iterator
			i = elem_members.begin(),
			e = elem_members.end();
		while(i != e)
		{
			elem_member_map[i->name()] = i;
			++i;
		}
	}
protected:
	friend class mijson_fragment<Iterator>;

	mijson_element(const mijson_element&) = delete;

	mijson_element(
		element_type _type,
		Iterator _value_begin,
		Iterator _value_end,
		element_list& elems
	): elem_type(_type)
	 , elem_name_begin(Iterator())
	 , elem_name_end(Iterator())
	 , elem_value_begin(_value_begin)
	 , elem_value_end(_value_end)
	{
		init_elems(elems);
	}

	mijson_element(
		element_type _type,
		Iterator _name_begin,
		Iterator _name_end,
		Iterator _value_begin,
		Iterator _value_end,
		element_list& elems
	): elem_type(_type)
	 , elem_name_begin(_name_begin)
	 , elem_name_end(_name_end)
	 , elem_value_begin(_value_begin)
	 , elem_value_end(_value_end)
	{
		init_elems(elems);
	}

public:
	mijson_element(mijson_element&& src)
	 : elem_type(src.elem_type)
	 , elem_name_begin(std::move(src.elem_name_begin))
	 , elem_name_end(std::move(src.elem_name_end))
	 , elem_value_begin(std::move(src.elem_value_begin))
	 , elem_value_end(std::move(src.elem_value_end))
	 , elem_elements(std::move(src.elem_elements))
	 , elem_members(std::move(src.elem_members))
	 , elem_member_map(std::move(src.elem_member_map))
	{ }

	element_type type(void) const
	{
		return elem_type;
	}

	const char* type_name(void) const
	{
		return element_type_name(type());
	}

	bool is_leaf(void) const
	{
		return	(elem_type == element_type::json_string) ||
			(elem_type == element_type::json_number) ||
			(elem_type == element_type::json_keyword);
	}

	bool is_node(void) const
	{
		return	(elem_type == element_type::json_object) ||
			(elem_type == element_type::json_array);
	}

	bool has_name(void) const
	{
		return elem_name_begin != elem_name_end;
	}

	Iterator name_begin(void) const
	{
		return elem_name_begin;
	}

	Iterator name_end(void) const
	{
		return elem_name_end;
	}

	std::string name(void) const
	{
		return aux::unescape(name_begin(), name_end());
	}

	bool has_value(void) const
	{
		return elem_value_begin != elem_value_end;
	}

	Iterator value_begin(void) const
	{
		return elem_value_begin;
	}

	Iterator value_end(void) const
	{
		return elem_value_end;
	}

	std::string value(void) const
	{
		if(elem_type == element_type::json_string)
			return aux::unescape(value_begin(), value_end());
		else return std::string(value_begin(), value_end());
	}

	const element_list& elements(void) const
	{
		return elem_elements;
	}

	const element_list& members(void) const
	{
		return elem_members;
	}

	const mijson_element& member(const std::string& name) const
	{
		auto p = elem_member_map.find(name);
		if(p == elem_member_map.end())
			throw std::runtime_error("Element "+name+" not found");
		return *(p->second);
	}
};

template <typename Iterator>
class mijson_fragment
 : public mijson_element<Iterator>
{
private:
	typedef mijson_element<Iterator> element;
	typedef typename element::element_list element_list;

	static bool is_quasi_space(char c)
	{
		return std::isspace(c) ||
			(c == '"') ||
			(c == '\'');
	}

	static bool is_trail_sep(char c)
	{
		return std::isspace(c) ||
			(c == ',') ||
			(c == '}') ||
			(c == ']');
	}

	static void skip_space(Iterator& cur, Iterator end)
	{
		while(cur != end)
			if(std::isspace(*cur)) ++cur;
			else break;
	}

	static void skip_trail_space(Iterator& cur, Iterator end)
	{
		if(cur != end)
		{
			if(is_quasi_space(*cur)) ++cur;
			skip_space(cur, end);
		}
	}

	static bool parse_string(Iterator& begin, Iterator& end)
	{
		if(begin == end) return false;
		// current char iterator
		Iterator cur = begin;
		// the possible quotation mark
		char quot = *cur;
		// if it is not a quotmark nor an apostrophe quit
		if(quot != '\'' && quot != '"') return false;
		else ++cur;
		// remember the begin of the potential literal
		Iterator lit_begin = cur;
		bool escape = false;
		// go through the input
		while(cur != end)
		{
			// if this character was not escaped and it is
			// the quotation mark that started the string
			if(*cur == quot && !escape)
			{
				// return the iterators marking the literal
				begin = lit_begin;
				end = cur;
				return true;
			}
			// check if the next character is escaped
			escape = (*cur == '\\' && !escape);
			++cur;
		}
		return false;
	}

	static bool parse_number(Iterator& begin, Iterator& end)
	{
		if(begin == end) return false;
		// current char iterator
		Iterator cur = begin;
		// skip the optional minus sign
		if(*cur == '-') ++cur;
		if(cur != end)
		{
			if(!std::isdigit(*cur))
				return false;
		}
		else return false;
		// the first sequence of digits
		while(cur != end)
		{
			if(std::isdigit(*cur)) ++cur;
			else if(*cur == '.') break;
			else if(*cur == 'e') break;
			else if(*cur == 'E') break;
			else if(is_trail_sep(*cur))
			{
				end = cur;
				return true;
			}
			else return false;
		}
		if(cur == end) return true;
		else if(*cur == '.')
		{
			++cur;
			while(cur != end)
			{
				if(std::isdigit(*cur)) ++cur;
				else if(*cur == 'e') break;
				else if(*cur == 'E') break;
				else if(is_trail_sep(*cur))
				{
					end = cur;
					return true;
				}
				else return false;
			}
		}
		if(cur == end) return true;
		else if(*cur == 'e' || *cur == 'E')
		{
			++cur;
			if(cur == end) return false;
			else if(*cur == '+' || *cur == '-') ++cur;
			if(cur == end) return false;
			else while(cur != end)
			{
				if(std::isdigit(*cur)) ++cur;
				else if(is_trail_sep(*cur)) break;
				else return false;
			}
		}
		end = cur;
		return true;
	}

	static bool parse_keyword(Iterator& begin, Iterator& end)
	{
		if(begin == end) return false;
		Iterator cur = begin;
		if(*cur == 'f' || *cur == 't' || *cur == 'n') ++cur;
		else return false;
		if(*cur == 'a' || *cur == 'r' || *cur == 'u') ++cur;
		else return false;
		if(*cur == 'l' || *cur == 'u' || *cur == 'l') ++cur;
		else return false;
		if(*cur == 's' || *cur == 'e' || *cur == 'l') ++cur;
		else return false;
		if(*cur == 'e') ++cur;
		if(is_trail_sep(*cur))
		{
			end = cur;
			return true;
		}
		return false;
	}

	static bool parse_range(
		Iterator& begin,
		Iterator& end,
		element_list& elements,
		char lt,
		char rt
	)
	{
		if(begin == end) return false;
		Iterator cur = begin;
		if(*cur != lt) return false;
		else ++cur;
		skip_space(cur, end);
		if(cur == end) return false;
		if(*cur != rt)
		{
			while(cur != end)
			{
				elements.push_back(std::move(parse(cur, end)));
				skip_trail_space(cur, end);
				if(*cur == rt)
				{
					++cur;
					break;
				}
				if(*cur != ',') return false;
				skip_trail_space(++cur, end);
			}
		}
		else ++cur;
		skip_trail_space(cur, end);
		end = cur;
		return true;
	}

	static element parse(Iterator& cur, Iterator end)
	{
		skip_space(cur, end);
		Iterator i = cur, e = end;
		bool parsed_string = parse_string(i, e);
		element_list elements;
		// if we parsed a string
		if(parsed_string)
		{
			cur = e;
			skip_trail_space(cur, end);
			// and that's all
			if((cur == end) || is_trail_sep(*cur))
			{
				// return the element
				return element(
					element::element_type::json_string,
					i, e, elements
				);
			}
			// if there is the separator, skip it
			else if(*cur == ':')
			{
				++cur;
				skip_space(cur, end);
			}
		}
		typename element::element_type type =
			element::element_type::json_unknown;
		Iterator
			j = parsed_string ? cur : i,
			f = parsed_string ? end : e;
		if(parse_string(j, f))
			type = element::element_type::json_string;
		else if(parse_number(j, f))
			type = element::element_type::json_number;
		else if(parse_keyword(j, f))
			type = element::element_type::json_keyword;
		else if(parse_range(j, f, elements, '[', ']'))
			type = element::element_type::json_array;
		else if(parse_range(j, f, elements, '{', '}'))
			type = element::element_type::json_object;
		else throw std::runtime_error("Parse error");
		cur = f;
		if(parsed_string) return element(type, i, e, j, f, elements);
		else return element(type, i, e, elements);
	}
public:
	mijson_fragment(
		Iterator input_begin,
		Iterator input_end
	): mijson_element<Iterator>(std::move(parse(input_begin, input_end)))
	{ }
};

MIRROR_NAMESPACE_END

#endif //include guard

