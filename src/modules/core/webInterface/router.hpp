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

#ifndef FIRESTARTER_ROUTER_HPP
#define FIRESTARTER_ROUTER_HPP

#include "log.hpp"
#include "webwidgets/basepage.hpp"

#include <boost/bind.hpp>
#include <fastcgi++/request.hpp>
#include <boost/functional/factory.hpp>
#include <boost/function.hpp>
#include <map>
#include <memory>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class Router : public Fastcgipp::Request<char> {
		protected:
		typedef boost::function<firestarter::common::WebWidgets::Pages::WebPage * 
									(/*const Fastcgipp::Http::Environment<char> & environment*/)> PageFactory;
		static std::map<std::string, PageFactory> page_factory;
		std::unique_ptr<firestarter::common::WebWidgets::Pages::WebPage> instantiate(std::string const & name) {
			return std::unique_ptr<firestarter::common::WebWidgets::Pages::WebPage>(this->page_factory[name]());
		}

		bool response();

		public:
		template <class T> static void registerPage(std::string name) {
			Router::page_factory[name] = boost::factory<T*>();
		}
		
	};

/* Close namespaces */
			}
		}
	}
}

#endif
