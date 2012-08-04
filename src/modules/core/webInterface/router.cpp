#include "router.hpp"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

std::map<std::string, Router::PageFactory> Router::page_factory;

bool Router::response() {
	using namespace firestarter::common::WebWidgets::Pages;

	auto page = this->instantiate("blank");
	std::string contents = page->render();
	LOG_DEBUG(logger, "Page contents: " << contents);
	this->out << contents;

	return true;
}
