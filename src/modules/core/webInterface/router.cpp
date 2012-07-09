#include "router.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

std::map<std::string, Router::PageFactory> Router::page_factory;

bool Router::response() {
	using namespace firestarter::common::WebWidgets::Pages;

	WebPage * page = this->page_factory["main"]();
	LOG_DEBUG(logger, "Page contents: " << this->page_factory["main"]()->render());
	this->out << this->page_factory["main"]()->render();

	return true;
}
