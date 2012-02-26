#include "webserver.h"

DECLARE_MODULE_LOG(WebServer);

using namespace firestarter::module::core::WebServer;

WebServer::WebServer(zmq::context_t & context) : RunnableModule(context), server("firestarter") {
	using namespace firestarter::protocol::module;

	LOG_INFO(logger, "WebServer object being created.");
}

void WebServer::run() {
	using namespace firestarter::protocol::module;
	boost::posix_time::microseconds delay(1000000);

	LOG_INFO(logger, "Running the WebServer's main function.");

	while (this->running || this->runlevel == NONE) {
		zmq::message_t message;
		
		if (this->runlevel == NONE) {
			LOG_INFO(logger, "Thread is in NONE state. Waiting for orders.");

			this->orders->recv(&message);
			RunlevelChangeRequest order;

			LOG_DEBUG(logger, "Received message from manager (" << message.size() << " bytes).");
			if (order.ParseFromArray(message.data(), message.size())) {
				LOG_DEBUG(logger, "Manager wants us to start up. Buckle up boys.");
			}

			else {
				LOG_WARN(logger, "Couldn't parse message received from manager. Shutting down.");
				return;
			}

			switch(order.runlevel()) {
				case INIT: {
					this->setup();
					LOG_DEBUG(logger, "Creating response message");
					RunlevelChangeResponse response;
					response.set_runlevel(INIT);
					response.set_result(SUCCESS);
					this->send(response, this->manager);
					LOG_DEBUG(logger, "Listening for empty reponse");
					zmq::message_t empty;
					// Listen for empty response
					this->manager->recv(&empty);
					break;
				}

				default:
					LOG_WARN(logger, "Unexpected runlevel change request received. Shutting down.");
					return;
					break;
			}
		}

		else {
			LOG_ERROR(logger, "Cool, the webserver is now running. Let's pretend to be doing something.");
			boost::this_thread::sleep(delay);
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
