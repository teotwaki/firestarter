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

#include "modulemanager.hpp"

namespace firestarter { namespace ModuleManager {
	DECLARE_LOG(logger, "firestarter.ModuleManager");
} }

using namespace firestarter::ModuleManager;

ModuleManager::ModuleManager(const libconfig::Config & config) 
	throw(firestarter::exception::InvalidConfigurationException) :
	configuration(config) {

	using namespace libconfig;
	using namespace std;

	if (not config.exists("application.modules")) {
		LOG_ERROR(logger, "Configuration file does not contain application.modules.");
		throw firestarter::exception::InvalidConfigurationException(
			"Configuration file does not contain application.modules.");
	}

	/** The following two lines are supposed to be used should we need to support platforms that do not use dynamic
	  * libraries, however, if we do use it, there is a conflict with how Boost.Test is designed. As I understand it,
	  * libltdl injects a specific symbol (lt__PROGRAM__LTX_preloaded_symbols) into the main executable. The
	  * problematic issue is that the main executable is provided by the Boost.Test shared library. Hence, no insertion
	  * of symbol can be done. Furthermore, if the following is active, we can't compile with clang for some reason.
	  * I don't really have time to check this out, and just want to get on with things; if we ever need support for
	  * platforms that require static linking. (Remove #if 0 to activate) */
#if 0
#ifndef IN_UNIT_TESTING
	LOG_DEBUG(logger, "Setting preloaded symbols.");
	LTDL_SET_PRELOADED_SYMBOLS();
#endif
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
			LOG_DEBUG(logger, "Setting default search path to: `" LIBDIR "'.");
			this->module_path = LIBDIR;
		}

		else {
			LOG_DEBUG(logger, "Default search path: `NULL'.");
			this->module_path = lt_dlgetsearchpath();
		}
	}

	if (not this->module_path.empty()) {
		LOG_INFO(logger, "Setting ltdl library search path to " << this->module_path);
		lt_dlsetsearchpath(this->module_path.c_str());
	}

	else {
		LOG_WARN(logger, "Module path configuration is empty.");
		LOG_DEBUG(logger, "Module search path: " << lt_dlgetsearchpath());
	}

	LOG_DEBUG(logger, "Initialising ltdl advise.");
	lt_dladvise_init(&(this->advise));
	lt_dladvise_ext(&(this->advise));
	lt_dladvise_global(&(this->advise));

	this->lookupDependencies(this->configuration);

	this->graph.resolve();

	this->loadModules();
}

ModuleManager::~ModuleManager() {
	LOG_INFO(logger, "Destructing ModuleManager object (" << this << ").");

	LOG_DEBUG(logger, "Unloading modules.");
	for (ModuleMap::value_type module : this->modules) {
		LOG_DEBUG(logger, "Closing module `" << module.first <<"'.");
		if (lt_dlclose(module.second->getHandle()) != 0) {
			LOG_ERROR(logger, "An error occured while closing module `" << module.first << "': " << lt_dlerror());
		}
	}

	LOG_DEBUG(logger, "Destroying advise object.");
	lt_dladvise_destroy(&(this->advise));

	if (ltdl == 0) {
		LOG_DEBUG(logger, "Shutting down ltdl library.");
		this->ltdl = lt_dlexit();
		if (ltdl != 0) {
			LOG_ERROR(logger, "Unable to shut down ltdl library:");
			while (this->ltdl-- > 0)
				LOG_ERROR(logger, "lt_dlerror(): " << lt_dlerror());
		}
	}

	LOG_INFO(logger, "ModuleManager correctly shut down. The end is nigh.");
}

void ModuleManager::lookupDependencies(const libconfig::Config & config) 
	throw(firestarter::exception::InvalidConfigurationException) {

	using namespace libconfig;
	using namespace std;

	if (not config.exists("application.modules") && not config.exists("module.dependencies")) {
		LOG_ERROR(logger, "Configuration file does not contain application.modules nor module.dependencies");
		throw firestarter::exception::InvalidConfigurationException(
			"Configuration file does not contain application.modules nor module.dependencies.");
	}

	string configuration_key = "application.modules";
	string parent_name = "root";

	if (config.exists("module.dependencies")) {
		configuration_key = "module.dependencies";
		parent_name = (const char *) config.lookup("module.name");
	}

	Setting & module_dependencies = config.lookup(configuration_key);

	for (int i = 0; i < module_dependencies.getLength(); i++) {
		string module_name = module_dependencies[i];

		this->graph.addDependency(module_name, parent_name);

		if (this->modules.find(module_name) == this->modules.end()) {

			/** Using a pointer instead of a reference because libconfig::Config does not support copy constructor...
			  * Not really sure why, as I use it fine in this constructor...
			  * \todo Make libconfig::Config in ModuleInfo a reference rather than a pointer. 
			  * \todo Catch/throw exceptions */
			Config * module_config = this->loadModuleConfiguration(module_name);
	
			LOG_INFO(logger, "Inserting `" << module_name << "' into ModuleMap");
			this->modules[module_name] = new ModuleInfo();
			this->modules[module_name]->setConfiguration(module_config);

			if (module_config->exists("module.dependencies"))
				this->lookupDependencies(*module_config);

		}

	}

}

libconfig::Config * ModuleManager::loadModuleConfiguration(const std::string & module_name) {

	using namespace libconfig;

	Config * module_config = new Config();

	try {
		/// \todo Use Boost.Filesystem to convert the slash into platform independent path separator.
		std::string config_file_path = MODCONFDIR "/" + module_name + ".cfg";
		boost::algorithm::to_lower(config_file_path);
		LOG_DEBUG(logger, "Attempting to read `" << config_file_path << "'.");
		module_config->readFile(config_file_path.c_str());
	}

	catch (ParseException pex) {
		LOG_ERROR(logger, "Invalid configuration file for module `" << module_name << "': Parse Exception.");
		throw firestarter::exception::InvalidConfigurationException(
			"Invalid configuration file for module: Parse Exception.");
	}

	catch (FileIOException fex) {
		LOG_ERROR(logger, "Could not read configuration file for module `" << module_name << "': File IO Exception.");
		throw firestarter::exception::InvalidConfigurationException(
			"Could not read configuration file for module: File IO Exception.");
	}

	return module_config;
}

void ModuleManager::loadModule(const std::string & module_name) 
	throw(firestarter::exception::ModuleNotFoundException) {

	using namespace std;

	LOG_INFO(logger, "Attempting to load module `" << module_name << "'.");

	if (modules.find(module_name) == modules.end()) {
		LOG_ERROR(logger, "Couldn't find module `" << module_name << "' in list of modules.");
		throw firestarter::exception::ModuleNotFoundException("Coulnd't find module in list of modules.");
	}

	LOG_DEBUG(logger, "Converting module name to lowercase");
	string module_name_lowercase = module_name;
	boost::algorithm::to_lower(module_name_lowercase);

	ModuleInfo * module = this->getModuleInfo(module_name);

	LOG_DEBUG(logger, "Opening module's shared library");
	module->setHandle(lt_dlopenadvise(module_name_lowercase.c_str(), this->advise));

	if (module->getHandle() == NULL) {
		LOG_ERROR(logger, "An error occured while opening `" << module_name_lowercase << "'.");
#ifdef LTDL_DEBUG
		string error;
		if ((error = lt_dlerror()) != "file not found") {
			LOG_ERROR(logger, error);
		}

		else {
			LOG_DEBUG(logger, "lt_dlerror() is useless. We need to trigger another error with dlerror().");
			if (dlopen((this->module_path + '/' + module_name_lowercase + ".so").c_str(), RTLD_NOW) == NULL)
				LOG_ERROR(logger, dlerror());
		}
#else
		LOG_ERROR(logger, lt_dlerror());
#endif
		throw firestarter::exception::ModuleNotFoundException();
	}
	
	LOG_DEBUG(logger, "Retrieving module's factory symbol");
	create_module * factory = reinterpret_cast<create_module *>(lt_dlsym(module->getHandle(),
	                                                                     ("create" + module_name).c_str()));
	if (factory == NULL) {
		LOG_ERROR(logger, "Unable to load symbol `create" << module_name << "'.");
		/// \todo Throw exception if unable to load symbol
	}

	LOG_DEBUG(logger, "Retrieving module's destructor symbol");
	destroy_module * destructor = reinterpret_cast<destroy_module *>(lt_dlsym(module->getHandle(),
	                                                                          ("destroy" + module_name).c_str()));
	if (destructor == NULL) {
		LOG_ERROR(logger, "Unable to load symbol `destroy" << module_name << "'.");
		/// \todo Throw exception if unable to load symbol
	}

	LOG_DEBUG(logger, "Retrieving module's version symbol");
	module_version * version = reinterpret_cast<module_version *>(lt_dlsym(module->getHandle(), 
	                                                                       ("version" + module_name).c_str()));
	if (version == NULL) {
		LOG_ERROR(logger, "Unable to load symbol `version" << module_name << "'.");
		/// \todo Throw exception if unable to load symbol
	}

	LOG_DEBUG(logger, "Storing module's information into modules (" << module << ")");
	LOG_DEBUG(logger, "Storing module version from " << version);
	module->setVersion(version != NULL ? version() : 1);
	LOG_DEBUG(logger, "Storing module's factory " << factory);
	module->setFactory(factory);
	LOG_DEBUG(logger, "Storing module's destructor " << destructor);
	module->setRecyclingFacility(destructor);

}

void ModuleManager::loadModules() {
	LOG_INFO(logger, "Attempting to open all modules.");
	std::list<std::string> * module_list = this->graph.getModules();

	for (std::string module_name : *module_list) {
		this->loadModule(module_name);
	}
}

ModuleInfo * ModuleManager::getModuleInfo(const std::string & name) 
	throw(firestarter::exception::ModuleNotFoundException) {

	/// \todo Implement try loading module before throwing
	try {
		return this->modules.at(name);
	}

	catch (...) {
		LOG_ERROR(logger, 
			"Exception occured in ModuleManager::getModule() when trying to access modules[" << name << "].");
		throw firestarter::exception::ModuleNotFoundException();
	}
}

