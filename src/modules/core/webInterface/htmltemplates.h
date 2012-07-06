
#ifndef __HTMLTEMPLATES_H
#define __HTMLTEMPLATES_H

#include "log.h"

#include <ctemplate/template.h>
#include <string>
#include <list>

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
		std::string render() {
			this->expand();
			return this->out;
		};
	};

	class FileTemplate : public Template {
		protected:
		std::string tpl_path;
		virtual void expand() { ExpandTemplate(this->tpl_path, ctemplate::DO_NOT_STRIP, &(this->dict), &(this->out)); };
		FileTemplate(std::string dict_name) : Template(dict_name) { };

		public:
		inline void setTemplatePath(std::string path) { this->tpl_path = path; };
	};

	class StringTemplate : public Template {
		protected:
		std::string cache_name;
		virtual void expand() {
			ExpandTemplate(this->cache_name, ctemplate::DO_NOT_STRIP, &(this->dict), &(this->out));
		};
		StringTemplate(std::string dict_name) : Template(dict_name) { };

		public:
		void setTemplateContents(std::string cache_name, std::string tpl_contents) {
			this->cache_name = cache_name;
			StringToTemplateCache(cache_name, tpl_contents, ctemplate::DO_NOT_STRIP);
		}
	};

	class Headers : public StringTemplate {
		public:
		Headers(std::string content = "text/html", std::string encoding = "utf-8") : StringTemplate("Headers") {
			this->setTemplateContents("headers", "Content-Type: {{CONTENT_TYPE}}; charset={{ENCODING}}\r\n\r\n");
			this->setContent(content);
			this->setEncoding(encoding);
		};
		inline void setContent(std::string & content) { this->dict["CONTENT_TYPE"] = content; };
		inline void setEncoding(std::string & encoding) { this->dict["ENCODING"] = encoding; };
	};

	class DocType : public StringTemplate {
		public:
		DocType() : StringTemplate("DocType") {
			this->setTemplateContents("doctype", "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN'"
				" 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n");
		};
	};

	class CoreTag : virtual public StringTemplate {
		protected:
		ctemplate::TemplateDictionary * core_dict;

		public:
		CoreTag(std::string style = "", std::string _class = "", std::string id = "", std::string title = "")
				: StringTemplate("CoreTag") {
			this->core_dict = this->dict.AddIncludeDictionary("CORETAG");
			this->core_dict->SetFilename("CORETAG");
			this->setTemplateContents("CORETAG", "{{#STYLE_S}} style=\"{{STYLE}}\"{{/STYLE_S}}"
				"{{#CLASS_S}} class=\"{{CLASS}}\"{{/CLASS_S}}"
				"{{#ID_S}} id=\"{{ID}}\"{{/ID_S}}"
				"{{#TITLE_S}} title=\"{{TITLE}}\"{{/TITLE_S}}");
			this->setStyle(style);
			this->setClass(_class);
			this->setId(id);
			this->setTitle(title);
		};

		inline void setStyle(std::string style) {
			if (not style.empty()) this->core_dict->SetValueAndShowSection("STYLE", style, "STYLE_S");
		};

		inline void setClass(std::string _class) {
			if (not _class.empty()) this->core_dict->SetValueAndShowSection("CLASS", _class, "CLASS_S");
		};

		inline void setId(std::string id) {
			if (not id.empty()) this->core_dict->SetValueAndShowSection("ID", id, "ID_S");
		};

		inline void setTitle(std::string title) {
			if (not title.empty()) this->core_dict->SetValueAndShowSection("TITLE", title, "TITLE_S");

		};
	};

	class LangTag : virtual public StringTemplate {
		protected:
		ctemplate::TemplateDictionary * lang_dict;

		public:
		LangTag(std::string dir = "", std::string lang = "", std::string xmllang = "") : StringTemplate("LangTag") {
			this->lang_dict = this->dict.AddIncludeDictionary("LANGTAG");
			this->lang_dict->SetFilename("LANGTAG");
			this->setTemplateContents("LANGTAG", "{{#DIR_S}} dir=\"{{DIR}}\"{{/DIR_S}}"
				"{{#LANG_S}} lang=\"{{LANG}}\"{{/LANG_S}}"
				"{{#XMLLANG_S}} xml:lang=\"{{XMLLANG}}\"{{/XMLLANG_S}}");
			this->setDir(dir);
			this->setLang(lang);
			this->setXmllang(xmllang);
		};

		inline void setDir(std::string dir) {
			if (not dir.empty() && (dir == "rtl" || dir == "ltr"))
				this->lang_dict->SetValueAndShowSection("DIR", dir, "DIR_S");
		};

		inline void setLang(std::string lang) {
			if (not lang.empty()) this->lang_dict->SetValueAndShowSection("LANG", lang, "LANG_S");

		};

		inline void setXmllang(std::string xmllang) {
			if (not xmllang.empty()) this->lang_dict->SetValueAndShowSection("XMLLANG", xmllang, "XMLLANG_S");
		};
	};

	class ParentTag : virtual public StringTemplate {
		protected:
		void expand() {
			std::string children_content;
			for (std::list<Template *>::iterator it = this->children.begin(); it != this->children.end(); it++)
				children_content += (*it)->render();
			this->setContents(children_content);
			StringTemplate::expand();
		 };
		ParentTag() : StringTemplate("ParentTag") { };

		public:
		std::list<Template *> children;
		inline void setContents(std::string contents) {
			if (not contents.empty()) this->dict["CONTENTS"] = contents;
		};
		inline void addChild(Template * child) { if (child != NULL) this->children.push_back(child); };
		inline void addChildren(std::list<Template *> children) {
			for (std::list<Template *>::iterator it = children.begin(); it != children.end(); it++)
				if (*it != NULL) this->children.push_back(*it);
		};
	};

	namespace Tags {

		class Html : public LangTag, public ParentTag {
			public:
			Html(std::string xmlns = "http://www.w3.org/1999/xhtml") : StringTemplate("Html") {
				this->setTemplateContents("html",
					"<html xmlns='{{XMLNS}}'{{>LANGTAG}}>\n"
					"{{CONTENTS}}"
					"</html>\n");
				this->dict["XMLNS"] = xmlns;
			}
		};

		class Head : public LangTag, public ParentTag {
			public:
			Head() : StringTemplate("Head") {
				this->setTemplateContents("head", "	<head>\n"
					"{{CONTENTS}}"
					"	</head>\n");
			}
		};

		class Meta : public LangTag {
			public:
			Meta(std::string httpequiv = "", std::string content = "", std::string name = "", std::string scheme = "") 
					: StringTemplate("Meta") {
				this->setTemplateContents("meta",
					"<meta{{>LANGTAG}}{{#HTTPEQUIV_S}} http-equiv=\"{{HTTPEQUIV}}\"{{/HTTPEQUIV_S}}"
					"{{#CONTENT_S}} content=\"{{CONTENT}}\"{{/CONTENT_S}}"
					"{{#NAME_S}} name=\"{{NAME}}\"{{/NAME_S}}"
					"{{#SCHEME_S}} scheme=\"{{SCHEME}}\"{{/SCHEME_S}}/>\n");
				this->setHttpequiv(httpequiv);
				this->setContent(content);
				this->setName(name);
				this->setScheme(scheme);
			};

			inline void setHttpequiv(std::string httpequiv) {
				if (not httpequiv.empty()) this->dict.SetValueAndShowSection("HTTPEQUIV", httpequiv, "HTTPEQUIV_S");
			};

			inline void setContent(std::string content) {
				if (not content.empty()) this->dict.SetValueAndShowSection("CONTENT", content, "CONTENT_S");
			};

			inline void setName(std::string name) {
				if (not name.empty()) this->dict.SetValueAndShowSection("NAME", name, "NAME_S");
			};

			inline void setScheme(std::string scheme) {
				if (not scheme.empty()) this->dict.SetValueAndShowSection("SCHEME", scheme, "SCHEME_S");
			};
		};

		class Title : public LangTag, public ParentTag {
			public:
			Title() : StringTemplate("Title") {
				this->setTemplateContents("title", "		<title{{>LANGTAG}}>{{CONTENTS}}</title>\n");
			};
		};

		class Body : public ParentTag, public LangTag, public CoreTag {
			public:
			Body() : StringTemplate("Body") {
				this->setTemplateContents("body", "	<body{{>LANGTAG}}{{>CORETAG}}>\n"
					"	{{CONTENTS}}"
					"	</body>\n");
			};
		};

		class P : public ParentTag, public LangTag, public CoreTag {
			public:
			P() : StringTemplate("P") {
				this->setTemplateContents("p", "<p{{>LANGTAG}}{{>CORETAG}}>{{CONTENTS}}</p>\n");
			};
		};

		class Div : public ParentTag, public LangTag, public CoreTag {
			public:
			Div() : StringTemplate("Div") {
				this->setTemplateContents("div", "		<div{{>LANGTAG}}{{>CORETAG}}>\n"
					"			{{CONTENTS}}"
					"		</div>\n");
			};
		};

		class Pre : public ParentTag, public LangTag, public CoreTag {
			public:
			Pre() : StringTemplate("Pre") {
				this->setTemplateContents("pre", "		<pre{{>LANGTAG}}{{>CORETAG}}>\n"
					"			{{CONTENTS}}"
					"		</pre>\n");
			};
		};
	
		class Span : public ParentTag, public LangTag, public CoreTag {
			public:
			Span() : StringTemplate("Span") {
				this->setTemplateContents("span", "		<span{{>LANGTAG}}{{>CORETAG}}>{{CONTENTS}}</span>\n");
			};
		};

		class Script : public ParentTag {
			public:
			Script(std::string type, std::string charset = "", bool defer = false, std::string src = "",
					std::string xmlspace = "") : StringTemplate("Script") {
				this->setTemplateContents("script", "		<script"
					"{{#TYPE_S}} type=\"{{TYPE}}\"{{/TYPE_S}}"
					"{{#CHARSET_S}} charset=\"{{CHARSET}}\"{{/CHARSET_S}}"
					"{{#DEFER_S}} defer=\"{{DEFER}}\"{{/DEFER_S}}"
					"{{#SRC_S}} src=\"{{SRC}}\"{{/SRC_S}}"
					"{{#XMLSPACE_S}} xml:space=\"{{XMLSPACE}}\"{{/XMLSPACE_S}}");
				this->setType(type);
				this->setCharset(charset);
				this->setDefer(defer);
				this->setSrc(src);
				this->setXmlspace(xmlspace);
			};

			inline void setType(std::string type) {
				if (not type.empty()) this->dict.SetValueAndShowSection("TYPE", type, "TYPE_S");
			};

			inline void setCharset(std::string charset) {
				if (not charset.empty()) this->dict.SetValueAndShowSection("CHARSET", charset, "CHARSET_S");
			};

			inline void setDefer(bool defer) {
				if (defer) this->dict.SetValueAndShowSection("DEFER", "defer", "DEFER_S");
			};

			inline void setSrc(std::string src) {
				if (not src.empty()) this->dict.SetValueAndShowSection("SRC", src, "SRC_S");
			};

			inline void setXmlspace(std::string xmlspace) {
				if (not xmlspace.empty()) this->dict.SetValueAndShowSection("XMLSPACE", xmlspace, "XMLSPACE_S");
			};
		};

		class Form : public ParentTag, public LangTag, public CoreTag {
			public:
			Form(std::string action, std::string accept = "", std::string acceptcharset = "", std::string enctype = "",
					std::string method = "") : StringTemplate("Form") {
				this->setTemplateContents("form", "<form{{>LANGTAG}}{{>CORETAG}}"
					"{{#ACTION_S}} action=\"{{ACTION}}\"{{/ACTION_S}}"
					"{{#ACCEPT_S}} accept=\"{{ACCEPT}}\"{{/ACCEPT_S}}"
					"{{#ACCEPTCHARSET_S}} accept-charset=\"{{ACCEPTCHARSET}}\"{{/ACCEPTCHARSET_S}}"
					"{{#ENCTYPE_S}} enctype=\"{{ENCTYPE}}\"{{/ENCTYPE_S}}"
					"{{#METHOD_S}} method=\"{{METHOD}}\"{{/METHOD_S}}>\n"
					"{{CONTENTS}}"
					"</form>\n");
				this->setAction(action);
				this->setAccept(accept);
				this->setAcceptcharset(acceptcharset);
				this->setEnctype(enctype);
				this->setMethod(method);
			};

			inline void setAction(std::string action) {
				this->dict.SetValueAndShowSection("ACTION", action, "ACTION_S");
			};

			inline void setAccept(std::string accept) {
				if (not accept.empty()) this->dict.SetValueAndShowSection("ACCEPT", accept, "ACCEPT_S");
			};

			inline void setAcceptcharset(std::string acceptcharset) {
				if (not acceptcharset.empty()) 
					this->dict.SetValueAndShowSection("ACCEPTCHARSET", acceptcharset, "ACCEPTCHARSET_S");
			};

			inline void setEnctype(std::string enctype) {
				if (not enctype.empty()) this->dict.SetValueAndShowSection("ENCTYPE", enctype, "ENCTYPE_S");
			};

			inline void setMethod(std::string method) {
				if (not method.empty()) this->dict.SetValueAndShowSection("METHOD", method, "METHOD_S");
			};

		};

		class Input : public LangTag, public CoreTag {
			public:
			Input(std::string accept = "", std::string alt = "", bool checked = false, bool disabled = false,
					int maxlength = -1, std::string name = "", bool readonly = false, int size = -1,
					std::string src = "", std::string type = "", std::string value = "") : StringTemplate("Input") {
				this->setTemplateContents("input", "<input{{>LANGTAG}}{{>CORETAG}}"
					"{{#ACCEPT_S}} accept=\"{{ACCEPT}}\"{{/ACCEPT_S}}"
					"{{#ALT_S}} alt=\"{{ALT}}\"{{/ALT_S}}"
					"{{#CHECKED_S}} checked=\"{{CHECKED}}\"{{/CHECKED_S}}"
					"{{#DISABLED_S}} disabled=\"{{DISABLED}}\"{{/DISABLED_S}}"
					"{{#MAXLENGTH_S}} maxlength=\"{{MAXLENGTH}}\"{{/MAXLENGTH_S}}"
					"{{#NAME_S}} name=\"{{NAME}}\"{{/NAME_S}}"
					"{{#READONLY_S}} readonly=\"{{READONLY}}\"{{/READONLY_S}}"
					"{{#SIZE_S}} size=\"{{SIZE}}\"{{/SIZE_S}}"
					"{{#SRC_S}} src=\"{{SRC}}\"{{/SRC_S}}"
					"{{#TYPE_S}} type=\"{{TYPE}}\"{{/TYPE_S}}"
					"{{#VALUE_S}} value=\"{{VALUE}}\"{{/VALUE_S}}/>");
				this->setAccept(accept);
				this->setAlt(alt);
				this->setChecked(checked);
				this->setDisabled(disabled);
				this->setMaxlength(maxlength);
				this->setName(name);
				this->setReadonly(readonly);
				this->setSize(size);
				this->setSrc(src);
				this->setType(type);
				this->setValue(value);
			};

			inline void setAccept(std::string accept) {
				if (not accept.empty()) this->dict.SetValueAndShowSection("ACCEPT", accept, "ACCEPT_S");
			};

			inline void setAlt(std::string alt) {
				if (not alt.empty()) this->dict.SetValueAndShowSection("ALT", alt, "ALT_S");
			};

			inline void setChecked(bool checked) {
				if (checked) this->dict.SetValueAndShowSection("CHECKED", "checked", "CHECKED_S");
			};

			inline void setDisabled(bool disabled) {
				if (disabled) this->dict.SetValueAndShowSection("DISABLED", "disabled", "DISABLED_S");
			};

			inline void setMaxlength(int maxlength) {
				if (maxlength > -1) {
					this->dict.SetIntValue("MAXLENGTH", maxlength);
					this->dict.ShowSection("MAXLENGTH_S");
				}
			};

			inline void setName(std::string name) {
				if (not name.empty()) this->dict.SetValueAndShowSection("NAME", name, "NAME_S");
			};

			inline void setReadonly(bool readonly) {
				if (readonly) this->dict.SetValueAndShowSection("READONLY", "readonly", "READONLY_S");
			};

			inline void setSize(int size) {
				if (size > -1) {
					this->dict.SetIntValue("SIZE", size);
					this->dict.ShowSection("SIZE_S");
				}
			};

			inline void setSrc(std::string src) {
				if (not src.empty()) this->dict.SetValueAndShowSection("SRC", src, "SRC_S");
			};

			inline void setType(std::string type) {
				if (not type.empty() && (type == "button" || type == "checkbox" || type == "file" || type == "hidden" ||
						type == "image" || type == "password" || type == "radio" || type == "reset" || type == "submit" ||
						type == "text"))
					this->dict.SetValueAndShowSection("TYPE", type, "TYPE_S");
			};

			inline void setValue(std::string value) {
				if (not value.empty()) this->dict.SetValueAndShowSection("VALUE", value, "VALUE_S");
			};

		};

	}

/* Close namespaces */
				}
			}
		}
	}
}

#endif
