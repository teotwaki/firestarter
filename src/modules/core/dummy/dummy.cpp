#include "dummy.h"

using namespace firestarter::module::core::Dummy;

Dummy::Dummy() {
	set_logfile_name("Dummy");
	LOG_DEBUG(dummyLog, "Dummy being created.");
}

void Dummy::beforeFilter() {
	LOG_DEBUG(dummyLog, "Dummy::beforeFilter() called.");
	addComponent("Foo", 1);
	addComponent("Bar", 1);
	addBehaviour("Slimy", 2);
}

void Dummy::setup() {
	LOG_INFO(dummyLog, "Dummy being set up.");
}

void Dummy::afterFilter() {
	LOG_DEBUG(dummyLog, "Dummy::afterFilter() called.");
}
