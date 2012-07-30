
#ifndef __BLANKPAGE_H
#define __BLANKPAGE_H

#include "log.h"
#include "webwidgets/basepage.h"
#include "webwidgets/htmltemplates.h"

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
