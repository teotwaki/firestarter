#ifndef __LOGGER_H
#define __LOGGER_H

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <string>
#include <cstdlib>
#include <cstring>

#ifndef IN_UNIT_TESTING
	#define DECLARE_LOG(var, name) \
		log4cxx::LoggerPtr var(log4cxx::Logger::getLogger(name))

	#define DECLARE_EXTERN_LOG(var) \
		extern log4cxx::LoggerPtr var

	#define DECLARE_MODULE_LOG(name) \
		namespace firestarter { namespace module { \
			DECLARE_LOG(logger, "module.core." #name); \
			namespace core { namespace name { \
				DECLARE_LOG(logger, "module.core." #name); \
		} } } }
	
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

#else
	#define DECLARE_LOG(var, name)
	#define DECLARE_EXTERN_LOG(var)
	
	#define LOG_TRACE(logger, msg)
	#define LOG_T(msg)
	
	#define LOG_DEBUG(logger, msg)
	#define LOG_D(msg)
	
	#define LOG_ERROR(logger, msg)
	#define LOG_E(msg)
	
	#define LOG_FATAL(logger, msg)
	#define LOG_F(msg)
	
	#define LOG_INFO(logger, msg)
	#define LOG_I(msg)
	
	#define LOG_WARN(logger, msg)
	#define LOG_W(msg)
#endif
void set_logfile_name(std::string name);

#endif
