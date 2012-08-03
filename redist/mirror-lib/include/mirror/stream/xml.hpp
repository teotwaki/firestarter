/**
 * @file mirror/stream/xml.hpp
 * @brief XML iostream writer implementation
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_STREAM_XML_1011291729_HPP
#define MIRROR_STREAM_XML_1011291729_HPP

#include <mirror/stream/base.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace stream {

/// Wrapper class allowing to write objects in XML format to std ostreams
/**
 *  @see to_json
 *  @see to_meta_xml
 */
struct to_xml : _target<to_xml> { };

template <typename T>
class _writer<T, to_xml>
{
private:
	static void do_indent(std::ostream& out, int depth)
	{
		out << std::endl;
		while(depth--) out << "    ";
	}

	struct write_x
	{
		std::ostream& out;
		const _wrap<T, to_xml>& obj;

		inline write_x(
			std::ostream& os,
			const _wrap<T, to_xml>& ob
		): out(os)
		 , obj(ob)
		{ }

		void indent(int ofs = 0) const
		{
			do_indent(out, obj.depth()+ofs);
		}

		static bool is_special_char(char x)
		{
			// TODO
			return (x == '<') ||
				(x == '>') ||
				(x == '\'') ||
				(x == '"') ||
				(x == '&');
		}

		void wr_translated(char x)
		{
			out << '&';
			switch(x)
			{
				// TODO
				case '<' : out << "lt";
				case '>' : out << "gt";
				case '\'': out << "apos";
				case '"' : out << "quot";
				case '&' : out << "amp";
				default:;
			}
			out << ';';
		}

		void wr_val(bool x)
		{
			out << x ? "true" : "false";
		}

		void wr_val(char x)
		{
			if(is_special_char(x))
				wr_translated(x);
			else out << x;
		}

		void wr_val(unsigned char x)
		{
			out << int(x);
		}

		void wr_val(const std::string& x)
		{
			std::string::const_iterator i = x.begin(), e = x.end();
			while(i != e) wr_val(*i++);
		}

		template <typename X>
		void wr_val(
			const X& x,
			typename std::enable_if<
				std::is_arithmetic<X>::value
			>::type* = nullptr
		)
		{
			out << x;
		}

		template <typename X>
		void wr_val(
			const X& x,
			typename std::enable_if<
				!std::is_arithmetic<X>::value &&
				!std::is_enum<X>::value
			>::type* = nullptr
		)
		{
			out << to_xml::from(x, obj);
		}

		template <typename X>
		void wr_val(
			X x,
			typename std::enable_if<
				std::is_enum<X>::value
			>::type* = nullptr
		)
		{
			out << MIRRORED_ENUM(X)::name_by_value(x);
		}
	};

	// helper functor executed on all members
	struct write_mem : write_x
	{
		inline write_mem(
			std::ostream& os,
			const _wrap<T, to_xml>& ob
		): write_x(os, ob)
		{ }

		template <typename IterInfo>
		void operator()(IterInfo)
		{
			typedef typename IterInfo::type meta_member;
			// write the separator if necessary
			// write the member name and the separating ':'
			this->indent(1);
			this->out
				<< '<'
				<< meta_member::base_name()
				<< ">";
			// write the value
			this->wr_val(meta_member::get(this->obj.value));
			this->out
				<< '<'
				<< meta_member::base_name()
				<< "/>";
			if(IterInfo::is_last::value) this->indent();
		}
	};

	// write a structure
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_xml>& obj,
		non_container_tag&
	)
	{
		static_assert(
			mp::empty<all_containers<MetaT> >::value,
			"Non-container types should not have containers"
		);
		mp::for_each_ii<members<MetaT> >(write_mem(out, obj));
	}

	// helper functor executed on all elements
	struct write_elem : write_x
	{
		std::string name;

		inline write_elem(
			std::ostream& os,
			const _wrap<T, to_xml>& ob,
			const std::string& n
		): write_x(os, ob)
		 , name(n)
		{ }

		template <typename Locator>
		void operator()(Locator loc, bool first)
		{
			this->indent(1);
			this->out << '<' << name << '>';
			// write the value
			wr_val(loc.get());
			this->out << '<' << name << "/>";
		}
	};

	// write a simple container
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_xml>& obj,
		simple_container_tag&
	)
	{
		typedef typename mp::front<all_containers<MetaT> >::type
			meta_cntnr;
		typedef typename default_traversal<
			meta_cntnr
		>::type meta_trav;
		//
		write_elem do_write(
			out,
			obj,
			meta_cntnr::element_type::base_name()
		);
		auto trav=meta_trav::start(obj.value);
		if(!trav.done())
		{
			do_write(trav.front(), true);
			trav.step_front();
			while(!trav.done())
			{
				do_write(trav.front(), false);
				trav.step_front();
			}
			do_write.indent();
		}
	}

	// write an optional/container
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_xml>& obj,
		optional_container_tag&
	)
	{
		typedef typename mp::front<all_containers<MetaT> >::type
			meta_cnt;
		if(!meta_cnt::empty(obj.value))
		{
			auto tr = meta_cnt::default_traversal::start(obj.value);
			assert(!tr.done());
			write_x do_write(out, obj);
			do_write.wr_val(tr.front().get());
		}
	}

	struct write_var : write_x
	{
		inline write_var(
			std::ostream& os,
			const _wrap<T, to_xml>& ob
		): write_x(os, ob)
		{ }

		template <typename MetaCntnr>
		void operator()(MetaCntnr)
		{
			if(!MetaCntnr::empty(this->obj.value))
			{
				wr_val(
					MetaCntnr::
					default_traversal::
					start(this->obj.value).
					front().get()
				);
			}
		}
	};

	// write an variant/container
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_xml>& obj,
		variant_container_tag&
	)
	{
		write_var do_write(out, obj);
		mp::for_each<all_containers<MetaT> >(do_write);
	}

	// other containers are not supported
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_xml>& obj,
		container_tag&
	);
public:
	std::ostream& operator()(
		std::ostream& out,
		const _wrap<T, to_xml>& obj
	)
	{
		typedef MIRRORED_CLASS(T) meta_T;
		typename container_kind<meta_T>::type kind;
		write(meta_T(), out, obj, kind);
		return out;
	}

	std::ostream& operator()(
		std::ostream& out,
		const _wrap<T, to_xml>& obj,
		const name_proc& namer
	)
	{
		typedef MIRRORED_CLASS(T) meta_T;
		out << '<'; namer(out); out << '>';
		typename container_kind<meta_T>::type kind;
		write(meta_T(), out, obj, kind);
		out << '<'; namer(out); out << "/>";
		return out;
	}
};

} // namespace stream
MIRROR_NAMESPACE_END

#endif //include guard

