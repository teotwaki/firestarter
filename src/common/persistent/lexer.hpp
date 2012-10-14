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

#ifndef FIRESTARTER_PERSISTENT_LEXER_HPP
#define FIRESTARTER_PERSISTENT_LEXER_HPP

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <mirror/mirror.hpp>

#include "persistent/storage.hpp"

namespace firestarter {
	namespace common {
		namespace Persistent {

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

	template <typename MetaMemberVariable>
	struct QueryLexer {	
		typedef typename MetaMemberVariable::type::original_type OriginalType;
		std::string content;
		std::string column_name;
		boost::shared_ptr<soci::statement> statement;

		QueryLexer() {
			this->column_name = // Create a c-style string ...
					mirror::cts::c_str<
						// ... of MetaMemberVariable
						mirror::static_name<
							MetaMemberVariable
						>
					>();

			this->content = mirror::cts::c_str<
						// ... of the name ...
						mirror::static_name<
							// ... of MetaMemberVariable's class
							mirror::scope<MetaMemberVariable>
						>
					>() + std::string(".") + this->column_name;

			this->statement = Storage::getStatement();
		};

		inline PartialQuery handle(OriginalType const & right, std::string const & op) {
			this->statement.get()->exchange(
				soci::use(right, this->column_name + boost::lexical_cast<std::string>(counter))
			);
			this->content += op + " :" + this->column_name + boost::lexical_cast<std::string>(counter++);
			return PartialQuery(this->content);
		};

		inline PartialQuery operator==(OriginalType const & right) {
			return handle(right, " =");
		};

		inline PartialQuery operator!=(OriginalType const & right) {
			return handle(right, " <>");
		};

		inline PartialQuery operator<(OriginalType const & right) {
			return handle(right, " <");
		};

		inline PartialQuery operator>(OriginalType const & right) {
			return handle(right, " >");
		};

		inline PartialQuery operator<=(OriginalType const & right) {
			return handle(right, " <=");
		};

		inline PartialQuery operator>=(OriginalType const & right) {
			return handle(right, " >=");
		};

		inline PartialQuery operator%=(OriginalType const & right) {
			return handle(right, " LIKE");
		};

	};

		}
	}
}

#endif
