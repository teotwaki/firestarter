/*
 * Copyright (C) 2012  Sebastian Lauwers <sebastian.lauwers@gmail.com>
 *
 * This file is part of Firestarter.
 *
 * Firestarter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firestarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRESTARTER_PERSISTENT_HPP
#define FIRESTARTER_PERSISTENT_HPP

#include "persistent/keywords.hpp"
#include "persistent/storage.hpp"
#include "persistent/lexer.hpp"

#include <mirror/mirror.hpp>
#include <puddle/puddle.hpp>

#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace firestarter {
	namespace common {
		namespace Persistent {

	static __thread std::vector<boost::shared_ptr<soci::indicator>> indicators;

	template <typename MetaClass>
	struct ClassTransf {
		struct type {
			template <typename Param>
			type(Param const &) { };
		};
	};

	template <class MetaMemberVariable>
	struct MemVarTransf {
		struct type : QueryLexer<MetaMemberVariable> {
			template <class ClassTranf>
			type(ClassTranf &, int) { };

		};
	};

	template <class PersistableClass>
	struct Column : public mirror::class_generator<
		PersistableClass, 
		ClassTransf, 
		MemVarTransf
	>::type {
		Column(void) : mirror::class_generator<
			PersistableClass,
			ClassTransf,
			MemVarTransf
		>::type(0)
		{ };
	};

	template <class Object>
	struct Persist {

		inline static void connect(std::string const & connection_string) {
			Storage::connect(connection_string);
		}

		// Create a structure that will contain ...
		struct column_list_cts :
			// ... a string from which we remove the first two characters ...
			mirror::cts::skip_front<
				// ... created by recursively concatenating 
				mirror::mp::fold<
					// ... the names of the member variables of class 'Object', ...
					mirror::mp::transform<
						mirror::mp::only_if<
							mirror::members<mirror::reflected<Object>>,
							mirror::mp::is_a<
								mirror::mp::arg<1>,
								mirror::meta_member_variable_tag
							>
						>,
						// ... prepended with ", ", ...
						mirror::cts::concat<
							mirror::cts::string<',', ' '>,
							mirror::static_name<
								mirror::mp::arg<1>
							>
						>
					>, // ... and an empty string
					mirror::cts::string<>,
					mirror::cts::concat<
						mirror::mp::arg<1>,
						mirror::mp::arg<2>
					>
				>,
				std::integral_constant<int, 2>
			>
		{ };

		struct populate {
			Object & obj;
			boost::shared_ptr<soci::statement> st;

			populate(Object & obj) : obj(obj) {
				this->st = Storage::getStatement();
				indicators.clear();
				// Allocate enough space ...
				indicators.reserve(
					// ... to store the count ...
					mirror::mp::size<
						// ... of any Object member ...
						mirror::mp::only_if<
							mirror::members<mirror::reflected<Object>>,
							// ... which is a member variable
							mirror::mp::is_a<
								mirror::mp::arg<1>,
								mirror::meta_member_variable_tag
							>
						>
					>::value
				);
			};

			template <class MetaVariable>
			inline void operator () (MetaVariable meta_var, bool first, bool last) {
				indicators.push_back(boost::shared_ptr<soci::indicator>(new soci::indicator));
				auto & var = *meta_var.address(this->obj);
				this->st.get()->exchange(soci::into(var, *(indicators.back().get())));
			};
		};

		struct grab_values {
			Object const & obj;
			boost::shared_ptr<soci::statement> st;
			std::stringstream & query;

			grab_values(Object const & obj, std::stringstream & query) : obj(obj), query(query) {
				this->st = Storage::getStatement();
			};

			template <class MetaVariable>
			inline void operator () (MetaVariable meta_var, bool first, bool last) {
				auto & var = *meta_var.address(this->obj);
				this->st.get()->exchange(
					soci::use(var, meta_var.base_name() + boost::lexical_cast<std::string>(counter))
				);

				if (first) query << "(";
				query << ":" << meta_var.base_name() << boost::lexical_cast<std::string>(counter++);
				if (not last) query << ", ";
				else query << ")";
			};
		};

		struct grab_names_and_values {
			Object const & obj;
			boost::shared_ptr<soci::statement> st;
			std::stringstream & query;

			grab_names_and_values(Object const & obj, std::stringstream & query) : obj(obj), query(query) {
				this->st = Storage::getStatement();
			};

			template <class MetaVariable>
			inline void operator () (MetaVariable meta_var, bool first, bool last) {
				auto & var = *meta_var.address(this->obj);
				this->st.get()->exchange(
					soci::use(var, meta_var.base_name() + boost::lexical_cast<std::string>(counter))
				);

				query << meta_var.base_name() << " = :" << 
					meta_var.base_name() << boost::lexical_cast<std::string>(counter++);
				if (not last) query << ", ";
			};
		};

		static void store(Object const & obj) {
			namespace pk = Persistent::keywords;

			std::stringstream query;
			grab_values grab_values_(obj, query);
			auto meta_obj = puddle::reflected_type<Object>();
			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();

			query <<
				// Create a c-style string ...
				mirror::cts::c_str<
					// ... from the concatatenation ...
					mirror::cts::concat<
						// ... of the insert statement, and ...
						pk::insert_into,
						// ... the class name of Object, and ...
						mirror::static_name<mirror::reflected<Object>>,
						// ... and an opening parenthesis, and ...
						mirror::cts::string<' ', '('>,
						// ... the list of columns (member variables), and ...
						column_list_cts,
						// ... a closing parenthesis, and ...
						mirror::cts::string<')', ' '>,
						// ... the values satement
						pk::values
					>
				>();
			meta_obj.member_variables().for_each(grab_values_);
			query << ";";

			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			Storage::resetStatement();
		};

		static unsigned int count(PartialQuery const & partial_query = PartialQuery(std::string())) {
			namespace pk = Persistent::keywords;

			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();
			unsigned int count;

			st.exchange(soci::into(count));
			st.alloc();

			std::stringstream query;
			query <<
				// Create a c-style string ...
				mirror::cts::c_str<
					// ... from the concatenation ...
					mirror::cts::concat<
						// ... of the select statement, and ...
						pk::select,
						// ... the count statement, and ...
						pk::count,
						// ... the from statement, and ...
						pk::from,
						// ... the class name of Object, and ...
						mirror::static_name<mirror::reflected<Object>>
					>
				>();

			if (not partial_query.content.empty())
				query << mirror::cts::c_str<pk::where>() << partial_query;


			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			Storage::resetStatement();

			return count;
		};

		static void find(Object & obj, PartialQuery const & partial_query = PartialQuery(std::string())) {
			namespace pk = Persistent::keywords;

			populate p(obj);
			auto meta_obj = puddle::reflected_type<Object>();
			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();

			meta_obj.member_variables().for_each(p);
			st.alloc();

			std::stringstream query;
			query << 
				// Create a c-style string ...
				mirror::cts::c_str<
					// ... from the concatenation ...
					mirror::cts::concat<
						// ... of the select statement, and ...
						pk::select,
						// ... the list of columns (member variables), and ...
						column_list_cts,
						// ... the from statement, and ...
						pk::from,
						// ... the class name of Object, and ...
						mirror::static_name<mirror::reflected<Object>>
					>
				>();

			if (not partial_query.content.empty())
				query << mirror::cts::c_str<pk::where>() << partial_query;

			query << " LIMIT 1;";

			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			Storage::resetStatement();
		};

		static void find(std::vector<Object> & objects, 
				PartialQuery const & partial_query = PartialQuery(std::string()), unsigned int from = 0)
		{
			namespace pk = Persistent::keywords;

			Object obj;
			populate p(obj);
			auto meta_obj = puddle::reflected_type<Object>();
			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();
			unsigned int const limit = objects.capacity();

			meta_obj.member_variables().for_each(p);
			st.alloc();

			std::stringstream query;
			query << 
				// Create a c-style string ...
				mirror::cts::c_str<
					// ... from the concatenation ...
					mirror::cts::concat<
						// ... of the select statement, and ...
						pk::select,
						// ... the list of columns (member variables), and ...
						column_list_cts,
						// ... the from statement, and ...
						pk::from,
						// ... the class name of Object, and ...
						mirror::static_name<mirror::reflected<Object>>
					>
				>();

			if (not partial_query.content.empty())
				query << mirror::cts::c_str<pk::where>() << partial_query;

			if (from != 0 && limit != 0)
				query << " LIMIT " << from << ", " << limit << ";";

			else if (limit != 0)
				query << " LIMIT " << limit << ";";

			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			objects.push_back(obj);

			while (st.fetch())
				objects.push_back(obj);

			Storage::resetStatement();
		};

		template <typename BaseType, typename MetaVariable>
		struct convertType {
			std::string value;

			convertType (BaseType const &) {
				// Store the static representation of the MetaVariable's type (unsigned int, float, etc)
				this->value = mirror::cts::c_str<
					mirror::static_name<
						typename MetaVariable::type
					>
				>();
				// And transform it to be uppercase.
				boost::to_upper(this->value);
			};
		};

		// Template specialisation when the member variable is a std::string
		template <typename MetaVariable>
		struct convertType<std::string, MetaVariable> {
			std::string value;

			convertType(std::string const & value) {
				// If the string's capacity is less than 256, we can store it in a varchar
				if (value.capacity() < 256)
					this->value = "VARCHAR(" + boost::lexical_cast<std::string>(value.capacity()) + ")";
				// If it is over 255 characters, we have to store it in a TEXT blob.
				/// \todo Detect whether a LONGTEXT is required.
				else
					this->value = "TEXT";
			};
		};

		// Template specialisation when the member variable is a float
		template <typename MetaVariable>
		struct convertType<float, MetaVariable> {
			std::string value;

			convertType(float const &) : value("REAL") { };
		};

		// Template specialisation when the member variable is a bool
		template <typename MetaVariable>
		struct convertType<bool, MetaVariable> {
			std::string value;

			convertType(bool const &) : value("BIT") { };
		};

		struct get_column_types {
			Object const & obj;
			std::stringstream & query;

			get_column_types(Object const & obj, std::stringstream & query) : obj(obj), query(query) { };
			template <class IterationInfo>
			inline void operator () (IterationInfo) {
				if (IterationInfo::is_first::value) query << "(";

				query <<
					// Create a c-style string ...
					mirror::cts::c_str<
						// ... from the concatenation of ...
						mirror::cts::concat<
							// ... the member variable's static name ...
							mirror::static_name<
								typename IterationInfo::type
							>,
							// ... and a space
							mirror::cts::string<' '>
						>
					>() <<
					convertType<
						// Pass the member variable's base/original type (std::string, unsigned int, etc)
						typename IterationInfo::type::type::original_type,
						// And the MetaObject 
						typename IterationInfo::type
					// And the actual value as argument, then access the "value" property of the object
					// that was constructed.
					>(IterationInfo::type::get(this->obj)).value;
						
				if (not IterationInfo::is_last::value) query << ", ";
				else query << ")";
			};

		};

		static void setup(Object const & obj, std::string const & create_table_query = std::string()) {
			namespace pk = Persistent::keywords;

			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();
			std::stringstream query;
			get_column_types get_col_types(obj, query);

			if (create_table_query.empty()) {
				query <<
					// Create a c-style string ...
					mirror::cts::c_str<
						// ... from the concatenation of ...
						mirror::cts::concat<
							// ... the create table statement, and ...
							pk::create_table,
							// ... the if not exists statement, and ...
							pk::if_not_exists,
							// ... the object's name
							mirror::static_name<mirror::reflected<Object>>
						>
					>();
	
				// For every object in the range ...
				mirror::mp::for_each_ii<
					// ... filtered, so that only ...
					mirror::mp::only_if<
						// ... member variables of the class Object are looped on ...
						mirror::member_variables<mirror::reflected<Object>>,
						mirror::mp::is_a<
							mirror::mp::arg<1>,
							mirror::meta_member_variable_tag
						>
					>
				// execute the provided functor
				>(get_col_types);

			}

			else {
				query << create_table_query;
			}
	
			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			Storage::resetStatement();
		};

		static void commit(Object const & obj) {
			namespace pk = Persistent::keywords;

			auto meta_obj = puddle::reflected_type<Object>();
			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();
			std::stringstream query;
			grab_names_and_values grab_names_and_values_(obj, query);
	
			query <<
				// Create a c-style string ...
				mirror::cts::c_str<
					// ... from the concatenation of ...
					mirror::cts::concat<
						// ... the update statement, and ...
						pk::update,
						// ... the object's name, and ...
						mirror::static_name<mirror::reflected<Object>>,
						// ... the set statement
						pk::set
					>
				>();

			meta_obj.member_variables().for_each(grab_names_and_values_);

			query <<
				// create a c-style string from ...
				mirror::cts::c_str<
					// ... the where statement
					pk::where
				>() << "id = :id" << boost::lexical_cast<std::string>(counter);

			st.exchange(soci::use(obj.id, "id" + boost::lexical_cast<std::string>(counter++)));

			st.alloc();
			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			Storage::resetStatement();
		};

	};

/* Close namespaces */
		}
	}
}

#endif
