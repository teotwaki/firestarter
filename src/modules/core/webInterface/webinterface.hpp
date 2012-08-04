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

#ifndef FIRESTARTER_WEBINTERFACE_HPP
#define FIRESTARTER_WEBINTERFACE_HPP

#include "module.hpp"
#include "log.hpp"
#include "router.hpp"
#include "mainpage.hpp"
#include "blankpage.hpp"

#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class WebInterface : public firestarter::module::RunnableModule {
		private:
		int socket_fd;

		public:
		WebInterface(zmq::context_t & context);
		virtual void run();
		virtual void setup();
		virtual void shutdown();
	};

	extern "C" WebInterface * createWebInterface(zmq::context_t & context) {
		return new WebInterface(context);
	}

	extern "C" void destroyWebInterface(Module * webinterface) {
		delete dynamic_cast<WebInterface *>(webinterface);
	}

	extern "C" int versionWebInterface() {
		return 1;
	}

/* Close namespaces */
			}
		}
	}
}

#endif
