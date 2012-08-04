#ifndef FIRESTARTER_HELPER_HPP
#define FIRESTARTER_HELPER_HPP

#include <string>
#include <sys/stat.h>

#include "log.hpp"
#include "module.hpp"
#include "exceptions.hpp"

bool file_exists(std::string path);

#endif
