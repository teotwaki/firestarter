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

#include "webinterface.hpp"

DECLARE_MODULE_LOG(WebInterface);

using namespace firestarter::module::core::WebInterface;

WebInterface::WebInterface(zmq::context_t & context) : RunnableModule(context) {
	LOG_INFO(logger, "WebInterface object being created.");
}

void WebInterface::run() {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "Running the WebInterface's main function.");

	try {
		Router::registerPage<AdminPage>("main");
		Router::registerPage<BlankPage>("blank");
		LOG_DEBUG(logger, "Creating fcgi object.");
		Fastcgipp::Manager<Router> fcgi(this->socket_fd);
		LOG_DEBUG(logger, "Calling fcgi.handler().");
		fcgi.handler();
	}
	catch (std::exception & e) {
		LOG_ERROR(logger, "Exception caught while running fcgi handler:");
		LOG_ERROR(logger, e.what());
	}
}

void WebInterface::setup() {
	std::string socket_path = "/tmp/fstest.socket";

	LOG_INFO(logger, "WebInterface being set up.");
	LOG_DEBUG(logger, "Creating Unix Domain Socket at " << socket_path);

	int len;
	struct sockaddr_un local, remote;

	int old_umask = ::umask(~(0777&0777));

    if ((this->socket_fd = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		LOG_ERROR(logger, "Couldn't socket().");
		return;
    }

	local.sun_family = AF_UNIX;
	::strcpy(local.sun_path, socket_path.c_str());
	::unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (::bind(this->socket_fd, (struct sockaddr *)&local, len) == -1) {
		LOG_ERROR(logger, "Couldn't bind().");
		return;
	}

	::umask(old_umask);

	if (::listen(this->socket_fd, 5) == -1) {
		LOG_ERROR(logger, "Couldn't listen().");
		return;
	}
}

void WebInterface::shutdown() {
	::unlink("/tmp/fstest.socket");
}
