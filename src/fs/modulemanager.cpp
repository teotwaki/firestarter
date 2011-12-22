#include "modulemanager.h"

namespace firestarter { namespace ModuleManager {
	DECLARE_LOG(logger, "firestarter.ModuleManager");
} }

using namespace firestarter::ModuleManager;

ModuleManager::ModuleManager(const libconfig::Config & config) :
	configuration(config) {

#if HAVE_LTDL_H
	/* The following two lines are supposed to be used should we need to support platforms 
	   that do not use dynamic libraries, however, if we do use it, there is a conflict 
	   with how Boost.Test is designed. 
	   As I understand it, libltdl injects a specific symbol (lt__PROGRAM__LTX_preloaded_symbols) 
	   into the main executable. The problematic issue is that the main executable is provided 
	   by the Boost.Test shared library. Hence, no insertion of symbol can be done.
	   TODO: Bugfix welcome.
	LOG_DEBUG(logger, "Setting preloaded symbols.");
	LTDL_SET_PRELOADED_SYMBOLS(); */

	LOG_DEBUG(logger, "Initialising ltdl library.");
	this->ltdl = lt_dlinit();

	if (ltdl != 0) {
		LOG_ERROR(logger, "Unable to initialise ltdl library:");
		while (this->ltdl-- > 0)
			LOG_ERROR(logger, "lt_dlerror(): " << lt_dlerror());
		return;
	}
#endif

	try {
		this->module_path = (const char *) this->configuration.lookup("application.module_path");
		if (not this->module_path.empty()) {
			LOG_DEBUG(logger, "Setting module search path to `" << this->module_path << "'.");
		}
	}

	catch (libconfig::SettingNotFoundException e) {
		LOG_WARN(logger, "Couldn't find module_path configuration key.");
#if HAVE_LTDL_H
		if (lt_dlgetsearchpath() == NULL) {
			LOG_DEBUG(logger, "Default search path: `NULL'.");
			LOG_DEBUG(logger, "Setting default search path to: `/usr/lib/firestarter'.");
			this->module_path = "/usr/lib/firestarter";
		}

		else {
			LOG_DEBUG(logger, "Default search path: `NULL'.");
			this->module_path = lt_dlgetsearchpath();
		}
#else
		LOG_DEBUG(logger, "Setting default search path to: `/usr/lib/firestarter'.");
		this->module_path = "/usr/lib/firestarter";
#endif
	}

#if HAVE_LTDL_H
	if (not module_path.empty()) {
		LOG_INFO(logger, "Setting ltdl library search path to " << this->module_path);
		lt_dlsetsearchpath(module_path.c_str());
	}
#endif

}

ModuleManager::~ModuleManager() {
	foreach(ModuleMap::value_type module, modules) {
#if HAVE_LTDL_H
		lt_dlhandle moduleHandle = module.second.get<3>();	
		if (lt_dlclose(moduleHandle) != 0)
			LOG_ERROR(logger, "An error occured while closing module `" << module.first << "': " << lt_dlerror());
#endif
	}

#if HAVE_LTDL_H
	if (ltdl == 0) {
		LOG_DEBUG(logger, "Shutting down ltdl library.");
		this->ltdl = lt_dlexit();
		if (ltdl != 0) {
			LOG_ERROR(logger, "Unable to shut down ltdl library:");
			while (this->ltdl-- > 0)
				LOG_ERROR(logger, "lt_dlerror(): " << lt_dlerror());
		}
	}
#endif

	// TODO: Implement reading list of modules in config file

}

void ModuleManager::loadModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException) {
	throw firestarter::exception::ModuleNotFoundException();
	// TODO: Implement function
}

void ModuleManager::loadModules() {
	// TODO: Implement function
}

ModuleInfo & ModuleManager::getModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException) {
	// TODO: Implement try loading module before throwing
	try {
		return this->modules.at(name);
	}

	catch (...) {
		LOG_ERROR(logger, "Exception occured in ModuleManager::getModule() when trying to access modules[" << name << "].");
		throw firestarter::exception::ModuleNotFoundException();
	}
}
