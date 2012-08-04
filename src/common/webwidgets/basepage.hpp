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

#ifndef FIRESTARTER_BASEPAGE_HPP
#define FIRESTARTER_BASEPAGE_HPP

#include "htmltemplates.hpp"

#include <string>
#include <fastcgi++/http.hpp>

namespace firestarter {
	namespace common {
		namespace WebWidgets {
			namespace Pages {

	class WebPage {
		protected:
		typedef Fastcgipp::Http::Sessions<std::string> Sessions;
		static Sessions sessions;
		Sessions::iterator session;
		firestarter::common::WebWidgets::Templates::Headers headers;
		firestarter::common::WebWidgets::Templates::Tags::Html html;
		virtual bool response(/*const Fastcgipp::Http::Environment<char> & environment*/) = 0;

		public:
		std::string render();
	};

/* Close namespaces */
			}
		}
	}
}

#endif
