#include "modulemanager.h"

namespace firestarter { namespace ModuleManager {
	DECLARE_LOG(logger, "firestarter.ModuleManager");
} }

using namespace firestarter::ModuleManager;

ModuleManager::ModuleManager(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException):
	configuration(config) {

	using namespace libconfig;
	using namespace std;

	if (not config.exists("application.modules")) {
		LOG_ERROR(logger, "Configuration file does not contain application.modules.");
		throw firestarter::exception::InvalidConfigurationException("Configuration file does not contain application.modules.");
	}

	/** The following two lines are supposed to be used should we need to support platforms 
	  * that do not use dynamic libraries, however, if we do use it, there is a conflict 
	  * with how Boost.Test is designed. 
	  * As I understand it, libltdl injects a specific symbol (lt__PROGRAM__LTX_preloaded_symbols) 
	  * into the main executable. The problematic issue is that the main executable is provided 
	  * by the Boost.Test shared library. Hence, no insertion of symbol can be done. */
#ifndef IN_UNIT_TESTING
	LOG_DEBUG(logger, "Setting preloaded symbols.");
	LTDL_SET_PRELOADED_SYMBOLS();
#endif

	LOG_DEBUG(logger, "Initialising ltdl library.");
	this->ltdl = lt_dlinit();

	if (ltdl != 0) {
		LOG_ERROR(logger, "Unable to initialise ltdl library:");
		while (this->ltdl-- > 0)
			LOG_ERROR(logger, "lt_dlerror(): " << lt_dlerror());
		return;
	}

	if (this->configuration.exists("application.module_path")) {
		this->module_path = (const char *) this->configuration.lookup("application.module_path");
		if (not this->module_path.empty()) {
			LOG_DEBUG(logger, "Setting module search path to `"<< this->module_path << "'.");
		}
	}

	else {
		LOG_WARN(logger, "Couldn't find module_path configuration key.");
		if (lt_dlgetsearchpath() == NULL) {
			LOG_DEBUG(logger, "Default search path: `NULL'.");
			LOG_DEBUG(logger, "Setting default search path to: `/usr/lib/firestarter'.");
			this->module_path = "/usr/lib/firestarter";
		}

		else {
			LOG_DEBUG(logger, "Default search path: `NULL'.");
			this->module_path = lt_dlgetsearchpath();
		}
	}

	if (not module_path.empty()) {
		LOG_INFO(logger, "Setting ltdl library search path to " << this->module_path);
		lt_dlsetsearchpath(this->module_path.c_str());
	}

	this->lookupDependencies(this->configuration);

	this->dependencies.resolve();
}

ModuleManager::~ModuleManager() {
	foreach(ModuleMap::value_type module, this->modules) {
		lt_dlhandle * moduleHandle = module.second.get<2>();
		if (moduleHandle != NULL && lt_dlclose(*moduleHandle) != 0)
			LOG_ERROR(logger, "An error occured while closing module `" << module.first << "': " << lt_dlerror());
	}

	if (ltdl == 0) {
		LOG_DEBUG(logger, "Shutting down ltdl library.");
		this->ltdl = lt_dlexit();
		if (ltdl != 0) {
			LOG_ERROR(logger, "Unable to shut down ltdl library:");
			while (this->ltdl-- > 0)
				LOG_ERROR(logger, "lt_dlerror(): " << lt_dlerror());
		}
	}

}

void ModuleManager::lookupDependencies(const libconfig::Config & config) throw(firestarter::exception::InvalidConfigurationException) {

	using namespace libconfig;
	using namespace std;

	if (not config.exists("application.modules") && not config.exists("module.components")) {
		LOG_ERROR(logger, "Configuration file does not contain application.modules nor module.components");
		throw firestarter::exception::InvalidConfigurationException("Configuration file does not contain application.modules nor module.components.");
	}

	string configuration_key = "application.modules";
	string parent_name = "root";

	if (config.exists("module.components")) {
		configuration_key = "module.components";
		parent_name = (const char *) config.lookup("module.name");
	}

	Setting & module_dependencies = config.lookup(configuration_key);

	for (int i = 0; i < module_dependencies.getLength(); i++) {
		string module_name = module_dependencies[i];

		this->dependencies.addDependency(module_name, parent_name);

		if (this->modules.find(module_name) == this->modules.end()) {

			/** Using a pointer instead of a reference because libconfig::Config does not support
			  * Copy constructor... Not really sure why, as I use it fine in this constructor...
			  * \todo Make libconfig::Config in ModuleInfo a reference rather than a pointer. 
			  * \todo Catch/throw exceptions */
			Config * module_config = this->loadModuleConfiguration(module_name);
	
			LOG_INFO(logger, "Inserting `" << module_name << "' into ModuleMap");
			this->modules[module_name] = ModuleInfo(module_config, 1, static_cast<lt_dlhandle *>(NULL), 
			                                                          static_cast<create_module *>(NULL), 
			                                                          static_cast<destroy_module *>(NULL));

			if (module_config->exists("module.components"))
				this->lookupDependencies(*module_config);


		}

	}

}

libconfig::Config * ModuleManager::loadModuleConfiguration(const std::string & module_name) {

	using namespace libconfig;

	Config * module_config = new Config();

	try {
		/** \todo Use Boost.Filesystem to convert the slash into platform independent path separator. */
		std::string config_file = this->module_path + '/' + module_name + ".cfg";
		boost::algorithm::to_lower(config_file);
		LOG_DEBUG(logger, "Attempting to read `" << config_file << "'.");
		module_config->readFile(config_file.c_str());
	}

	catch (ParseException pex) {
		LOG_ERROR(logger, "Invalid configuration file for module `" << module_name << "': Parse Exception.");
		throw firestarter::exception::InvalidConfigurationException("Invalid configuration file for module: Parse Exception.");
	}

	catch (FileIOException fex) {
		LOG_ERROR(logger, "Could not read configuration file for module `" << module_name << "': File IO Exception.");
		throw firestarter::exception::InvalidConfigurationException("Could not read configuration file for module: File IO Exception.");
	}

	return module_config;
}

void ModuleManager::loadModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException) {
	throw firestarter::exception::ModuleNotFoundException();
	/** \todo Implement function */
}

void ModuleManager::loadModules() {
	/** \todo Implement function */
}

ModuleInfo & ModuleManager::getModule(const std::string & name) throw(firestarter::exception::ModuleNotFoundException) {
	/** \todo Implement try loading module before throwing */
	try {
		return this->modules.at(name);
	}

	catch (...) {
		LOG_ERROR(logger, "Exception occured in ModuleManager::getModule() when trying to access modules[" << name << "].");
		throw firestarter::exception::ModuleNotFoundException();
	}
}
