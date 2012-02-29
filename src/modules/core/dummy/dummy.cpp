#include "dummy.h"

DECLARE_MODULE_LOG(Dummy);

using namespace firestarter::module::core::Dummy;

Dummy::Dummy(zmq::context_t & context) : Module() {
	set_logfile_name("Dummy");
	LOG_DEBUG(logger, "Dummy being created.");
}

void Dummy::setup() {
	LOG_INFO(logger, "Dummy being set up.");
}
