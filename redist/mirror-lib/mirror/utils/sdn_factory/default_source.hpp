/**
 * .file mirror/utils/sdn_factory/default_source.hpp
 * .brief Default manufacturer for the structured data factory plugin framework
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_SDN_DEFAULT_SOURCE_1011291729_HPP
#define MIRROR_UTILS_SDN_DEFAULT_SOURCE_1011291729_HPP

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class sdn_fact_struct_handler
 : public sdn_fact_handler<Traits>
{
private:
	typedef sdn_fact_handler<Traits> base_intf;
	typedef sdn_fact_param<Traits> param_type;
	typedef typename Traits::element element;

	base_intf* subhandler;
	typename Traits::shared_data& shared_data;
	typename Traits::shared_string_proxy type_name;
	typename Traits::shared_string_proxy object_name;

	bool is_toplevel(void) const
	{
		return Traits::get_shared_string(
			shared_data,
			object_name
		).empty();
	}


	double match_toplevel(element elem)
	{
		return subhandler->match(elem);
	}

	double match_param(element elem)
	{
		try
		{
			element child = elem.child(
				Traits::translate(
					shared_data,
					Traits::get_shared_string(
						shared_data,
						object_name
					)
				)
			);
			if(child)
			{
				if(child.has_type(
					Traits::translate(
						shared_data,
						Traits::get_shared_string(
							shared_data,
							type_name
						)
					)
				)) return subhandler->match(child);
			}
		}
		catch(sdn_fact_node_not_found_error&)
		{
		}
		return 0.0;
	}
public:
	sdn_fact_struct_handler(
		const param_type& parent_data,
		const std::string& type,
		const std::string& object
	): subhandler(nullptr)
	 , shared_data(parent_data.shared_data())
	 , type_name(Traits::share_string(shared_data, type))
	 , object_name(Traits::share_string(shared_data, object))
	{ }

	sdn_fact_struct_handler(
		const param_type& parent_data,
		const std::string& type
	): subhandler(nullptr)
	 , shared_data(parent_data.shared_data())
	 , type_name(Traits::share_string(shared_data, type))
	 , object_name(Traits::share_string(shared_data, std::string()))
	{ }

	void add_subhandler(base_intf* handler)
	{
		assert(handler != nullptr);
		assert(subhandler == nullptr);
		subhandler = handler;
	}

	double match(element elem)
	{
		assert(subhandler != nullptr);
		return is_toplevel() ? match_toplevel(elem) : match_param(elem);
	}

	std::string expected(void) const
	{
		std::string temp;
		if(!is_toplevel())
		{
			temp.append(
				Traits::translate(
					shared_data,
					Traits::get_shared_string(
						shared_data,
						object_name
					)
				) + ": "
			);
		}
		return temp + Traits::translate(
			shared_data,
			Traits::get_shared_string(shared_data, type_name)
		) ;
	}
protected:
	void print(
		std::vector<bool>& conns,
		std::ostream& out,
		bool last
	)
	{
		assert(subhandler != nullptr);
		this->print_conns(conns, out);
		out <<	"+-+-<" << this->expected() << ">" <<
			"-(" << this << ")" <<
			std::endl;
		conns.push_back(!last);
		this->print_conns(conns, out);
		out << "|" << std::endl;
		subhandler->print(conns, out, true);
		conns.pop_back();
	}
};

template <class Product, class Traits>
class sdn_fact_source<
	Product,
	Traits,
	std::false_type
>
{
private:
	typedef sdn_fact_param<Traits> param_type;

	typedef sdn_fact_struct_handler<Traits> struct_handler;
	std::shared_ptr<struct_handler> handler;

	typedef typename mirror::factory_maker<
		sdn_fact_manuf,
		sdn_fact_suppl,
		sdn_fact_enum,
		Traits
	> maker;
	typename maker::template factory< Product >::type fact;
public:
	template <class ConstructionInfo>
	inline sdn_fact_source(
		const param_type& parent_data,
		ConstructionInfo construction_info
	): handler(
		new struct_handler(
			parent_data,
			ConstructionInfo::parameter::type::local_name(),
			ConstructionInfo::parameter::base_name()
		)
	), fact(param_type(handler.get(), parent_data), construction_info)
	{ }

	void finish(const param_type& parent_data)
	{
		parent_data.handler().add_subhandler(handler.get());
	}

	inline Product operator()(void)
	{
		return fact();
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

