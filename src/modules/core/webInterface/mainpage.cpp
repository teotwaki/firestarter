/*
 * Copyright (C) 2012  Sebastian Lauwers <sebastian.lauwers@gmail.com>
 *
 * This file is part of Firestarter.
 *
 * Firestarter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firestarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainpage.hpp"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

bool AdminPage::response() {
	using namespace firestarter::common::WebWidgets::Templates::Tags;

	LOG_DEBUG(logger, "AdminPage::response() called.")

//	this->sessions.cleanup();
//	this->session = sessions.find(environment.findCookie("SESSIONID").data());

	this->html.attributes.setLang("en");
	this->html.attributes.setXmllang("en");

	auto & head = this->html.addChild<Head>();

	head.addChild<Link>()
			.setHref("http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/themes/pepper-grinder/jquery-ui.css")
			.setRel("stylesheet")
			.setType("text/css");

	head.addChild<Title>().setContents("Firestarter starts fires.");

	head.addChild<Script>()
			.setSrc("http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js")
			.setType("text/javascript");

	head.addChild<Script>()
			.setSrc("http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js")
			.setType("text/javascript");

	head.addChild<Script>()
			.setType("text/javascript")
			.setContents("$(document).ready(function() {\n"
				"	$('#loginbox').position({\n"
				"		my: 'center',\n"
				"		at: 'center',\n"
				"		of: $(window)\n"
				"	});\n"
				"	$('#submit').position({\n"
				"		my: 'center',\n"
				"		at: 'center',\n"
				"		of: '#loginbox'\n"
				"	});\n"
				"	$('#submit').button();\n"
				"});\n");

	head.addChild<Style>()
			.setContents("label { display: block; margin: 15px 0 5px; }\n");

	auto & body = this->html.addChild<Body>();
	body.attributes.setStyle("font-size: 62.5%; min-height: 90%;");

	auto & loginbox = body.addChild<Div>();
	loginbox.attributes.setId("loginbox");
	loginbox.attributes.setStyle("width: 399px; height: 399px;");

	auto & form = loginbox.addChild<Form>()
			.setMethod("post");

	form.addChild<Label>()
			.setFor("username")
			.setContents("Username:");
	form.addChild<Input>()
			.setType("text")
			.setName("username")
			.attributes.setId("username");

	form.addChild<Br>();

	form.addChild<Label>()
			.setFor("username")
			.setContents("Password:");
	form.addChild<Input>()
			.setType("password")
			.setName("password")
			.attributes.setId("password");

	form.addChild<Br>();

	form.addChild<Input>()
			.setType("submit")
			.setValue("Submit")
			.attributes.setId("submit");
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

