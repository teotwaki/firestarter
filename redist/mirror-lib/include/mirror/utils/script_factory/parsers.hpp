/**
 * .file mirror/utils/script_factory/parsers.hpp
 * .brief Script parser interface and basic implementations
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SCRIPT_FACTORY_PARSERS_1011291729_HPP
#define MIRROR_UTILS_SCRIPT_FACTORY_PARSERS_1011291729_HPP

#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <map>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class script_fact_parser_node;

template <class Traits>
class script_fact_proxy_parser;

template <class Traits>
class script_fact_sequence_parser;

// Interface for script factory helper parsers
template <class Traits>
struct script_fact_parser
{
	script_fact_parser(void) = default;
	script_fact_parser(const script_fact_parser&) = delete;

	virtual ~script_fact_parser(void) { }

	// The iterator type used to traverse the input string
	typedef typename Traits::iterator iterator;

	virtual double match(iterator& begin, iterator end) = 0;

	virtual std::string expected(void) const = 0;

	virtual void add_subparser(script_fact_parser* subparser)
	{
		assert(!"This parser cannot have subparsers!");
	}

	virtual void skip_subparser(void)
	{
		assert(!"This parser cannot skip subparsers!");
	}
protected:
	friend class mirror::script_fact_data<Traits>;
	friend class script_fact_parser_node<Traits>;
	friend class script_fact_proxy_parser<Traits>;
	friend class script_fact_sequence_parser<Traits>;

	virtual void print_conns(std::vector<bool>& conns, std::ostream& out)
	{
		auto i = conns.begin(), e = conns.end();
		while(i != e) out << ((*i++) ? "| " : "  ");
	}

	virtual void print(
		std::vector<bool>& conns,
		std::ostream& out,
		bool last
	)
	{
		this->print_conns(conns, out);
		out <<	"+---[" <<
			this->expected() <<
			"]" << "-(" << this << ")" <<
			std::endl;
	}
};

// Interface for script factory helper parsers
template <class Traits>
class script_fact_proxy_parser
 : public script_fact_parser<Traits>
{
private:
	// The iterator type used to traverse the input string
	typedef typename Traits::iterator iterator;
	typedef script_fact_parser<Traits> base_intf;
	base_intf* subparser;
public:
	script_fact_proxy_parser(void)
	 : subparser(nullptr)
	{ }

	void add_subparser(base_intf* parser)
	{
		assert(parser != nullptr);
		assert(subparser == nullptr);
		subparser = parser;
	}

	double match(iterator& begin, iterator end)
	{
		assert(subparser != nullptr);
		return subparser->match(begin, end);
	}

	std::string expected(void) const
	{
		assert(subparser != nullptr);
		return subparser->expected();
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

template <class Traits>
struct script_fact_dummy_parser
 : public script_fact_parser<Traits>
{
private:
	typedef typename Traits::iterator iterator;
public:
	virtual double match(iterator& begin, iterator end)
	{
		return 0.0;
	}

	virtual std::string expected(void) const
	{
		return std::string("-");
	}
protected:
	virtual void print(
		std::vector<bool>& conns,
		std::ostream& out,
		bool last
	)
	{
		this->print_conns(conns, out);
		out <<	"+---[-]-(" << this << ")" << std::endl;
	}
};

template <class Traits>
class script_fact_parser_node
 : public script_fact_parser<Traits>
{
private:
	typedef script_fact_parser<Traits> base_intf;
protected:
	// list of pointers to sub-parsers
	std::vector<base_intf*> subparsers;
public:
	void add_subparser(base_intf* parser)
	{
		subparsers.push_back(parser);
	}

	void skip_subparser(void)
	{
		static script_fact_dummy_parser<Traits> shared_dummy;
		subparsers.push_back(&shared_dummy);
	}
protected:
	friend class mirror::script_fact_data<Traits>;

	virtual void print(
		std::vector<bool>& conns,
		std::ostream& out,
		bool last
	)
	{
		this->print_conns(conns, out);
		auto i = subparsers.begin(), e = subparsers.end();
		out <<	((i != e) ? "+-+-{" : "+---[" ) <<
			this->expected() <<
			((i != e) ? "}" : "]") <<
			"-(" << this << ")" <<
			std::endl;
		conns.push_back(!last);
		while(i != e)
		{
			this->print_conns(conns, out);
			out << "|" << std::endl;
			(*i)->print(conns, out, (i + 1) == e);
			++i;
		}
		conns.pop_back();
	}
};

template <typename Traits>
class script_fact_param
{
private:
	script_fact_parser<Traits>& parent_parser;
	typename Traits::shared_data& shared_ref;
protected:
	friend class mirror::script_fact_data<Traits>;

	static inline script_fact_parser<Traits>& deref(
		script_fact_parser<Traits>* parser_ptr
	)
	{
		assert(parser_ptr != nullptr);
		return *parser_ptr;
	}

	script_fact_param(
		script_fact_parser<Traits>& parser,
		typename Traits::shared_data& shared
	): parent_parser(parser)
	 , shared_ref(shared)
	{ }
public:
	script_fact_param(
		script_fact_parser<Traits>& parser,
		const script_fact_param<Traits>& param
	): parent_parser(parser)
	 , shared_ref(param.shared_ref)
	{ }

	script_fact_param(
		script_fact_parser<Traits>* parser_ptr,
		const script_fact_param<Traits>& param
	): parent_parser(deref(parser_ptr))
	 , shared_ref(param.shared_ref)
	{ }

	script_fact_parser<Traits>& parser(void) const
	{
		return parent_parser;
	}

	typename Traits::shared_data& shared_data(void) const
	{
		return shared_ref;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

