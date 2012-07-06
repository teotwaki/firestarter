
#ifndef __HTMLTEMPLATES_H
#define __HTMLTEMPLATES_H

#include "log.h"

#include <ctemplate/template.h>
#include <string>

namespace firestarter {
	namespace module {
		namespace core {
			namespace WebInterface {
				namespace Templates {

	class Template {
		protected:
		std::string out;
		ctemplate::TemplateDictionary dict;
		virtual void expand() = 0;
		Template(std::string dict_name) : dict(dict_name) { };

		public:
		std::wstring & render();
	};

	class FileTemplate : public Template {
		protected:
		std::string tpl_path;
		void expand();
		FileTemplate(std::string dict_name) : Template(dict_name) { };

		public:
		inline void setTemplatePath(std::string path) { this->tpl_path = path; };
	};

	class StringTemplate : public Template {
		protected:
		std::string cache_name;
		std::string tpl_contents;
		void expand();
		StringTemplate(std::string dict_name) : Template(dict_name) { };

		public:
		inline void setTemplateContents(std::string cache_name, std::string tpl_contents) {
			this->cache_name = cache_name;
			this->tpl_contents = tpl_contents;
		}
	};

	class Headers : public StringTemplate {
		public:
		Headers(std::string content = "text/html", std::string encoding = "utf-8") : StringTemplate("Headers") {
			this->setTemplateContents("headers", "Content-Type: {{CONTENT_TYPE}}; charset={{ENCODING}}\r\n\r\n");
			this->dict.SetValue("CONTENT_TYPE", content);
			this->dict.SetValue("ENCODING", encoding);
		};
		inline void setContent(std::string & content) { this->dict.SetValue("CONTENT_TYPE", content); };
		inline void setEncoding(std::string & encoding) { this->dict.SetValue("ENCODING", encoding); };
	};

	class DocType : public StringTemplate {
		public:
		DocType() : StringTemplate("DocType") {
			this->setTemplateContents("doctype", "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN'"
				" 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n");
		};
	};

	class CoreTag : public StringTemplate {
		public:
		CoreTag(std::string style = "", std::string _class = "", std::string id = "", std::string title = "")
				: StringTemplate("CoreTag") {
			this->setTemplateContents("coretag", "{{#STYLE_S}}style=\"{{STYLE}}\"{{/STYLE_S}} "
				"{{#CLASS_S}}class=\"{{CLASS}}\"{{/CLASS_S}} "
				"{{#ID_S}}id=\"{{ID}}\"{{/ID_S}} "
				"{{#TITLE_S}}title=\"{{TITLE}}\"{{/TITLE_S}} ");
			this->setStyle(style);
			this->setClass(_class);
			this->setId(id);
			this->setTitle(title);
		};

		inline void setStyle(std::string style) {
			if (not style.empty()) this->dict.SetValueAndShowSection("STYLE", style, "STYLE_S");
		};

		inline void setClass(std::string _class) {
			if (not _class.empty()) this->dict.SetValueAndShowSection("CLASS", _class, "CLASS_S");
		};

		inline void setId(std::string id) {
			if (not id.empty()) this->dict.SetValueAndShowSection("ID", id, "ID_S");
		};

		inline void setTitle(std::string title) {
			if (not title.empty()) this->dict.SetValueAndShowSection("TITLE", title, "TITLE_S");

		};
	};

	class LangTag : public StringTemplate {
		public:
		LangTag(std::string dir = "", std::string lang = "", std::string xmllang = "") : StringTemplate("LangTag") {
			this->setTemplateContents("langtag", "{{#DIR_S}}dir=\"{{DIR}}\"{{/DIR_S}} "
				"{{#LANG_S}}lang=\"{{LANG}}\"{{/LANG_S}} "
				"{{#XMLLANG_S}}xml:lang=\"{{XMLLANG}}\"{{/XMLLANG_S}} ");
			this->setDir(dir);
			this->setLang(lang);
			this->setXmllang(xmllang);
		};

		inline void setDir(std::string dir) {
			if (not dir.empty()) this->dict.SetValueAndShowSection("DIR", dir, "DIR_S");
		};

		inline void setLang(std::string lang) {
			if (not lang.empty()) this->dict.SetValueAndShowSection("LANG", lang, "LANG_S");

		};

		inline void setXmllang(std::string xmllang) {
			if (not xmllang.empty()) this->dict.SetValueAndShowSection("XMLLANG", xmllang, "XMLLANG_S");
		};
	};

	namespace Tags {

		class Html : public StringTemplate {
			public:
			Html(std::string contents = "", std::string lang = "en",
					std::string xmlns = "http://www.w3.org/1999/xhtml")
					: StringTemplate("Html") {
				this->setTemplateContents("html",
					"<html xmlns='{{XMLNS}}' xml:lang='{{LANG}' lang='{{LANG}}'>\n"
					"{{CONTENTS}}\n"
					"</html>\n");
				this->dict.SetValue("LANG", lang);
				this->dict.SetValue("XMLNS", xmlns);
				this->dict.SetValue("CONTENTS", contents);
			}
		};

		class Head : public StringTemplate {
			public:
			Head(std::string contents = "") : StringTemplate("Head") {
				this->setTemplateContents("head", "	<head>\n"
					"{{CONTENTS}}\n"
					"	</head>\n");
				this->dict.SetValue("CONTENTS", contents);
			}
		};

	}

/* Close namespaces */
				}
			}
		}
	}
}

#endif
