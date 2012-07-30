
#ifndef __HTMLTEMPLATES_H
#define __HTMLTEMPLATES_H

#include "log.h"

#include <ctemplate/template.h>
#include <string>
#include <list>
#include <utility>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <sstream>

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

		inline void setClass(std::string const & _class) { this->_class = _class; };
		inline void setTitle(std::string const & title) { this->title = title; };
		inline void setId(std::string const & id) { this->id = id; };
		inline void setStyle(std::string const & style) { this->style = style; };
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

		inline void setDir(std::string const & dir) { this->dir = dir; };
		inline void setLang(std::string const & lang) { this->lang = lang; };
		inline void setXmllang(std::string const & xmllang) { this->xmllang = xmllang; };
	};

	class StandardAttr : public CoreAttr, public LangAttr {
		public:
		void populate(ctemplate::TemplateDictionary & dict) {
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
		void cacheTemplate(std::string const & template_name, std::string template_contents) {
			this->template_name = template_name;
			StringToTemplateCache(this->template_name, template_contents, ctemplate::DO_NOT_STRIP);
		};
		typedef std::pair<std::string const, std::string const> ExtraAttribute;
		std::list<ExtraAttribute> extra_attributes;
		Tag() : dict("HTMLTAG") { };

		public:
		Attributes attributes;

		Type & setExtraAttribute(std::string const & attr_name, std::string const & attr_value) {
			this->extra_attributes.push_back(std::make_pair(attr_name, attr_value));
			return static_cast<Type &>(*this);
		};
		inline void setContents(std::string const & contents) { this->dict["CONTENTS"] = contents; };
		std::string const render() {
			std::string out;
			this->attributes.populate(this->dict);

			for (ExtraAttribute attr : this->extra_attributes)
				if (not attr.first.empty()) {
					std::stringstream ss;
					ss << " " << attr.first << "=\"" << attr.second << "\"";
					this->dict["EXTRA_ATTRS"] = ss.str();
				}

			static_cast<Type *>(this)->populate();
			ExpandTemplate(this->template_name, ctemplate::DO_NOT_STRIP, &this->dict, &out);
			return out;
		};
	};

	template <class Type, class Attributes>
	class ContainerTag : public Tag<ContainerTag<Type, Attributes>, Attributes> {
		std::list<boost::shared_ptr<void> > children_list;
		typedef std::list<boost::function<std::string ()> > ChildrenRenderers;
		protected:
		ChildrenRenderers children;

		public:
		template <class Child>
		void registerChild(const Child & child) {
			this->children.push_back(boost::bind(&Child::render, boost::cref(child)));
		};
		void populate() {
			std::string children_output;
			for (boost::function<std::string ()> renderChild : this->children) {
				children_output += renderChild();
			}
			if (not children_output.empty()) this->setContents(children_output);
			static_cast<Type *>(this)->populate();
		};
		template <class Child>
		Child & getNew() {
			Child * child_ptr = new Child;
			boost::shared_ptr<void> child(child_ptr);
			this->children_list.push_back(child);
			return *child_ptr;
		};
		template <class Child>
		Child & addChild() {
			Child & child = this->getNew<Child>();
			this->registerChild(child);
			return child;
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
		inline Headers & setContenttype(std::string const & contenttype) { this->contenttype = contenttype; return *this; };
		inline Headers & setCharset(std::string const & charset) { this->charset = charset; return *this; };
	};

	namespace Tags {

		class DocType : public Tag<DocType, NoAttr> {
			public:
			void populate() {
				this->cacheTemplate("doctype", "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN'"
					" 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n");
			};
		};

		class Comment : public ContainerTag<Comment, NoAttr> {
			private:
			std::string condition;

			public:
			void populate() {
				this->cacheTemplate("comment", "<!--{{#CONDITION_S}}[if {{CONDITION}}]>{{CONTENTS}}<![endif]-->{{/CONDITION_S}}"
					"{{#NOCONDITION_S}} {{CONTENTS}} -->{{/NOCONDITION_S}}\n");
				if (not this->condition.empty()) this->dict.SetValueAndShowSection("CONDITION", this->condition, "CONDITION_S");
				else this->dict.ShowSection("NOCONDITION_S");
			};
			inline Comment & setCondition(std::string const & condition) { this->condition = condition; return *this; };
		};

		class Html : public ContainerTag<Html, LangAttr> {
			private:
			std::string xmlns;

			public:
			void populate() {
				this->cacheTemplate("html", "<html{{#XMLNS_S}} xmlns=\"{{XMLNS}}\"{{/XMLNS_S}}{{>LANGTAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</html>\n");
				if (not this->xmlns.empty()) this->dict.SetValueAndShowSection("XMLNS", this->xmlns, "XMLNS_S");
			};
			inline Html & setXmlns(std::string const & xmlns) { this->xmlns = xmlns; return *this; };
		};

		class Head : public ContainerTag<Head, LangAttr> {
			private:
			std::string profile;

			public:
			void populate() {
				this->cacheTemplate("head", "<head{{#PROFILE_S}} profile=\"{{PROFILE}}\"{{/PROFILE_S}}{{>LANGTAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</head>\n");
				if (not this->profile.empty())
					this->dict.SetValueAndShowSection("PROFILE", this->profile, "PROFILE_S");

			};
			inline Head & setProfile(std::string const & profile) { this->profile = profile; return *this; };
		};

		class Style : public Tag<Style, LangAttr> {
			private:
			std::string type;
			std::string media;

			public:
			void populate() {
				this->cacheTemplate("style", "<style{{#TYPE_S}} type=\"{{TYPE}}\"{{/TYPE_S}}"
					"{{#MEDIA_S}} media=\"{{MEDIA}}\"{{/MEDIA_S}}{{>LANGTAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</style>\n");
				if (this->type.empty()) this->dict.SetValueAndShowSection("TYPE", "text/css", "TYPE");
				else this->dict.SetValueAndShowSection("TYPE", this->type, "TYPE_S");
				if (not this->media.empty() && (this->media == "screen" || this->media == "tty" ||
						this->media == "tv" || this->media == "projection" || this->media == "handheld" ||
						this->media == "print" || this->media == "braille" || this->media == "aural" ||
						this->media == "all"))
					this->dict.SetValueAndShowSection("MEDIA", this->media, "MEDIA_S");
			};
			inline Style & setType(std::string const & type) { this->type = type; return *this; };
			inline Style & setMedia(std::string const & media) { this->media = media; return *this; };
		};

		class Br : public Tag<Br, CoreAttr> {
			public:
			void populate() {
				this->cacheTemplate("br", "<br{{>CORETAG}} />\n");
			};
		};

		class A : public ContainerTag<A, StandardAttr> {
			private:
			std::string charset;
			std::string coords;
			std::string href;
			std::string hreflang;
			std::string name;
			std::string rel;
			std::string rev;
			std::string shape;

			public:
			void populate() {
				this->cacheTemplate("a", "<a{{>CORETAG}}{{>LANGTAG}}"
					"{{#CHARSET_S}} charset=\"{{CHARSET}}\"{{/CHARSET_S}}"
					"{{#COORDS_S}} coords=\"{{COORDS}}\"{{/COORDS_S}}"
					"{{#HREF_S}} href=\"{{HREF}}\"{{/HREF_S}}"
					"{{#HREFLANG_S}} hreflang=\"{{HREFLANG}}\"{{/HREFLANG_S}}"
					"{{#NAME_S}} name=\"{{NAME}}\"{{/NAME_S}}"
					"{{#REL_S}} rel=\"{{REL}}\"{{/REL_S}}"
					"{{#REV_S}} rev=\"{{REV}}\"{{/REV_S}}"
					"{{#SHAPE_S}} shape=\"{{SHAPE}}\"{{/SHAPE_S}}{{EXTRA_ATTRS}}>"
					"{{CONTENTS}}"
					"</a>\n");
				if (not this->charset.empty())
					this->dict.SetValueAndShowSection("CHARSET", this->charset, "CHARSET_S");
				if (not this->coords.empty())
					this->dict.SetValueAndShowSection("COORDS", this->coords, "COORDS_S");
				if (not this->href.empty())
					this->dict.SetValueAndShowSection("HREF", this->href, "HREF_S");
				if (not this->hreflang.empty())
					this->dict.SetValueAndShowSection("HREFLANG", this->hreflang, "HREFLANG_S");
				if (not this->name.empty())
					this->dict.SetValueAndShowSection("NAME", this->name, "NAME_S");
				if (not this->rel.empty())
					this->dict.SetValueAndShowSection("REL", this->rel, "REL_S");
				if (not this->rev.empty())
					this->dict.SetValueAndShowSection("REV", this->rev, "REV_S");
				if (not this->shape.empty() && (this->shape == "default" || this->shape == "rect" ||
						this->shape == "circle" || this->shape == "poly"))
					this->dict.SetValueAndShowSection("SHAPE", this->shape, "SHAPE_S");
			};
			inline A & setCharset(std::string const & charset) { this->charset = charset; return *this; };
			inline A & setCoords(std::string const & coords) { this->coords = coords; return *this; };
			inline A & setHref(std::string const & href) { this->href = href; return *this; };
			inline A & setHreflang(std::string const & hreflang) { this->hreflang = hreflang; return *this; };
			inline A & setName(std::string const & name) { this->name = name; return *this; };
			inline A & setRel(std::string const & rel) { this->rel = rel; return *this; };
			inline A & setRev(std::string const & rev) { this->rev = rev; return *this; };
			inline A & setShape(std::string const & shape) { this->shape = shape; return *this; };
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
					"{{#SCHEME_S}} scheme=\"{{SCHEME}}\"{{/SCHEME_S}}{{EXTRA_ATTRS}}/>\n");
				if (not this->httpequiv.empty())
					this->dict.SetValueAndShowSection("HTTPEQUIV", this->httpequiv, "HTTPEQUIV_S");
				if (not this->content.empty())
					this->dict.SetValueAndShowSection("CONTENT", this->content, "CONTENT_S");
				if (not this->name.empty())
					this->dict.SetValueAndShowSection("NAME", this->name, "NAME_S");
				if (not this->scheme.empty())
					this->dict.SetValueAndShowSection("SCHEME", this->scheme, "SCHEME_S");
			};
			inline Meta & setHttpequiv(std::string const & httpequiv) { this->httpequiv = httpequiv; return *this;};
			inline Meta & setContent(std::string const & content) { this->content = content; return *this;};
			inline Meta & setName(std::string const & name) { this->name = name; return *this;};
			inline Meta & setScheme(std::string const & scheme) { this->scheme = scheme; return *this; };
		};

		class Title : public Tag<Title, LangAttr> {
			public:
			void populate() {
				this->cacheTemplate("title", "<title{{>LANGTAG}}{{EXTRA_ATTRS}}>{{CONTENTS}}</title>\n");
			};
		};

		class Body : public ContainerTag<Body, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("body", "<body{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</body>\n");
			};
		};

		class P : public Tag<P, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("p", "<p{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>{{CONTENTS}}</p>\n");
			};
		};

		class Div : public ContainerTag<Div, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("div", "<div{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</div>\n");
			};
		};

		class Pre : public Tag<Pre, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("pre", "<pre{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</pre>\n");
			};
		};

		class Span : public Tag<Span, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("span", "<span{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>{{CONTENTS}}</span>\n");
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
					"{{#XMLSPACE_S}} xml:space=\"{{XMLSPACE}}\"{{/XMLSPACE_S}}{{EXTRA_ATTRS}}>"
					"{{#CONTENTS_S}}//<![CDATA[\n"
					"{{CONTENTS}}"
					"//]]>{{/CONTENTS_S}}</script>\n");
				if (not this->type.empty()) this->dict.SetValueAndShowSection("TYPE", this->type, "TYPE_S");
				if (not this->charset.empty())
					this->dict.SetValueAndShowSection("CHARSET", this->charset, "CHARSET_S");
				if (this->defer) this->dict.ShowSection("DEFER_S");
				if (not this->src.empty()) this->dict.SetValueAndShowSection("SRC", this->src, "SRC_S");
				else this->dict.ShowSection("CONTENTS_S");
				if (not this->xmlspace.empty())
					this->dict.SetValueAndShowSection("XMLSPACE", this->xmlspace, "XMLSPACE_S");
			};
			inline Script & setType(std::string const & type) { this->type = type; return *this; };
			inline Script & setCharset(std::string const & charset) { this->charset = charset; return *this; };
			inline Script & setDefer(bool defer) { this->defer = defer; return *this; };
			inline Script & setSrc(std::string const & src) { this->src = src; return *this; };
			inline Script & setXmlspace(std::string const & xmlspace) { this->xmlspace = xmlspace; return *this; };
		};

		class Link : public Tag<Link, StandardAttr> {
			private:
			std::string charset;
			std::string href;
			std::string hreflang;
			std::string media;
			std::string rel;
			std::string rev;
			std::string target;
			std::string type;

			public:
			void populate() {
				this->cacheTemplate("link", "<link{{>CORETAG}}{{>LANGTAG}}"
					"{{#CHARSET_S}} charset=\"{{CHARSET}}\"{{/CHARSET_S}}"
					"{{#HREF_S}} href=\"{{HREF}}\"{{/HREF_S}}"
					"{{#HREFLANG_S}} hreflang=\"{{HREFLANG}}\"{{/HREFLANG_S}}"
					"{{#MEDIA_S}} media=\"{{MEDIA}}\"{{/MEDIA_S}}"
					"{{#REL_S}} rel=\"{{REL}}\"{{/REL_S}}"
					"{{#REV_S}} rev=\"{{REV}}\"{{/REV_S}}"
					"{{#TARGET_S}} target=\"{{TARGET}}\"{{/TARGET_S}}"
					"{{#TYPE_S}} type=\"{{TYPE}}\"{{/TYPE_S}}{{EXTRA_ATTRS}}/>\n");
				if (not this->charset.empty())
					this->dict.SetValueAndShowSection("CHARSET", this->charset, "CHARSET_S");
				if (not this->href.empty())
					this->dict.SetValueAndShowSection("HREF", this->href, "HREF_S");
				if (not this->hreflang.empty())
					this->dict.SetValueAndShowSection("HREFLANG", this->hreflang, "HREFLANG_S");
				if (not this->media.empty() && (this->media == "screen" || this->media == "tty" ||
						this->media == "tv" || this->media == "projection" || this->media == "handheld" ||
						this->media == "print" || this->media == "braille" || this->media == "aural" ||
						this->media == "all"))
					this->dict.SetValueAndShowSection("MEDIA", this->media, "MEDIA_S");
				if (not this->rel.empty() && (this->rel == "alternate" || this->rel == "appendix" ||
						this->rel == "bookmark" || this->rel == "chapter" || this->rel == "contents" ||
						this->rel == "copyright" || this->rel == "glossary" || this->rel == "help" ||
						this->rel == "home" || this->rel == "index" || this->rel == "next" || this->rel == "prev" ||
						this->rel == "section" || this->rel == "start" || this->rel == "stylesheet" ||
						this->rel == "subsection"))
					this->dict.SetValueAndShowSection("REL", this->rel, "REL_S");
				if (not this->rev.empty() && (this->rev == "alternate" || this->rev == "appendix" ||
						this->rev == "bookmark" || this->rev == "chapter" || this->rev == "contents" ||
						this->rev == "copyright" || this->rev == "glossary" || this->rev == "help" ||
						this->rev == "home" || this->rev == "index" || this->rev == "next" || this->rev == "prev" ||
						this->rev == "section" || this->rev == "start" || this->rev == "stylesheet" ||
						this->rev == "subsection"))
					this->dict.SetValueAndShowSection("REV", this->rev, "REV_S");
				if (not this->target.empty())
					this->dict.SetValueAndShowSection("TARGET", this->target, "TARGET_S");
				if (not this->type.empty())
					this->dict.SetValueAndShowSection("TYPE", this->type, "TYPE_S");
			};
			inline Link & setCharset(std::string const & charset) { this->charset = charset; return *this; };
			inline Link & setHref(std::string const & href) { this->href = href; return *this; };
			inline Link & setHreflang(std::string const & hreflang) { this->hreflang = hreflang; return *this; };
			inline Link & setMedia(std::string const & media) { this->media = media; return *this; };
			inline Link & setRel(std::string const & rel) { this->rel = rel; return *this; };
			inline Link & setRev(std::string const & rev) { this->rev = rev; return *this; };
			inline Link & setTarget(std::string const & target) { this->target = target; return *this; };
			inline Link & setType(std::string const & type) { this->type = type; return *this; };
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
					"{{#METHOD_S}} method=\"{{METHOD}}\"{{/METHOD_S}}{{EXTRA_ATTRS}}>\n"
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
			inline Form & setAction(std::string const & action) { this->action = action; return *this; };
			inline Form & setAccept(std::string const & accept) { this->accept = accept; return *this; };
			inline Form & setAcceptcharset(std::string const & acceptcharset) { this->acceptcharset = acceptcharset; return *this; };
			inline Form & setEnctype(std::string const & enctype) { this->enctype = enctype; return *this; };
			inline Form & setMethod(std::string const & method) { this->method = method; return *this; };
		};

		class Label : public Tag<Label, StandardAttr> {
			private:
			std::string _for;

			public:
			void populate() {
				this->cacheTemplate("label", "<label{{>CORETAG}}{{>LANGTAG}}"
					"{{#FOR_S}} for=\"{{FOR}}\"{{/FOR_S}}{{EXTRA_ATTRS}}>{{CONTENTS}}</label>\n");
				if (not this->_for.empty()) this->dict.SetValueAndShowSection("FOR", this->_for, "FOR_S");
			};
			inline Label & setFor(std::string const & _for) { this->_for = _for; return *this; };
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
					"{{#VALUE_S}} value=\"{{VALUE}}\"{{/VALUE_S}}{{EXTRA_ATTRS}}/>\n");
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
			inline Input & setAccept(std::string const & accept) { this->accept = accept; return *this; };
			inline Input & setAlt(std::string const & alt) { this->alt = alt; return *this; };
			inline Input & setChecked(bool checked) { this->checked = checked; return *this; };
			inline Input & setDisabled(bool disabled) { this->disabled = disabled; return *this; };
			inline Input & setMaxlength(unsigned int maxlength) { this->maxlength = maxlength; return *this; };
			inline Input & setName(std::string const & name) { this->name = name; return *this; };
			inline Input & setReadonly(bool readonly) { this->readonly = readonly; return *this; };
			inline Input & setSize(unsigned int size) { this->size = size; return *this; };
			inline Input & setSrc(std::string const & src) { this->src = src; return *this; };
			inline Input & setType(std::string const & type) { this->type = type; return *this; };
			inline Input & setValue(std::string const & value) { this->value = value; return *this; };
		};

		class H1 : public ContainerTag<H1, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("h1", "<h1{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</h1>\n");
			};
		};

		class H2 : public ContainerTag<H2, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("h2", "<h2{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</h2>\n");
			};
		};

		class H3 : public ContainerTag<H3, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("h3", "<h3{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</h3>\n");
			};
		};

		class H4 : public ContainerTag<H4, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("h4", "<h4{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</h4>\n");
			};
		};

		class H5 : public ContainerTag<H5, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("h5", "<h5{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</h5>\n");
			};
		};

		class H6 : public ContainerTag<H6, StandardAttr> {
			public:
			void populate() {
				this->cacheTemplate("h6", "<h6{{>LANGTAG}}{{>CORETAG}}{{EXTRA_ATTRS}}>\n"
					"{{CONTENTS}}"
					"</h6>\n");
			};
		};

	}

/* Close namespaces */
			}
		}
	}
}

#endif
