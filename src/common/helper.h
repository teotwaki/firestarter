#ifndef __HELPER_H
#define __HELPER_H

#include "foreach.h"

#include <string>
#include <sys/stat.h>

#include "log.h"
#include "module.h"
#include "exceptions.h"

bool file_exists(std::string path);

#endif
