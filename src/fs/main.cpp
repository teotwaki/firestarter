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

#include "main.hpp"

DECLARE_LOG(logger, "main");

int main(void) {

	using namespace firestarter::ModuleManager;
	using namespace firestarter::InstanceManager;

	set_logfile_name("main");
	LOG_INFO(logger, "Firestarter initialising.");

	libconfig::Config config;

	try {
		LOG_DEBUG(logger, "Loading the configuration...");
		config.readFile(SYSCONFDIR "/fs.cfg");
	}

	catch (libconfig::SettingTypeException e) {
		LOG_ERROR(logger, "Caught SettingTypeException while loading the configuration file.");
	}

	catch (libconfig::SettingNameException e) {
		LOG_ERROR(logger, "Caught SettingNameException while loading the configuration file.");
	}

	catch (libconfig::ParseException e) {
		LOG_ERROR(logger, "Caught ParseException while loading the configuration file.");
	}

	catch (libconfig::FileIOException e) {
		LOG_ERROR(logger, "Caught FileIOException while loading the configuration file.");
	}

	catch (...) {
		LOG_ERROR(logger, "Failed loading the configuration file.");
	}

	LOG_DEBUG(logger, "Creating ZMQ context");
	zmq::context_t context(1);

	ModuleManager module_manager(config);
	InstanceManager instance_manager(module_manager, context);
	instance_manager.runAll();

	boost::posix_time::microseconds delay(1000000);

	while (instance_manager.isRunning()) {
		boost::this_thread::sleep(delay);
	}

}
