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

				struct insert_into : mirror::cts::string<
					'I', 'N', 'S', 'E', 'R', 'T', ' ', 'I', 'N', 'T', 'O', ' '
				> { };

				struct values : mirror::cts::string<
					' ', 'V', 'A', 'L', 'U', 'E', 'S', ' '
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
		};
	};

	struct PartialQuery {
		std::string content;
		PartialQuery(std::string content) : content(content) { };

		operator std::string () {
			return this->content;
		};

		friend std::ostream & operator<<(std::ostream & os, PartialQuery const & pq) {
			os << pq.content;
			return os;
		};

		PartialQuery operator&&(PartialQuery right) {
			std::stringstream ss;
			ss << "(" << this->content << " AND " << right.content << ")";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator||(PartialQuery right) {
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
					mirror::cts::c_str<typename MetaMemberVariable::scope::static_name>(),
					mirror::cts::c_str<typename MetaMemberVariable::static_name>()
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
							mirror::members<MIRRORED_CLASS(Object)>,
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
				indicators.reserve(
					mirror::mp::size<
						mirror::mp::only_if<
							mirror::members<MIRRORED_CLASS(Object)>,
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
			grab_values gv(obj, query);
			auto meta_obj = puddle::reflected_type<Object>();
			auto st = Storage::getStatement();

			query <<
				mirror::cts::c_str<
					mirror::cts::concat<
						pk::insert_into,
						mirror::static_name<MIRRORED_CLASS(Object)>,
						mirror::cts::string<' ', '('>,
						column_list_cts,
						mirror::cts::string<')', ' '>,
						pk::values
					>
				>();
			meta_obj.member_variables().for_each(gv);
			query << ";";

			st.get()->prepare(query.str());
			st.get()->define_and_bind();
			st.get()->execute(true);

			Storage::resetStatement();
		};

		static void find(Object & obj, PartialQuery partial_query) {
			namespace pk = Persistent::keywords;

			populate p(obj);
			auto meta_obj = puddle::reflected_type<Object>();
			auto st = Storage::getStatement();

			meta_obj.member_variables().for_each(p);
			st.get()->alloc();

			std::stringstream query;
			query << 
				mirror::cts::c_str<
					mirror::cts::concat<
						pk::select,
						column_list_cts,
						pk::from,
						mirror::static_name<MIRRORED_CLASS(Object)>,
						pk::where
					>
				>() << partial_query << "LIMIT 1";

			st.get()->prepare(query.str());
			st.get()->define_and_bind();
			st.get()->execute(true);

			Storage::resetStatement();
			counter = 0;
		};

		static void find(std::vector<Object> & objects, PartialQuery partial_query,
				unsigned int from = 0)
		{
			namespace pk = Persistent::keywords;

			Object obj;
			populate p(obj);
			auto meta_obj = puddle::reflected_type<Object>();
			auto st = Storage::getStatement();
			unsigned int const limit = objects.capacity();

			meta_obj.member_variables().for_each(p);
			st.get()->alloc();

			std::stringstream query;
			query << 
				mirror::cts::c_str<
					mirror::cts::concat<
						pk::select,
						column_list_cts,
						pk::from,
						mirror::static_name<MIRRORED_CLASS(Object)>,
						pk::where
					>
				>() << partial_query << " LIMIT ";

			if (from != 0)
				query << from << ", ";

			query << limit;

			st.get()->prepare(query.str());
			st.get()->define_and_bind();
			st.get()->execute(true);

			objects.push_back(obj);

			while (st.get()->fetch())
				objects.push_back(obj);

			Storage::resetStatement();
			counter = 0;
		};

	};

/* Close namespaces */
	}
}

#endif
