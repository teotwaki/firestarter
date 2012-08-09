#include "persistance.hpp"

DECLARE_MODULE_LOG(Persistance);

using namespace firestarter::module::examples::Persistance;

Persistance::Persistance(zmq::context_t & context) : RunnableModule(context) {
	using namespace firestarter::common;
	LOG_INFO(logger, "Persistance object being created.");
	Person p;
	p.first_name = "Roger LaBite";
	Persistent persist;

	persist.store(p);
}

void Persistance::run() {

}

void Persistance::setup() {

}

void Persistance::shutdown() {

}
