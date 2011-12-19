#include "modulemanager.h"

using namespace firestarter::ModuleManager;

ModuleManager::ModuleManager(const libconfig::Config & config) :
	configuration(config) {

#if HAVE_LTDL_H
	LOG_DEBUG(modManLog, "Setting preloaded symbols.");
	LTDL_SET_PRELOADED_SYMBOLS();
	LOG_DEBUG(modManLog, "Initialising ltdl library.");
	this->ltdl = lt_dlinit();

	if (ltdl != 0) {
		LOG_ERROR(modManLog, "Unable to initialise ltdl library:");
		while (this->ltdl-- > 0)
			LOG_ERROR(modManLog, "lt_dlerror(): " << lt_dlerror());
		return;
	}
#endif

	try {
		this->module_path = (const char *) config.lookup("application.module_path");
		LOG_DEBUG(modManLog, "Setting module search path to `" << this->module_path << "'.");
	}

	catch (libconfig::SettingNotFoundException e) {
		LOG_WARN(modManLog, "Couldn't find module_path configuration key.");
#if HAVE_LTDL_H
		LOG_DEBUG(modManLog, "Default search path: `" << lt_dlgetsearchpath() <<"'.");
		this->module_path = lt_dlgetsearchpath();
#else
		LOG_DEBUG(modManLog, "Setting default search path to: `/usr/lib/firestarter'.");
		this->module_path = "/usr/lib/firestarter";
#endif
	}

#if HAVE_LTDL_H
	LOG_DEBUG(modManLog, "Setting ltdl library search path.");
	lt_dlsetsearchpath(module_path.c_str());
#endif

}

ModuleManager::~ModuleManager() {
	foreach(ModuleMap::value_type module, modules) {
#if HAVE_LTDL_H
		lt_dlhandle moduleHandle = module.second.get<3>();	
		if (lt_dlclose(moduleHandle) != 0)
			LOG_ERROR(modManLog, "An error occured while closing module `" << module.first << "': " << lt_dlerror());
#endif
	}

#if HAVE_LTDL_H
	if (ltdl == 0) {
		LOG_DEBUG(modManLog, "Shutting down ltdl library.");
		this->ltdl = lt_dlexit();
		if (ltdl != 0) {
			LOG_ERROR(modManLog, "Unable to shut down ltdl library:");
			while (this->ltdl-- > 0)
				LOG_ERROR(modManLog, "lt_dlerror(): " << lt_dlerror());
		}
	}
#endif

}

void ModuleManager::loadModule(const std::string & name) {

}

void ModuleManager::loadModules() {

}

ModuleInfo & ModuleManager::getModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException) {
	try {
		return this->modules.at(name);
	}

	catch (...) {
		LOG_ERROR(modManLog, "Exception occured in ModuleManager::getModule() when trying to access modules[" << name << "].");
		throw firestarter::exception::ModuleNotFoundException();
	}
}