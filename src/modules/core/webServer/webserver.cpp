#include "webserver.h"

DECLARE_MODULE_LOG(WebServer);

using namespace firestarter::module::core::WebServer;

WebServer::WebServer(zmq::context_t & context) : RunnableModule(context), server("firestarter") {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "WebServer object being created.");
}

void WebServer::run() {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "Running the WebServer's main function.");

	while (this->running || this->runlevel == NONE) {
		zmq::message_t message;
		
		if (this->runlevel == NONE) {
			LOG_INFO(logger, "Thread is in NONE state. Waiting for orders.");

			this->manager->recv(&message);
			RunlevelChangeRequest request;

			LOG_DEBUG(logger, "Received message from manager.");
			LOG_DEBUG(logger, "Message size: " << message.size());
			if (request.ParseFromArray(message.data(), message.size())) {
				LOG_DEBUG(logger, "Manager wants us to start up. Buckle up boys.");
			}

			else {
				LOG_WARN(logger, "Couldn't parse message received from manager.");
				return;
			}

			switch(request.runlevel()) {
				case INIT:
					this->setup();
					break;

				default:
					LOG_WARN(logger, "Unexpected runlevel change request received. Shutting down.");
					return;
					break;
			}
		}

		else {
			LOG_ERROR(logger, "Not implemented yet.");
			return;
		}
		
	}
}

void WebServer::setup() {
	LOG_INFO(logger, "WebServer being set up.");
	try {
		LOG_DEBUG(logger, "Attempting to set the web server's configuration.");
		this->server.setServerConfiguration(0, NULL, "/etc/firestarter/httpd.cfg");
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

void WebServer::shutdown() {
	LOG_INFO(logger, "WebServer shutting down.");
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
