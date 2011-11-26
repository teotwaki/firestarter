#include "helper.h"

bool file_exists(std::string path) {
	struct stat st;
	stat(path.c_str(), &st);
	if (S_ISREG(st.st_mode))
		return true;

	return false;
}

