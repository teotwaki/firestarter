#ifndef __INSTANCEMANAGER_H
#define __INSTANCEMANAGER_H

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include "helper.h"
#include "modulemanager.h"
#include "zmqhelper.h"
#include "module.h"

#include <libconfig.h++>
#include <list>
#include <boost/thread.hpp>

namespace firestarter {
	namespace InstanceManager {

typedef boost::unordered_map<std::string, firestarter::module::Module *> InstanceMap;
typedef boost::unordered_map<std::string, std::pair<boost::thread *, firestarter::module::RunnableModule *> > ThreadMap;

/** \brief Manages the running instances of modules
  *
  */
class InstanceManager {
	private:
	firestarter::ModuleManager::ModuleManager & modulemanager;
	InstanceMap instances;
	ThreadMap threads;
	zmq::context_t & context;
	zmq::socket_t modules;

	public:
	InstanceManager(firestarter::ModuleManager::ModuleManager & modulemanager, zmq::context_t & context) throw(std::invalid_argument); 
	void run(const std::string & name, bool autostart = false) throw(firestarter::exception::ModuleNotFoundException);
	void runAll(bool autostart = false);
	void stop(const std::string & name) throw(firestarter::exception::ModuleNotFoundException);
	void stopAll();
	void poll();

};

/* Closing the namespace */
	}
}

#endif
