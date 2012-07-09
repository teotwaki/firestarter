#include "mainpage.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

bool AdminPage::response() {
	using namespace firestarter::common::WebWidgets::Templates;

	LOG_DEBUG(logger, "AdminPage::response() called.")

//	this->sessions.cleanup();
//	this->session = sessions.find(environment.findCookie("SESSIONID").data());

//	LOG_DEBUG(logger, "Building tags.");
//	Tags::Head head;
//	Tags::Title title;
//	Tags::Body body;
//	Tags::P paragraph;

	LOG_DEBUG(logger, "Setting language.");
	this->html.attributes.setLang("en");
	this->html.attributes.setXmllang("en");

//	LOG_DEBUG(logger, "Setting contents.");
//	title.setContents("Firestarter starts fires.");
//	paragraph.setContents("This is our first paragraph.");

	LOG_DEBUG(logger, "Building tag hierarchy.");
//	this->html.addChild(&head);
//	head.addChild(&title);
//	this->html.addChild(&body);
//	body.addChild(&paragraph);

	LOG_DEBUG(logger, "Done.");

/*
	std::wstring command = environment().findGet(L"command");

	this->setloc(std::locale(getloc(), new boost::posix_time::time_facet("%a, %d-%b-%Y %H:%M:%S GMT")));

	if (this->session != sessions.end()) {
		if (command == L"logout") {

			this->out << "Set-Cookie: SESSIONID=deleted; expires=Thu, 01-Jan-1970 00:00:00 GMT;\n";
            this->sessions.erase(session);
            this->session = this->sessions.end();
            this->handleNoSession();
		}
		else {
			this->session->first.refresh();
			this->out << "Set-Cookie: SESSIONID=" << encoding(URL) << this->session->first << encoding(NONE) << 
				"; expires=" << this->sessions.getExpiry(this->session) << '\n';
			this->handleSession();
		}
	}
	else {
		if (command == L"login") {
			this->session = this->sessions.generate(environment().findPost(L"data").value);
			this->out << "Set-Cookie: SESSIONID=" << encoding(URL) << this->session->first << encoding(NONE) <<
				"; expires=" << this->sessions.getExpiry(this->session) << '\n';
			this->handleSession();
		}
		else {
			this->handleNoSession();
		}
	}

	this->out << "<p>There are " << this->sessions.size() << " sessions open</p>\n"
		"	</body>\n"
		"</html>";
*/
	return true;
}

