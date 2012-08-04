#include "log.hpp"

void set_logfile_name(std::string name) {
	setenv("logname", name.c_str(), 1);
	log4cxx::PropertyConfigurator::configure(SYSCONFDIR "/logging.cfg");
}
