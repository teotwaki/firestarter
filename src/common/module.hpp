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

#ifndef FIRESTARTER_MODULE_HPP
#define FIRESTARTER_MODULE_HPP

#include <string>
#include <cstdlib>
#include <ctime>

#include "helper.hpp"
#include "clients/instancemanager.hpp"
#include "protobuf/module.pb.h"
#include "zmq/zmqhelper.hpp"

namespace firestarter {
	namespace module {

class Module {
	public:
	virtual void setup() = 0; /**< pure virtual */
};

class RunnableModule : public Module {
	protected:
	bool running;
	firestarter::InstanceManager::InstanceManagerClientSocket manager_socket;
	firestarter::protocol::module::RunLevel runlevel;

	RunnableModule(zmq::context_t & context) : running(false), manager_socket(context) , runlevel(firestarter::protocol::module::NONE) { };

	public:
	virtual void run() = 0; /**< pure virtual */
	virtual void shutdown();
	virtual void restart();
	virtual void _initialiser();
	
};

typedef Module * create_module(zmq::context_t & context);
typedef void destroy_module(Module *);
typedef int module_version();

/* Close namespaces */
	}
}

#endif
