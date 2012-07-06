#include "htmltemplates.h"

DECLARE_EXTERN_LOG(logger);

using namespace firestarter::module::core::WebInterface::Templates;

void FileTemplate::expand() {
	using namespace ctemplate;

	ExpandTemplate(this->tpl_path, DO_NOT_STRIP, &(this->dict), &(this->out));
}

void StringTemplate::expand() {
	using namespace ctemplate;

	StringToTemplateCache(this->cache_name, this->tpl_contents, DO_NOT_STRIP);
	ExpandTemplate(this->cache_name, DO_NOT_STRIP, &(this->dict), &(this->out));
}
