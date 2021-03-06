/**
 * @file mirror/stream/simple_struct.hpp
 * @brief A structured object representation iostream writer implementation
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_STREAM_STRUCTURED_1102141318_HPP
#define MIRROR_STREAM_STRUCTURED_1102141318_HPP

#include <mirror/stream/base.hpp>
#include <mirror/cntnr_kind_tags.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace stream {

/// Wrapper class allowing to write objects in a structured format to ostreams
/**
 *  @see to_xml
 *  @see to_json
 */
struct to_simple_struct : _target<to_simple_struct> { };

template <typename T>
class _writer<T, to_simple_struct>
{
private:
	// helper functor executed on all members
	struct write_x
	{
		std::ostream& out;
		const _wrap<T, to_simple_struct>& obj;

		inline write_x(
			std::ostream& os,
			const _wrap<T, to_simple_struct>& ob
		): out(os)
		 , obj(ob)
		{ }

		static bool is_escaped_char(char c)
		{
			return	(c == '"') ||
				(c == '\\') ||
				(c == '\b') ||
				(c == '\f') ||
				(c == '\n') ||
				(c == '\r') ||
				(c == '\t');
		}

		static char translate_esc_char(char c)
		{
			switch(c)
			{
				case '\b': return 'b';
				case '\f': return 'f';
				case '\n': return 'n';
				case '\r': return 'r';
				case '\t': return 't';
				default:;
			}
			return c;
		}

		void wr_val(bool x)
		{
			out << (x ? "true" : "false");
		}

		void do_wr_val(char x)
		{
			if(is_escaped_char(x))
				out << '\\' << translate_esc_char(x);
			else out << x;
		}

		void wr_val(char x)
		{
			out << '"';
			do_wr_val(x);
			out << '"';
		}

		void wr_val(unsigned char x)
		{
			out << int(x);
		}

		void wr_val(const std::string& x)
		{
			out << '"';
			std::string::const_iterator i = x.begin(), e = x.end();
			while(i != e) do_wr_val(*i++);
			out << '"';
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
			out << to_simple_struct::from(x, obj);
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

	struct write_mem : write_x
	{
		inline write_mem(
			std::ostream& os,
			const _wrap<T, to_simple_struct>& ob
		): write_x(os, ob)
		{ }

		template <typename IterInfo>
		void operator()(IterInfo)
		{
			typedef typename IterInfo::type meta_member;
			// write the separator if necessary
			if(!IterInfo::is_first::value) this->out << ", ";
			// write the value
			wr_val(meta_member::get(this->obj.value));
		}
	};

	// write a structure
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_simple_struct>& obj,
		non_container_tag&
	)
	{
		static_assert(
			mp::empty<all_containers<MetaT> >::value,
			"Non-container types should not have containers"
		);
		out << "{";
		mp::for_each_ii<all_member_variables<MetaT> >(write_mem(out, obj));
		out << "}";
	}

	// helper functor executed on all elements
	struct write_elem : write_x
	{
		inline write_elem(
			std::ostream& os,
			const _wrap<T, to_simple_struct>& ob
		): write_x(os, ob)
		{ }

		template <typename Locator>
		void operator()(Locator loc, bool first = true)
		{
			if(loc)
			{
				// write the separator if necessary
				if(!first) this->out << ", ";
				// write the value
				this->wr_val(loc.get());
			}
		}
	};

	// write a simple container
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_simple_struct>& obj,
		simple_container_tag&
	)
	{
		out << "[";
		typedef typename default_traversal<
			mp::front<all_containers<MetaT> >
		>::type meta_trav;
		//
		write_elem do_write(out, obj);
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
		}
		out << "]";
	}

	// write an optional/container
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_simple_struct>& obj,
		optional_container_tag&
	)
	{
		typedef typename mp::front<all_containers<MetaT> >::type
			meta_cnt;
		if(meta_cnt::empty(obj.value))
		{
			out << "nil";
		}
		else
		{
			auto tr = meta_cnt::default_traversal::start(obj.value);
			assert(!tr.done());
			write_elem do_write(out, obj);
			do_write(tr.front());
		}
	}

	struct write_var
	{
		write_elem do_write;

		inline write_var(
			std::ostream& os,
			const _wrap<T, to_simple_struct>& ob
		): do_write(os, ob)
		{ }

		template <typename MetaCntnr>
		void operator()(MetaCntnr)
		{
			// write the value
			if(!MetaCntnr::empty(do_write.obj.value))
			{
				do_write(
					MetaCntnr::
					default_traversal::
					start(do_write.obj.value).
					front()
				);
			}
		}
	};

	// write an variant/container
	template <typename MetaT>
	void write(
		MetaT,
		std::ostream& out,
		const _wrap<T, to_simple_struct>& obj,
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
		const _wrap<T, to_simple_struct>& obj,
		container_tag&
	);

public:
	std::ostream& operator()(
		std::ostream& out,
		const _wrap<T, to_simple_struct>& obj
	)
	{
		typedef MIRRORED_CLASS(T) meta_T;
		typename container_kind<meta_T>::type kind;
		write(meta_T(), out, obj, kind);
		return out;
	}

	std::ostream& operator()(
		std::ostream& out,
		const _wrap<T, to_simple_struct>& obj,
		const name_proc& namer
	)
	{
		typedef MIRRORED_CLASS(T) meta_T;
		typename container_kind<meta_T>::type kind;
		out << '"'; namer(out); out << '"' << " = ";
		write(meta_T(), out, obj, kind);
		return out;
	}
};

} // namespace stream
MIRROR_NAMESPACE_END

#endif //include guard

