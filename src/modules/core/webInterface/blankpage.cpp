#include "blankpage.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface;

bool BlankPage::response() {
	using namespace firestarter::common::WebWidgets::Templates::Tags;

	LOG_DEBUG(logger, "BlankPage::response() called.")

	this->html.attributes.setLang("en");
	this->html.attributes.setXmllang("en");

	auto & head = this->html.addChild<Head>();

	head.addChild<Meta>()
			.setName("description")
			.setContent("Firestarter demo page");

	head.addChild<Script>()
			.setSrc("https://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js")
			.setType("text/javascript");

	head.addChild<Comment>()
			.setCondition("lt IE 9")
			.addChild<Script>()
				.setSrc("http://html5shiv.googlecode.com/svn/trunk/html5.js");

	// Prettify JS
	head.addChild<Script>()
			.setSrc("js/prettify.js")
			.setType("text/javascript");
			
	// Kickstart JS
	head.addChild<Script>()
			.setSrc("js/kickstart.js")
			.setType("text/javascript");

	// Kickstart CSS
	head.addChild<Link>()
			.setHref("css/kickstart.css")
			.setMedia("all")
			.setType("text/css")
			.setRel("stylesheet");

	// Custom style
	head.addChild<Link>()
			.setHref("style.css")
			.setMedia("all")
			.setType("text/css")
			.setRel("stylesheet");
			
	auto & body = this->html.addChild<Body>();
	body.addChild<A>()
			.attributes.setId("top-of-page");

	auto & wrap = body.addChild<Div>();
	wrap.attributes.setId("wrap");
	wrap.attributes.setClass("clearfix");

	auto & contents = wrap.addChild<Div>();
	contents.attributes.setClass("col_12");

	auto & center = contents.addChild<H1>();
	center.attributes.setClass("center");

	auto & icon = center.addChild<Span>();
	icon.attributes.setClass("icon");
	icon.attributes.setStyle("font-size: 400px; text-shadow: 0px 3px 2px rgba(0,0,0,0.3); color: #efefef;");
	icon.setExtraAttribute("data-icon", "F");

	auto & text = contents.addChild<H3>();
	text.attributes.setStyle("color: #ccc; margin-bottom: 40px;");
	text.attributes.setClass("center");
	text.setContents("firestarter\n");

	return true;
}

