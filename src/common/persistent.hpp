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

#include <iostream>
#include <string>

namespace firestarter {
	namespace common {

	struct Persistent {

		template <class T>
		struct print {
			T const & obj;
			print(T const & obj) : obj(obj) { }
			template <class MetaObject>
			inline void operator () (MetaObject meta, bool first, bool last) {
				std::cout << meta.base_name() << ": " << meta.get(this->obj) << std::endl;
			};
		};

		template <class Object>
		void store(Object const & obj) {
			auto meta_obj = puddle::reflected_type<Object>();
			/** \todo Throw when !meta_obj.is_class(), !.is_type, member_variables().empty(), etc... */
			std::cout << meta_obj.base_name() << std::endl;
			print<Object> p(obj);
			meta_obj.member_variables().for_each(p);
		};

	};

/* Close namespaces */
	}
}

#endif
