#include "modulemanager.h"

using namespace firestarter::ModuleManager;

ModuleManager::ModuleManager(const libconfig::Config & config) :
	configuration(config) {

#if HAVE_LTDL_H
	LOG_DEBUG(modManLog, "Setting preloaded symbols.");
	LTDL_SET_PRELOADED_SYMBOLS();
	LOG_DEBUG(modManLog, "Initialising ltdl library.");
	ltdl = lt_dlinit();

	if (ltdl != 0) {
		LOG_ERROR(modManLog, "Unable to initialise ltdl library:");
		while (ltdl-- > 0)
			LOG_ERROR(modManLog, "lt_dlerror(): " << lt_dlerror());
		return;
	}
#endif

	try {
		module_path = (const char *) config.lookup("application.module_path");
		LOG_DEBUG(modManLog, "Setting module search path to `" << module_path << "'.");
	}

	catch (libconfig::SettingNotFoundException e) {
		LOG_WARN(modManLog, "Couldn't find module_path configuration key.");
		LOG_DEBUG(modManLog, "Selecting default `/usr/lib/firestarter'.");
		module_path = "/usr/lib/firestarter";
	}

#if HAVE_LTDL_H
	LOG_DEBUG(modManLog, "Setting ltdl library search path.");
	lt_dlsetsearchpath(module_path.c_str());
#endif

}

ModuleManager::~ModuleManager() {
	// TODO: Close handles to opened modules

#if HAVE_LTDL_H
	if (ltdl == 0) {
		LOG_DEBUG(modManLog, "Shutting down ltdl library.");
		ltdl = lt_dlexit();
		if (ltdl != 0) {
			LOG_ERROR(modManLog, "Unable to shut down ltdl library:");
			while (ltdl-- > 0)
				LOG_ERROR(modManLog, "lt_dlerror(): " << lt_dlerror());
		}
	}
#endif

}

void ModuleManager::loadModule(const std::string & name) {

}

ModuleInfo & ModuleManager::getModule(const std::string & name) {

}
