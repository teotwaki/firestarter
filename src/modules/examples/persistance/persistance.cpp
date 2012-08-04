#include "persistance.hpp"

DECLARE_MODULE_LOG(Persistance);

using namespace firestarter::module::examples::Persistance;

Persistance::Persistance(zmq::context_t & context) : RunnableModule(context) {
	LOG_INFO(logger, "Persistance object being created.");
}

void Persistance::run() {

}

void Persistance::setup() {

}

void Persistance::shutdown() {

}
