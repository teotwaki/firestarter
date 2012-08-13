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

#include <mirror/mirror.hpp>
#include <puddle/puddle.hpp>
#include <puddle/auxiliary/wrap.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include "vendor/sqlite/sqlite.hpp"

namespace firestarter {
	namespace common {

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
		QueryLexer(std::string name) : content(name) { };

		PartialQuery operator==(T right) {
			std::stringstream ss;
			ss << this->content << " = '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator!=(T right) {
			std::stringstream ss;
			ss << this->content << " <> '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator<(T right) {
			std::stringstream ss;
			ss << this->content << " < '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator>(T right) {
			std::stringstream ss;
			ss << this->content << " > '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator<=(T right) {
			std::stringstream ss;
			ss << this->content << " <= '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator>=(T right) {
			std::stringstream ss;
			ss << this->content << " >= '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

		PartialQuery operator%=(T right) {
			std::stringstream ss;
			ss << this->content << " LIKE '" << right << "'";
			this->content = ss.str();
			return PartialQuery(this->content);
		};

	};

	template <typename MetaClass>
	struct ClassTransf {
		struct type {
			template <typename Param>
			type(const Param &) { };
		};
	};

	template <class MetaMemberVariable>
	struct MemVarTransf {
		struct type : puddle::aux::wrap<MetaMemberVariable>::type {
			template <class ClassTranf>
			type(ClassTranf &, int) : puddle::aux::wrap<MetaMemberVariable>::type() { };

			QueryLexer<typename MetaMemberVariable::type::original_type> operator()(void) const {
				std::stringstream ss;
				ss << this->scope().base_name() << "." << this->base_name();
				return QueryLexer<typename MetaMemberVariable::type::original_type>(ss.str());
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

	struct Persistent {

		template <class T>
		struct print_values {
			T const & obj;
			print_values(T const & obj) : obj(obj) { }
			template <class MetaVariable>
			inline void operator () (MetaVariable meta_var, bool first, bool last) {
				std::cout << "'" << meta_var.get(this->obj) << "'";
				if (not last)
					std::cout << ", ";
			};
		};

		struct print_columns {
			template <class MetaVariable>
			inline void operator () (MetaVariable meta_var, bool first, bool last) {
				std::cout << meta_var.base_name();
				if (not last)
					std::cout << ", ";
			};
		};

		template <class Object>
		static void store(Object const & obj) {
			auto meta_obj = puddle::reflected_type<Object>();
			/** \todo Throw when !meta_obj.is_class(), !.is_type, member_variables().empty(), etc... */
			std::cout << "INSERT INTO " << meta_obj.base_name() << " (";
			meta_obj.member_variables().for_each(print_columns());
			std::cout << ") VALUES (";
			print_values<Object> print_values_(obj);
			meta_obj.member_variables().for_each(print_values_);
			std::cout << ");" << std::endl;
		};

		template <class Object>
		static void find(PartialQuery partial_query) {
			auto meta_obj = puddle::reflected_type<Object>();
			std::cout << "SELECT ";
			meta_obj.member_variables().for_each(print_columns());
			std::cout << " FROM " << meta_obj.base_name() << " WHERE " << partial_query << ";" << std::endl;
		};

	};

/* Close namespaces */
	}
}

#endif
