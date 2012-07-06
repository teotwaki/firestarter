#include "mainpage.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

AdminPage::Sessions AdminPage::sessions(3600, 3600);

bool AdminPage::response() {
	using namespace Fastcgipp;

	sessions.cleanup();
	this->session = sessions.find(environment().findCookie("SESSIONID").data());

	std::string command = environment().findGet("command");

	this->setloc(std::locale(getloc(), new boost::posix_time::time_facet("%a, %d-%b-%Y %H:%M:%S GMT")));

	if (this->session != sessions.end()) {
		if (command == "logout") {

			this->out << "Set-Cookie: SESSIONID=deleted; expires=Thu, 01-Jan-1970 00:00:00 GMT;\n";
            sessions.erase(session);
            this->session = sessions.end();
            this->handleNoSession();
		}
		else {
			this->session->first.refresh();
			this->out << "Set-Cookie: SESSIONID=" << encoding(URL) << this->session->first << encoding(NONE) << 
				"; expires=" << sessions.getExpiry(session) << '\n';
			this->handleSession();
		}
	}
	else {
		if (command == "login") {
			this->session = sessions.generate(environment().findPost("data").value);
			this->out << "Set-Cookie: SESSIONID=" << encoding(URL) << this->session->first << encoding(NONE) <<
				"; expires=" << sessions.getExpiry(session) << '\n';
			this->handleSession();
		}
		else {
			this->handleNoSession();
		}
	}

	this->out << "<p>There are " << sessions.size() << "sessions open</p>\n"
		"	</body>\n"
		"</html>";

	return true;
}

void AdminPage::handleSession() {
	using namespace Fastcgipp;

	this->out << "Content-Type: text/html; charset=utf-8\r\n\r\n"
		"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' "
		" 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n"
		"<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>\n"
		"	<head>\n"
		"		<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />\n"
		"		<title>fastcgi++: Session Handling example</title>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p>We are currently in a session. The session id is " << this->session->first << " and the session data"
		" is \"" << encoding(HTML) << this->session->second << encoding(NONE) << "\". It will expire at "
		<< sessions.getExpiry(session) << ".</p>\n"
		"		<p>Click <a href='?command=logout'>here</a> to logout</p>\n";
}

void AdminPage::handleNoSession() {
	using namespace Fastcgipp;

	this->out << "Content-Type: text/html; charset=utf-8\r\n\r\n"
		"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' "
		" 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n"
		"<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>\n"
		"	<head>\n"
		"		<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />\n"
		"		<title>fastcgi++: Session Handling example</title>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p>We are currently not in a session.</p>\n"
		"		<form action='?command=login' method='post' enctype='application/x-www-form-urlencoded' "
		"accept-charset='ISO-8859-1'>\n"
		"			<div>\n"
		"				Text: <input type='text' name='data' value='Hola señor, usted me almacenó en una sesión' />\n"
		"				<input type='submit' name='submit' value='submit' />\n"
		"			</div>\n"
		"		</form>\n";

}
