
#ifndef FIRESTARTER_BLANKPAGE_HPP
#define FIRESTARTER_BLANKPAGE_HPP

#include "log.hpp"
#include "webwidgets/basepage.hpp"
#include "webwidgets/htmltemplates.hpp"

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {

	class BlankPage : public firestarter::common::WebWidgets::Pages::WebPage {
		bool response();
	};

/* Close namespaces */
			}
		}
	}
}

#endif
