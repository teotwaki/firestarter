#include "main.h"

void loadModules(ModuleMap & modules, list<string> & invalidModules) {
	DECLARE_LOG(loadModLog, "main.loadModules");

#if HAVE_LTDL_H
	LOG_DEBUG(loadModLog, "Compiled with libltdl (required for modules)");
	int ltdl;
	lt_dlhandle module;

	LTDL_SET_PRELOADED_SYMBOLS();
	
	ltdl = lt_dlinit();
	if (ltdl == 0) {
		LOG_DEBUG(loadModLog, "lt_dlinit() success");
		module = lt_dlopenext("/usr/local/lib/firestarter/dummy");
		if (module != 0) {
			LOG_DEBUG(loadModLog, "Correctly loaded dummy.la!");
		}

		else
			LOG_ERROR(loadModLog, "lt_dlopen() failed: " << lt_dlerror());
	}

	else
		LOG_ERROR(loadModLog, "lt_dlinit() failed: " << lt_dlerror());
#else
	LOG_WARN(loadModLog, "Compiled without libltdl (required for modules)");
#endif

#if HAVE_LTDL_H
	if (ltdl == 0) {
		if (module != 0)
			lt_dlclose(module);
		lt_dlexit();
	}
#endif

#if 0

	LOG_DEBUG(loadModLog, "Loading mods/signalemitter.so");
	void * foo = dlopen("mods/signalemitter.so", RTLD_LAZY | RTLD_GLOBAL);
	if (!foo) {
		LOG_ERROR(loadModLog, "Cannot load library: " << dlerror());
		return;
	}

	dlerror();
	LOG_DEBUG(loadModLog, "Loading mods/signalreceiver.so");
	void * bar = dlopen("mods/signalreceiver.so", RTLD_LAZY | RTLD_GLOBAL);
	if (!bar) {
		LOG_ERROR(loadModLog, "Cannot load library: " << dlerror());
		return;
	}
//#else
	LOG_WARN(loadModLog, "No lt_dlopen features available!");
//#endif

	return;

	int position = -1;

	foreach(ModuleMap::value_type i, modules) {
		position++;
		string name = i.first;
		string path = i.second.get<0>();

		if (name.empty()) {
			LOG_WARN(loadModLog, "\tFound empty string in modules at position " << position);
			continue;
		}

		LOG_DEBUG(loadModLog, "Checking if file exists");
		if (file_exists(path)) {
			LOG_INFO(loadModLog, "\t" << name << " -> " << path << ":\t\t[OK]");
			LOG_INFO(loadModLog, "Loading " << path);

			void * module = dlopen(path.c_str(), RTLD_LAZY);
			if (!module) {
				LOG_ERROR(loadModLog, "Cannot load library: " << dlerror());
				invalidModules.push_back(name);
				continue;
			}

			dlerror(); // discard previous errors held in memory

			create_module * create = (create_module *) dlsym(module, ("create" + name).c_str());
			const char * dlsym_error = dlerror();

			if (dlsym_error) {
				LOG_ERROR(loadModLog, "Cannot load symbol create" << name << ": " << dlsym_error);
				invalidModules.push_back(name);
				dlclose(module);
				continue;
			}
			LOG_DEBUG(loadModLog, "Loaded symbol create" << name);

			destroy_module * destroy = (destroy_module *) dlsym(module, ("destroy" + name).c_str());
			dlsym_error = dlerror();

			if (dlsym_error) {
				LOG_ERROR(loadModLog, "Cannot load symbol destroy" << name << ": " << dlsym_error);
				invalidModules.push_back(name);
				dlclose(module);
				continue;
			}
			LOG_DEBUG(loadModLog, "Loaded symbol destroy" << name);

			module_version * version = (module_version *) dlsym(module, ("version" + name).c_str());
			dlsym_error = dlerror();

			if (dlsym_error) {
				LOG_ERROR(loadModLog, "Cannot load symbol version" << name << ": " << dlsym_error);
				invalidModules.push_back(name);
				dlclose(module);
				continue;
			}
			LOG_DEBUG(loadModLog, "Loaded symbol version" << name);

			modules[name] = boost::make_tuple(path, version(), module, create, destroy);
			LOG_DEBUG(loadModLog, "Module stored in modules map");

		}

		else {
			LOG_WARN(loadModLog, "\t" + name + " -> " + path + ":\t\t[FILE NOT FOUND]");
			invalidModules.push_back(name);
		}
	}
#endif
}

ModuleDependencyMap * run_beforeFilter(ModuleMap & modules, list<string> & invalidModules) {
	DECLARE_LOG(bffLog, "main.run_beforeFilter");

	if (modules.size() < 1) {
		LOG_ERROR(bffLog, "No modules to run beforeFilter() on!");
		return NULL;
	}

	ModuleDependencyMap * dependencies = new ModuleDependencyMap;

	foreach(ModuleMap::value_type i, modules) {
		create_module * instantiate = i.second.get<3>();
		Module * module = NULL;

		if (instantiate != NULL)
			module = instantiate(); // instantiate an object
		else
			return NULL;

		module->beforeFilter(); /* Allow the module to populate the list of other modules it is going to need */
		ComponentMap components = module->getComponents();
		// We now have the list of other modules this module wishes to load. If any of these do not validate,
		// Then this module will have to be killed as well. (with "i.second.get<4>()(module)") and added to the
		// invalid modules list.

		if (components.size() > 0) {
			ModuleMap modmap;
			list<string> invalid;

			foreach(ComponentMap::value_type i, components) {
				string name = i.first;
				int version = i.second.first;
				string path = "mods/" + name + ".la";
				boost::to_lower(path);

				modmap[name] = ModuleTuple(path, version, NULL, static_cast<create_module *>(NULL), static_cast<destroy_module *>(NULL));
//				modmap[name] = boost::make_tuple(path, version, NULL, static_cast<create_module *>(NULL), static_cast<destroy_module *>(NULL));
			}
			loadModules(modmap, invalid);

			if (invalid.size() != 0) {
				LOG_ERROR(bffLog, "Some requested submodules have not been loaded properly. " << i.first << " can not be loaded.");
				i.second.get<4>()(module); // destroy the instance
				invalidModules.push_back(i.first);
				continue;
			}

			(*dependencies)[i.first] = modmap;

		}
	}

	return dependencies;

}

int main(void) {
	cout << "--- Firestarter initialising. ---" << endl;

	set_logfile_name("main");

	libconfig::Config	config;
	ModuleMap			modules;

	try {
		LOG_INFO(mainLog, "Loading the configuration...");
		config.readFile("/etc/firestarter/fs.cfg");
		LOG_INFO(mainLog, "done");

		LOG_INFO(mainLog, "Modules that will be loaded:");
		{
			bool end = false;
			int i = 0;
			libconfig::Setting & conf_modules = config.lookup("application.modules");

			while (!end) {
				try {
					LOG_DEBUG(mainLog, "Retrieving conf_modules[" << i << "]");
					string module_name = conf_modules[i++];

					string path = "firestarter/" + module_name + ".so";
					boost::to_lower(path);

					LOG_DEBUG(mainLog, "Storing modulePaths[" << module_name << "] = " << path);
					modules[module_name] = ModuleTuple(path, 0, NULL, static_cast<create_module *>(NULL), static_cast<destroy_module *>(NULL));
//					modules[module_name] = boost::make_tuple(path, 0, NULL, static_cast<create_module *>(NULL), static_cast<destroy_module *>(NULL));
/*					modulePaths[module_name] = path;*/
					LOG_INFO(mainLog, "\t- " << module_name);
				}
				catch (libconfig::SettingNotFoundException e) {
					end = true;
					LOG_DEBUG(mainLog, "Reached end of application.modules list (Caught libconfig::SettingNotFoundException)");
				}
			}
		}
	}
	catch (libconfig::SettingTypeException e) {
		LOG_ERROR(mainLog, "Caught SettingTypeException");
	}

	catch (libconfig::SettingNameException e) {
		LOG_ERROR(mainLog, "Caught SettingNameException");
	}

	catch (libconfig::ParseException e) {
		LOG_ERROR(mainLog, "Caught ParseException");
	}

	catch (libconfig::FileIOException e) {
		LOG_ERROR(mainLog, "Caught FileIOException");
	}

	catch (...) {
		LOG_ERROR(mainLog, "Failed.");
	}

	LOG_DEBUG(mainLog, "Going over the modules list");

	list<string> invalidModules;

	loadModules(modules, invalidModules);

	if (invalidModules.size() != 0) {
		LOG_WARN(mainLog, "Invalid modules have been detected.");
		foreach(string name, invalidModules) {
			LOG_WARN(mainLog, "Removing " << name << " from list.");
			modules.erase(name);
		}
		invalidModules.clear();
	}
	ModuleDependencyMap dependencies;
	ModuleDependencyMap * d = NULL;

	d = run_beforeFilter(modules, invalidModules);

	/*while (d != NULL && d->size() > 0) {
		ModuleDependencyMap * tmp = d;
		dependencies += (*d);
		foreach(ModuleMap::value_type i, tmp->second) {
			
		}
	}
	do {
		if (d != NULL) {
			dependencies += (*d);
		}
		d = run_beforeFilter(modules, invalidModules);
		if (d == NULL)
			break;
	} while (dependencies.size() < d->size());*/

	LOG_INFO(mainLog, "Firestarter exit.");
}
