#include "main.h"

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
