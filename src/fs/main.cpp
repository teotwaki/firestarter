#include "main.h"

DECLARE_LOG(logger, "main");

int main(void) {

	using namespace firestarter::ModuleManager;

	set_logfile_name("main");
	LOG_INFO(logger, "Firestarter initialising.");

	libconfig::Config	config;

	try {
		LOG_DEBUG(logger, "Loading the configuration...");
		config.readFile("/etc/firestarter/fs.cfg");
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

	ModuleManager module_manager(config);

}
