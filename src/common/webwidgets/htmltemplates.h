
#ifndef __HTMLTEMPLATES_H
#define __HTMLTEMPLATES_H

#include "log.h"

#include <ctemplate/template.h>
#include <string>
#include <list>
#include <boost/function.hpp>

namespace firestarter {
	namespace common {
		namespace WebWidgets {
			namespace Templates {

	class CoreAttr {
		private:
		std::string _class;
		std::string title;
		std::string id;
		std::string style;

		public:
		void populate(ctemplate::TemplateDictionary & dict) {
			ctemplate::TemplateDictionary * core_dict = dict.AddIncludeDictionary("CORETAG");
			core_dict->SetFilename("CORETAG");
			StringToTemplateCache("CORETAG", "{{#STYLE_S}} style=\"{{STYLE}}\"{{/STYLE_S}}"
				"{{#CLASS_S}} class=\"{{CLASS}}\"{{/CLASS_S}}"
				"{{#ID_S}} id=\"{{ID}}\"{{/ID_S}}"
				"{{#TITLE_S}} title=\"{{TITLE}}\"{{/TITLE_S}}", ctemplate::DO_NOT_STRIP);
			if (not this->style.empty()) core_dict->SetValueAndShowSection("STYLE", this->style, "STYLE_S");
			if (not this->_class.empty()) core_dict->SetValueAndShowSection("CLASS", this->_class, "CLASS_S");
			if (not this->id.empty()) core_dict->SetValueAndShowSection("ID", this->id, "ID_S");
			if (not this->title.empty()) core_dict->SetValueAndShowSection("TITLE", this->title, "TITLE_S");
		};

		inline void setClass(std::string _class) { this->_class = _class; };
		inline void setTitle(std::string title) { this->title = title; };
		inline void setId(std::string id) { this->id = id; };
		inline void setStyle(std::string style) { this->style = style; };
	};

	class LangAttr {
		private:
		std::string dir;
		std::string lang;
		std::string xmllang;

		public:
		void populate(ctemplate::TemplateDictionary & dict) {
			ctemplate::TemplateDictionary * lang_dict = dict.AddIncludeDictionary("LANGTAG");
			lang_dict->SetFilename("LANGTAG");
			StringToTemplateCache("LANGTAG", "{{#DIR_S}} dir=\"{{DIR}}\"{{/DIR_S}}"
				"{{#LANG_S}} lang=\"{{LANG}}\"{{/LANG_S}}"
				"{{#XMLLANG_S}} xml:lang=\"{{XMLLANG}}\"{{/XMLLANG_S}}", ctemplate::DO_NOT_STRIP);
			if (not this->dir.empty() && (this->dir == "rtl" || this->dir == "ltr"))
				lang_dict->SetValueAndShowSection("DIR", this->dir, "DIR_S");
			if (not this->lang.empty()) lang_dict->SetValueAndShowSection("LANG", this->lang, "LANG_S");
			if (not this->xmllang.empty()) lang_dict->SetValueAndShowSection("XMLLANG", this->xmllang, "XMLLANG_S");
		};

		inline void setDir(std::string dir) { this->dir = dir; };
		inline void setLang(std::string lang) { this->lang = lang; };
		inline void setXmllang(std::string xmllang) { this->xmllang = xmllang; };
	};

	class StandardAttr : public CoreAttr, public LangAttr {
		public:
		void populate(ctemplate::TemplateDictionary dict) {
			static_cast<CoreAttr *>(this)->populate(dict);
			static_cast<LangAttr *>(this)->populate(dict);
		};
	};

	class NoAttr {
		public:
		inline void populate(ctemplate::TemplateDictionary & dict) { };
	};

	template <class Type, class Attributes>
	class Tag {
		protected:
		ctemplate::TemplateDictionary dict;
		std::string template_name;
		void cacheTemplate(std::string template_name, std::string template_contents) {
			this->template_name = template_name;
			StringToTemplateCache(template_name, template_contents, ctemplate::DO_NOT_STRIP);
		};
		Tag() : dict("HTMLTAG") { };

		public:
		Attributes attributes;

		void setContents(std::string contents) { this->dict["CONTENTS"] = contents; };
		std::string render() {
			std::string out;
			this->attributes.populate(this->dict);
			static_cast<Type *>(this)->populate();
			ExpandTemplate(this->template_name, ctemplate::DO_NOT_STRIP, &this->dict, &out);
			return out;
		};
	};

	template <class Type, class Attributes>
	class ContainerTag : public Tag<ContainerTag<Type, Attributes>, Attributes> {
		typedef std::list<boost::function<std::string & ()> > Children;
		protected:
		Children children;

		public:
		void createTemplate() { static_cast<Type *>(this)->createTemplate(); };
		template <class Child>
		void registerChild(Child & child) { this->children.push_back(&child.render); };
		void populate() {
			std::string children_output;
			for (boost::function<std::string & ()> renderChild : this->children)
				children_output += renderChild();
			this->setContents(children_output);
			static_cast<Type *>(this)->populate();
		};
	};

	class Headers : public Tag<Headers, NoAttr> {
		private:
		std::string contenttype;
		std::string charset;

		public:
		Headers() : contenttype("text/html"), charset("utf-8") { };
		void populate() {
			this->cacheTemplate("headers", "Content-Type: {{CONTENT_TYPE}}; charset={{CHARSET}}\r\n\r\n");
			this->dict["CONTENT_TYPE"] = this->contenttype;
			this->dict["CHARSET"] = this->charset;
		};
		void setContenttype(std::string contenttype) { this->contenttype = contenttype; };
		void setCharset(std::string charset) { this->charset = charset; };
	};

	namespace Tags {

		class DocType : public Tag<DocType, NoAttr> {
			public:
			void populate() {
				this->cacheTemplate("doctype", "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN'"
				" 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n");
			};
		};

		class Html : public ContainerTag<Html, LangAttr> {
			private:
			std::string xmlns;

			public:
			void populate() {
				this->cacheTemplate("html", "<html{{#XMLNS_S}} xmlns=\"{{XMLNS}}\"{{/XMLNS_S}}{{>LANGTAG}}>\n"
					"{{CONTENTS}}"
					"</html>\n");
				if (not this->xmlns.empty()) this->dict.SetValueAndShowSection("XMLNS", this->xmlns, "XMLNS_S");
			};
			void setXmlns(std::string xmlns) { this->xmlns = xmlns; };
		};

		class Meta : public Tag<Meta, LangAttr> {
			private:
			std::string httpequiv;
			std::string content;
			std::string name;
			std::string scheme;

			public:
			void populate() {
				this->cacheTemplate("meta",
					"<meta{{>LANGTAG}}{{#HTTPEQUIV_S}} http-equiv=\"{{HTTPEQUIV}}\"{{/HTTPEQUIV_S}}"
					"{{#CONTENT_S}} content=\"{{CONTENT}}\"{{/CONTENT_S}}"
					"{{#NAME_S}} name=\"{{NAME}}\"{{/NAME_S}}"
					"{{#SCHEME_S}} scheme=\"{{SCHEME}}\"{{/SCHEME_S}}/>\n");
				if (not this->httpequiv.empty())
					this->dict.SetValueAndShowSection("HTTPEQUIV", this->httpequiv, "HTTPEQUIV_S");
				if (not this->content.empty())
					this->dict.SetValueAndShowSection("CONTENT", this->content, "CONTENT_S");
				if (not this->name.empty())
					this->dict.SetValueAndShowSection("NAME", this->name, "NAME_S");
				if (not this->scheme.empty())
					this->dict.SetValueAndShowSection("SCHEME", this->scheme, "SCHEME_S");
			};
			void setHttpequiv(std::string httpequiv) { this->httpequiv = httpequiv; };
			void setContent(std::string content) { this->content = content; };
			void setName(std::string name) { this->name = name; };
			void setScheme(std::string scheme) { this->scheme = scheme; };
		};

		class Title : public Tag<Title, LangAttr> {
			public:
			void populate() {
				this->cacheTemplate("title", "<title{{>LANGTAG}}>{{CONTENTS}}</title>\n");
			};
		};

		class Body : public ContainerTag<Body, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("body", "<body{{>LANGTAG}}{{>CORETAG}}>\n"
					"{{CONTENTS}}"
					"</body>\n");
			};
		};

		class P : public Tag<P, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("p", "<p{{>LANGTAG}}{{>CORETAG}}>{{CONTENTS}}</p>\n");
			};
		};

		class Div : public ContainerTag<Div, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("div", "<div{{>LANGTAG}}{{>CORETAG}}>\n"
					"{{CONTENTS}}"
					"</div>\n");
			};
		};

		class Pre : public Tag<Pre, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("pre", "<pre{{>LANGTAG}}{{>CORETAG}}>\n"
					"{{CONTENTS}}"
					"</pre>\n");
			};
		};

		class Span : public Tag<Span, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("span", "<span{{>LANGTAG}}{{>CORETAG}}>{{CONTENTS}}</span>\n");
			};
		};

		class Script : public Tag<Script, NoAttr> {
			private:
			std::string type;
			std::string charset;
			bool defer;
			std::string src;
			std::string xmlspace;

			public:
			void populate() {
				this->cacheTemplate("script", "<script"
					"{{#TYPE_S}} type=\"{{TYPE}}\"{{/TYPE_S}}"
					"{{#CHARSET_S}} charset=\"{{CHARSET}}\"{{/CHARSET_S}}"
					"{{#DEFER_S}} defer=\"defer\"{{/DEFER_S}}"
					"{{#SRC_S}} src=\"{{SRC}}\"{{/SRC_S}}"
					"{{#XMLSPACE_S}} xml:space=\"{{XMLSPACE}}\"{{/XMLSPACE_S}}>\n"
					"{{CONTENTS}}"
					"</script>\n");
				if (not this->type.empty()) this->dict.SetValueAndShowSection("TYPE", this->type, "TYPE_S");
				if (not this->charset.empty())
					this->dict.SetValueAndShowSection("CHARSET", this->charset, "CHARSET_S");
				if (this->defer) this->dict.ShowSection("DEFER_S");
				if (not this->src.empty()) this->dict.SetValueAndShowSection("SRC", this->src, "SRC_S");
				if (not this->xmlspace.empty())
					this->dict.SetValueAndShowSection("XMLSPACE", this->xmlspace, "XMLSPACE_S");
			};
			void setType(std::string type) { this->type = type; };
			void setCharset(std::string charset) { this->charset = charset; };
			void setDefer(bool defer) { this->defer = defer; };
			void setXmlspace(std::string xmlspace) { this->xmlspace = xmlspace; };
		};

		class Form : public ContainerTag<Form, StandardAttr> {
			private:
			std::string action;
			std::string accept;
			std::string acceptcharset;
			std::string enctype;
			std::string method;

			public:
			void populate() {
				this->cacheTemplate("form", "<form{{>LANGTAG}}{{>CORETAG}}"
					"{{#ACTION_S}} action=\"{{ACTION}}\"{{/ACTION_S}}"
					"{{#ACCEPT_S}} accept=\"{{ACCEPT}}\"{{/ACCEPT_S}}"
					"{{#ACCEPTCHARSET_S}} accept-charset=\"{{ACCEPTCHARSET}}\"{{/ACCEPTCHARSET_S}}"
					"{{#ENCTYPE_S}} enctype=\"{{ENCTYPE}}\"{{/ENCTYPE_S}}"
					"{{#METHOD_S}} method=\"{{METHOD}}\"{{/METHOD_S}}>\n"
					"{{CONTENTS}}"
					"</form>\n");
				if (not this->action.empty()) this->dict.SetValueAndShowSection("ACTION", this->action, "ACTION_S");
				if (not this->accept.empty()) this->dict.SetValueAndShowSection("ACCEPT", this->accept, "ACCEPT_S");
				if (not this->acceptcharset.empty()) 
					this->dict.SetValueAndShowSection("ACCEPTCHARSET", this->acceptcharset, "ACCEPTCHARSET_S");
				if (not this->enctype.empty())
					this->dict.SetValueAndShowSection("ENCTYPE", this->enctype, "ENCTYPE_S");
				if (not this->method.empty()) this->dict.SetValueAndShowSection("METHOD", this->method, "METHOD_S");
			};
			void setAction(std::string action) { this->action = action; };
			void setAccept(std::string accept) { this->accept = accept; };
			void setAcceptcharset(std::string acceptcharset) { this->acceptcharset = acceptcharset; };
			void setEnctype(std::string enctype) { this->enctype = enctype; };
			void setMethod(std::string method) { this->method = method; };
		};

		class Input : public Tag<Input, StandardAttr> {
			private:
			std::string accept;
			std::string alt;
			bool checked;
			bool disabled;
			unsigned int maxlength;
			std::string name;
			bool readonly;
			unsigned int size;
			std::string src;
			std::string type;
			std::string value;

			public:
			void populate() {
				this->cacheTemplate("input", "<input{{>LANGTAG}}{{>CORETAG}}"
					"{{#ACCEPT_S}} accept=\"{{ACCEPT}}\"{{/ACCEPT_S}}"
					"{{#ALT_S}} alt=\"{{ALT}}\"{{/ALT_S}}"
					"{{#CHECKED_S}} checked=\"checked\"{{/CHECKED_S}}"
					"{{#DISABLED_S}} disabled=\"disabled\"{{/DISABLED_S}}"
					"{{#MAXLENGTH_S}} maxlength=\"{{MAXLENGTH}}\"{{/MAXLENGTH_S}}"
					"{{#NAME_S}} name=\"{{NAME}}\"{{/NAME_S}}"
					"{{#READONLY_S}} readonly=\"readonly\"{{/READONLY_S}}"
					"{{#SIZE_S}} size=\"{{SIZE}}\"{{/SIZE_S}}"
					"{{#SRC_S}} src=\"{{SRC}}\"{{/SRC_S}}"
					"{{#TYPE_S}} type=\"{{TYPE}}\"{{/TYPE_S}}"
					"{{#VALUE_S}} value=\"{{VALUE}}\"{{/VALUE_S}}/>");
				if (not this->accept.empty()) this->dict.SetValueAndShowSection("ACCEPT", this->accept, "ACCEPT_S");
				if (not this->alt.empty()) this->dict.SetValueAndShowSection("ALT", this->alt, "ALT_S");
				if (this->checked) this->dict.ShowSection("CHECKED_S");
				if (this->disabled) this->dict.ShowSection("DISABLED_S");
				if (this->maxlength > -1) {
					this->dict.SetIntValue("MAXLENGTH", this->maxlength);
					this->dict.ShowSection("MAXLENGTH_S");
				}
				if (not this->name.empty()) this->dict.SetValueAndShowSection("NAME", this->name, "NAME_S");
				if (this->readonly) this->dict.ShowSection("READONLY_S");
				if (this->size > -1) {
					this->dict.SetIntValue("SIZE", this->size);
					this->dict.ShowSection("SIZE_S");
				}
				if (not this->src.empty()) this->dict.SetValueAndShowSection("SRC", this->src, "SRC_S");
				if (not this->type.empty() && (this->type == "button" || this->type == "checkbox" ||
						this->type == "file" || this->type == "hidden" || this->type == "image" ||
						this->type == "password" || this->type == "radio" || this->type == "reset" ||
						this->type == "submit" || this->type == "text"))
					this->dict.SetValueAndShowSection("TYPE", this->type, "TYPE_S");
				if (not this->value.empty()) this->dict.SetValueAndShowSection("VALUE", this->value, "VALUE_S");
			};
		};

	}

/* Close namespaces */
			}
		}
	}
}

#endif
