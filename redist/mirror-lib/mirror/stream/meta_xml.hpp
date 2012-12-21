/**
 * @file mirror/stream/meta_xml.hpp
 * @brief XML iostream writer implementation
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_STREAM_META_XML_1011291729_HPP
#define MIRROR_STREAM_META_XML_1011291729_HPP

#include <mirror/stream/base.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace stream {

/// Wrapper class allowing to write objects in XML format to std ostreams
/**
 *  @see to_json
 *  @see to_xml
 */
struct to_meta_xml : _target<to_meta_xml> { };

template <typename T>
class _writer<T, to_meta_xml>
{
private:
	static std::ostream& indent(std::ostream& out, int depth)
	{
		while(depth--) out << "    ";
		return out;
	}

	// helper functor executed on all members
	struct write_mem
	{
		std::ostream& out;
		const _wrap<T, to_meta_xml>& obj;

		inline write_mem(
			std::ostream& os,
			const _wrap<T, to_meta_xml>& ob
		): out(os)
		 , obj(ob)
		{ }

		void wr_val(bool x)
		{
			out << (x ? "true" : "false");
		}

		void wr_val(const std::string& x)
		{
			// TODO: xml string escaping
			out << x;
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
			out << std::endl << to_meta_xml::from(x, obj);
			indent(out, obj.depth()+1);
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

		template <typename IterInfo>
		void operator()(IterInfo)
		{
			typedef typename IterInfo::type meta_member;
			// write the separator if necessary
			// write the member name and the separating ':'
			indent(out, obj.depth()+1)
				<< "<member name='"
				<< meta_member::base_name()
				<< "' type='"
				<< meta_member::type::full_name()
				<< "'>";
			// write the value
			wr_val(meta_member::get(obj.value));
			out	<< "<member/>"
				<< std::endl;
		}
	};
public:
	std::ostream& operator()(
		std::ostream& out,
		const _wrap<T, to_meta_xml>& obj
	)
	{
		typedef MIRRORED_CLASS(T) meta_T;
		mp::for_each_ii<members<meta_T> >(write_mem(out, obj));
		return out;
	}

	std::ostream& operator()(
		std::ostream& out,
		const _wrap<T, to_meta_xml>& obj,
		const name_proc& namer
	)
	{
		typedef MIRRORED_CLASS(T) meta_T;
		out	<< "<object name='";
		namer(out);
		out	<< "' type='"
			<< meta_T::full_name()
			<< "'>"
			<< std::endl;
		mp::for_each_ii<members<meta_T> >(write_mem(out, obj));
		out << "<object/>";
		return out;
	}
};

} // namespace stream
MIRROR_NAMESPACE_END

#endif //include guard

