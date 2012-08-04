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

#include "module.hpp"

namespace firestarter { namespace module {
	DECLARE_EXTERN_LOG(logger);
} }

using namespace firestarter::module;

void RunnableModule::shutdown() { 
	LOG_WARN(logger, "shutdown() not implemented in RunnableModule (this = " << this << ")!");
}

void RunnableModule::restart() { 
	LOG_WARN(logger, "restart() not implemented in RunnableModule (this = " << this << ")!");
};

void RunnableModule::_initialiser() {
	using namespace firestarter::protocol::module;

	LOG_DEBUG(logger, "Waiting for InstanceManager orders");

	while (this->running || this->runlevel == NONE) {
		RunlevelRequest order;

		if (this->manager_socket.receive(order, true) && order.type() == UPDATE) {
			LOG_DEBUG(logger, "Message received from manager.");
		}

		else {
			LOG_ERROR(logger, order.DebugString());
			LOG_ERROR(logger, order.InitializationErrorString());
			if (order.IsInitialized()) {
				LOG_ERROR(logger, "Weird, the message is initialised!");
			}
			LOG_WARN(logger, "Couldn't parse message received from manager. Shutting down.");
			return;
		}

		switch(order.runlevel()) {
			case INIT: {
				LOG_DEBUG(logger, "Calling setup() after receiving INIT message.");
				this->setup();
				LOG_DEBUG(logger, "Creating response message");
				RunlevelResponse response;
				response.set_runlevel(order.runlevel());
				response.set_result(SUCCESS);
				if (not this->manager_socket.send(response)) {
					LOG_ERROR(logger, "Response couldn't be sent to manager!");
					LOG_INFO(logger, "ZMQ error (" << errno << ") message: " << zmq_strerror(zmq_errno()));
					return;
				}
				break;
			}

			case RUNNING: {
				LOG_DEBUG(logger, "Received RUNNING message, sending reply first.");
				LOG_DEBUG(logger, "Creating response message");
				RunlevelResponse response;
				response.set_runlevel(order.runlevel());
				response.set_result(SUCCESS);
				if (not this->manager_socket.send(response)) {
					LOG_ERROR(logger, "Response couldn't be sent to manager!");
					return;
				}
				LOG_DEBUG(logger, "Message sent, calling run().");
				this->run();
				break;
			}

			default:
				LOG_WARN(logger, "Unexpected runlevel change request received. Shutting down.");
				return;
				break;
		}
	}
};
