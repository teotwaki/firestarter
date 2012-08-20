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

#include "log.hpp"

#include <mirror/mirror.hpp>
#include <puddle/puddle.hpp>
#include <puddle/auxiliary/wrap.hpp>
#include <mirror/ct_string/concat.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#ifdef HAVE_CONFIG_H
  #include "config.h"

  #ifdef HAVE_SOCI_SQLITE
    #include "vendor/sqlite/sqlite.hpp"
  #endif

  #ifdef HAVE_SOCI_MYSQL
    #include "vendor/mysql/mysql.hpp"
  #endif

  #ifdef HAVE_SOCI_PGSQL
    #include "vendor/postgresql/postgresql.hpp"
  #endif
#endif

namespace firestarter {
	namespace common {
		namespace Persistent {

			namespace keywords {
				struct select : mirror::cts::string<
					'S', 'E', 'L', 'E', 'C', 'T', ' '
				> { };

				struct from : mirror::cts::string<
					' ', 'F', 'R', 'O', 'M', ' '
				> { };

				struct or_ : mirror::cts::string<
					' ', 'O', 'R', ' '
				> { };
	
				struct and_ : mirror::cts::string<
					' ', 'A', 'N', 'D', ' '
				> { };

				struct delete_ : mirror::cts::string<
					'D', 'E', 'L', 'E', 'T', 'E', ' '
				> { };

				struct where : mirror::cts::string<
					' ', 'W', 'H', 'E', 'R', 'E', ' '
				> { };

				struct limit : mirror::cts::string<
					' ', 'L', 'I', 'M', 'I', 'T', ' '
				> { };

				struct order_by : mirror::cts::string<
					' ', 'O', 'R', 'D', 'E', 'R', ' ', 'B', 'Y', ' '
				> { };

				struct asc : mirror::cts::string<
					' ', 'A', 'S', 'C', ' '
				> { };

				struct desc : mirror::cts::string<
					' ', 'D', 'E', 'S', 'C', ' '
				> { };

				struct create_table : mirror::cts::string<
					'C', 'R', 'E', 'A', 'T', 'E', ' ', 'T', 'A', 'B', 'L', 'E', ' '
				> { };

				struct if_not_exists : mirror::cts::string<
					' ', 'I', 'F', ' ', 'N', 'O', 'T', ' ', 'E', 'X', 'I', 'S', 'T', 'S', ' '
				> { };

				struct insert_into : mirror::cts::string<
					'I', 'N', 'S', 'E', 'R', 'T', ' ', 'I', 'N', 'T', 'O', ' '
				> { };

				struct values : mirror::cts::string<
					' ', 'V', 'A', 'L', 'U', 'E', 'S', ' '
				> { };

				struct count : mirror::cts::string<
					' ', 'C', 'O', 'U', 'N', 'T', '(', '*', ')', ' '
				> { };
			}

		}

	static __thread soci::session sql;
	static __thread boost::shared_ptr<soci::statement> statement;
	static __thread unsigned int counter;
	static __thread std::vector<boost::shared_ptr<soci::indicator>> indicators;

	class Storage {
		public:
		static boost::shared_ptr<soci::statement> getStatement() {
			if (!statement)
				statement = boost::shared_ptr<soci::statement>(new soci::statement(sql));
				
			return statement;
		};

		static inline void connect(std::string const & connection_string) {
			sql.open(connection_string);
		};

		static inline void resetStatement() {
			statement.reset();
			counter = 0;
		};
	};

	struct PartialQuery {
		std::string content;
		PartialQuery(std::string const & content) : content(content) { };

		operator std::string () {
			return this->content;
		};

		friend std::ostream & operator<<(std::ostream & os, PartialQuery const & pq) {
			os << pq.content;
			return os;
		};

		PartialQuery operator&&(PartialQuery const & right) {
			std::stringstream ss;
			ss << "(" << this->content << " AND " << right.content << ")";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator||(PartialQuery const & right) {
			std::stringstream ss;
			ss << "(" << this->content << " OR " << right.content << ")";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

	};

	template <typename T>
	struct QueryLexer {	
		std::string content;
		std::string column_name;
		boost::shared_ptr<soci::statement> statement;

		QueryLexer(std::string const & table_name, std::string const & column_name) :
			content(table_name + "." + column_name), 
			column_name(column_name) {
			this->statement = Storage::getStatement();
		};

		inline PartialQuery handle(T const & right, std::string const & op) {
			this->statement.get()->exchange(
				soci::use(right, this->column_name + boost::lexical_cast<std::string>(counter))
			);
			this->content += op + " :" + this->column_name + boost::lexical_cast<std::string>(counter++);
			return PartialQuery(this->content);
		};

		inline PartialQuery operator==(T const & right) {
			return handle(right, " =");
		};

		inline PartialQuery operator!=(T const & right) {
			return handle(right, " !=");
		};

		inline PartialQuery operator<(T const & right) {
			return handle(right, " <");
		};

		inline PartialQuery operator>(T const & right) {
			return handle(right, " >");
		};

		inline PartialQuery operator<=(T const & right) {
			return handle(right, " <=");
		};

		inline PartialQuery operator>=(T const & right) {
			return handle(right, " >=");
		};

		inline PartialQuery operator%=(T const & right) {
			return handle(right, " LIKE");
		};

	};

	template <typename MetaClass>
	struct ClassTransf {
		struct type {
			template <typename Param>
			type(Param const &) { };
		};
	};

	template <class MetaMemberVariable>
	struct MemVarTransf {
		struct type : puddle::aux::wrap<MetaMemberVariable>::type {
			template <class ClassTranf>
			type(ClassTranf &, int) : puddle::aux::wrap<MetaMemberVariable>::type() { };

			QueryLexer<typename MetaMemberVariable::type::original_type> operator()(void) const {
				return QueryLexer<typename MetaMemberVariable::type::original_type>(
					// Create a c-style string ...
					mirror::cts::c_str<
						// ... of the name ...
						mirror::static_name<
							// ... of MetaMemberVariable's class
							mirror::scope<MetaMemberVariable>
						>
					>(),
					// Create a c-style string ...
					mirror::cts::c_str<
						// ... of MetaMemberVariable
						mirror::static_name<
							MetaMemberVariable
						>
					>()
				);
			};
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
				this->st.get()->exchange(soci::use(var, meta_var.base_name()));

				if (first) query << "(";
				query << ":" << meta_var.base_name();
				if (not last) query << ", ";
				else query << ")";
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
						mirror::static_name<mirror::reflected<Object>>,
						// ... the where statement
						pk::where
					>
				>() << partial_query;

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
						mirror::static_name<mirror::reflected<Object>>,
						// ... the where statement
						pk::where
					>
				>() << partial_query << " LIMIT 1";

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
						mirror::static_name<mirror::reflected<Object>>,
						// ... the where statement
						pk::where
					>
				>() << partial_query << " LIMIT ";

			if (from != 0)
				query << from << ", ";

			query << limit;

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
				this->value = mirror::cts::c_str<
					mirror::static_name<
						typename MetaVariable::type
					>
				>();
				boost::to_upper(this->value);
			};
		};

		template <typename MetaVariable>
		struct convertType<std::string, MetaVariable> {
			std::string value;

			convertType(std::string const & value) {
				if (value.capacity() < 256)
					this->value = "VARCHAR(" + boost::lexical_cast<std::string>(value.capacity()) + ")";
				else
					this->value = "TEXT";
			};
		};

		template <typename MetaVariable>
		struct convertType<float, MetaVariable> {
			std::string value;

			convertType(float const &) : value("REAL") { };
		};

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
					mirror::cts::c_str<
						mirror::cts::concat<
							mirror::static_name<
								typename IterationInfo::type
							>,
							mirror::cts::string<' '>
						>
					>() <<
					convertType<
						typename IterationInfo::type::type::original_type,
						typename IterationInfo::type
					>(IterationInfo::type::get(this->obj)).value;
						
				if (not IterationInfo::is_last::value) query << ", ";
				else query << ")";
			};

		};

		static void setup(Object const & obj) {
			namespace pk = Persistent::keywords;

			auto meta_obj = puddle::reflected_type<Object>();
			auto st_ptr = Storage::getStatement();
			auto & st = *st_ptr.get();
			std::stringstream query;
			get_column_types get_col_types(obj, query);

			query <<
				mirror::cts::c_str<
					mirror::cts::concat<
						pk::create_table,
						pk::if_not_exists,
						mirror::static_name<mirror::reflected<Object>>
					>
				>();

			mirror::mp::for_each_ii<
				mirror::mp::only_if<
					mirror::member_variables<mirror::reflected<Object>>,
					mirror::mp::is_a<
						mirror::mp::arg<1>,
						mirror::meta_member_variable_tag
					>
				>
			>(get_col_types);

			st.prepare(query.str());
			st.define_and_bind();
			st.execute(true);

			Storage::resetStatement();
		};

	};

/* Close namespaces */
	}
}

#endif
