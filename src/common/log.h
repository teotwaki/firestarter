#ifndef __LOGGER_H
#define __LOGGER_H

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <string>
#include <cstdlib>
#include <cstring>

#define DECLARE_LOG(var, name) \
	log4cxx::LoggerPtr var(log4cxx::Logger::getLogger(name))

#define LOG_TRACE(logger, msg)	LOG4CXX_TRACE(logger, msg)
#define LOG_T(msg)				LOG4CXX_TRACE(logger, msg)

#define LOG_DEBUG(logger, msg)	LOG4CXX_DEBUG(logger, msg)
#define LOG_D(msg)				LOG4CXX_DEBUG(logger, msg)

#define LOG_ERROR(logger, msg)	LOG4CXX_ERROR(logger, msg)
#define LOG_E(msg)				LOG4CXX_ERROR(logger, msg)

#define LOG_FATAL(logger, msg)	LOG4CXX_FATAL(logger, msg)
#define LOG_F(msg)				LOG4CXX_FATAL(logger, msg)

#define LOG_INFO(logger, msg)	LOG4CXX_INFO(logger, msg)
#define LOG_I(msg)				LOG4CXX_INFO(logger, msg)

#define LOG_WARN(logger, msg)	LOG4CXX_WARN(logger, msg)
#define LOG_W(msg)				LOG4CXX_WARN(logger, msg)

void set_logfile_name(std::string name);

#endif
