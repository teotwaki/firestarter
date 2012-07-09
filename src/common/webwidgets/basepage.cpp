#include "webwidgets/basepage.h"

namespace firestarter { namespace common { namespace WebWidgets { namespace Pages {
	DECLARE_LOG(logger, "firestarter.common.WebWidgets.Pages");
} } } }

using namespace firestarter::common::WebWidgets::Pages;

WebPage::Sessions WebPage::sessions(3600, 3600);

std::string WebPage::render() {
	LOG_DEBUG(logger, "WebPage::render() called.");
	LOG_DEBUG(logger, "Calling this->response().");
	this->response();
	LOG_DEBUG(logger, "Page built, rendering output.");
	return this->headers.render() + this->html.render();
}
