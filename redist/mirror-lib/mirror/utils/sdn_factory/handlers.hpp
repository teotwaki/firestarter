/**
 * .file mirror/utils/sdn_factory/handlers.hpp
 * .brief Structured data notation handler interface and basic implementation
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_FACTORY_HANDLERS_1011291729_HPP
#define MIRROR_UTILS_SDN_FACTORY_HANDLERS_1011291729_HPP

#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <map>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class sdn_fact_handler_node;

template <class Traits>
class sdn_fact_struct_handler;

// Interface for SDN factory helper handlers
template <class Traits>
struct sdn_fact_handler
{
	sdn_fact_handler(void) = default;
	sdn_fact_handler(const sdn_fact_handler&) = delete;

	virtual ~sdn_fact_handler(void) { }

	typedef typename Traits::element element;

	virtual double match(element elem) = 0;

	virtual std::string expected(void) const = 0;

	virtual void add_subhandler(sdn_fact_handler* subhandler)
	{
		assert(!"This handler cannot have subhandlers!");
	}

	virtual void skip_subhandler(void)
	{
		assert(!"This handler cannot skip subhandlers!");
	}

protected:
	friend class mirror::sdn_fact_data<Traits>;
	friend class sdn_fact_handler_node<Traits>;
	friend class sdn_fact_struct_handler<Traits>;

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

template <class Traits>
struct sdn_fact_dummy_handler
 : public sdn_fact_handler<Traits>
{
private:
	typedef typename Traits::element element;
public:
	virtual double match(element elem)
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
class sdn_fact_handler_node
 : public sdn_fact_handler<Traits>
{
private:
	typedef sdn_fact_handler<Traits> base_intf;
protected:
	// list of pointers to sub-handlers
	std::vector<base_intf*> subhandlers;
public:
	void add_subhandler(base_intf* handler)
	{
		subhandlers.push_back(handler);
	}

	void skip_subhandler(void)
	{
		static sdn_fact_dummy_handler<Traits> shared_dummy;
		subhandlers.push_back(&shared_dummy);
	}

	virtual std::string expected(void) const
	{
		std::string result("{");
		auto i = subhandlers.begin(), e = subhandlers.end();
		while(i != e)
		{
			result.append((*i)->expected());
			++i;
			if(i != e) result.append(", ");
		}
		result.append("}");
		return result;
	}

protected:
	friend class mirror::sdn_fact_data<Traits>;

	virtual void print(
		std::vector<bool>& conns,
		std::ostream& out,
		bool last
	)
	{
		this->print_conns(conns, out);
		auto i = subhandlers.begin(), e = subhandlers.end();
		out <<	((i != e) ? "+-+-<" : "+---[" ) <<
			this->expected() <<
			((i != e) ? ">" : "]") <<
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
class sdn_fact_param
{
private:
	sdn_fact_handler<Traits>& parent_handler;
	typename Traits::shared_data& shared_ref;
protected:
	friend class mirror::sdn_fact_data<Traits>;

	static inline sdn_fact_handler<Traits>& deref(
		sdn_fact_handler<Traits>* handler_ptr
	)
	{
		assert(handler_ptr != nullptr);
		return *handler_ptr;
	}

	sdn_fact_param(
		sdn_fact_handler<Traits>& handler,
		typename Traits::shared_data& shared
	): parent_handler(handler)
	 , shared_ref(shared)
	{ }
public:
	sdn_fact_param(
		sdn_fact_handler<Traits>& handler,
		const sdn_fact_param<Traits>& param
	): parent_handler(handler)
	 , shared_ref(param.shared_ref)
	{ }

	sdn_fact_param(
		sdn_fact_handler<Traits>* handler_ptr,
		const sdn_fact_param<Traits>& param
	): parent_handler(deref(handler_ptr))
	 , shared_ref(param.shared_ref)
	{ }

	sdn_fact_handler<Traits>& handler(void) const
	{
		return parent_handler;
	}

	typename Traits::shared_data& shared_data(void) const
	{
		return shared_ref;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

