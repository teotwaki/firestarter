#include "wtserver.h"

DECLARE_MODULE_LOG(WtServer);

using namespace firestarter::module::core::WtServer;

WtServer::WtServer(zmq::context_t & context) : RunnableModule(context), server("firestarter") {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "WtServer object being created.");
}

void WtServer::run() {
	using namespace firestarter::protocol::module;
	boost::posix_time::microseconds delay(1000000);

	LOG_INFO(logger, "Running the WtServer's main function.");

	while (this->running || this->runlevel == NONE) {
		RunlevelRequest order;
		
		if (this->runlevel == NONE) {
			LOG_INFO(logger, "Thread is in NONE state. Waiting for orders.");

			if (this->manager_socket.receive(order, true) && order.type() == UPDATE) {
				LOG_DEBUG(logger, "Manager wants us to start up. Buckle up boys.");
			}

			else {
				LOG_ERROR(logger, order.DebugString());
				LOG_ERROR(logger, order.InitializationErrorString());
				if (order.IsInitialized()) {
					LOG_ERROR(logger, "Weird, the message is initialised!");
				}
				LOG_WARN(logger, "Couldn't parse message received from manager. Shutting down.");
				return;
			}

			switch(order.runlevel()) {
				case INIT: {
					this->setup();
					LOG_DEBUG(logger, "Creating response message");
					RunlevelResponse response;
					response.set_runlevel(order.runlevel());
					response.set_result(SUCCESS);
					this->manager_socket.send(response);
					break;
				}

				default:
					LOG_WARN(logger, "Unexpected runlevel change request received. Shutting down.");
					return;
					break;
			}
		}

		else {
			LOG_ERROR(logger, "Cool, the wtserver is now running. Let's pretend to be doing something.");
			boost::this_thread::sleep(delay);
		}
		
	}
}

void WtServer::setup() {
	LOG_INFO(logger, "WtServer being set up.");
	try {
		LOG_DEBUG(logger, "Attempting to set the web server's configuration.");
		this->server.setServerConfiguration(0, NULL, SYSCONFDIR "/httpd.cfg");
		LOG_DEBUG(logger, "Starting httpd thread.");
		this->server.start();
		LOG_DEBUG(logger, "Setting this thread's state.");
		this->running = true;
		this->runlevel = firestarter::protocol::module::INIT;
	}

	catch (Wt::WServer::Exception & wex) {
		LOG_ERROR(logger, "Exception raised while starting the server:");
		LOG_ERROR(logger, wex.what());
	}
	catch (std::exception & sex) {
		LOG_ERROR(logger, "Exception raised while starting the server:");
		LOG_ERROR(logger, sex.what());
	}
}

void WtServer::shutdown() {
	LOG_INFO(logger, "WtServer shutting down.");
	try {
		this->server.stop();
	}

	catch (Wt::WServer::Exception & wex) {
		LOG_ERROR(logger, "Exception raised while shutting down the server:");
		LOG_ERROR(logger, wex.what());
	}
	catch (std::exception & sex) {
		LOG_ERROR(logger, "Exception raised while shutting down the server:");
		LOG_ERROR(logger, sex.what());
	}
}
