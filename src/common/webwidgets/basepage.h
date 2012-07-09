
#ifndef __BASEPAGE_H
#define __BASEPAGE_H

#include "htmltemplates.h"

#include <string>
#include <fastcgi++/http.hpp>

namespace firestarter {
	namespace common {
		namespace WebWidgets {
			namespace Pages {

	class WebPage {
		protected:
		typedef Fastcgipp::Http::Sessions<std::string> Sessions;
		static Sessions sessions;
		Sessions::iterator session;
		firestarter::common::WebWidgets::Templates::Headers headers;
		firestarter::common::WebWidgets::Templates::Tags::Html html;
		virtual bool response(/*const Fastcgipp::Http::Environment<char> & environment*/) = 0;

		public:
		std::string render();
	};

/* Close namespaces */
			}
		}
	}
}

#endif
