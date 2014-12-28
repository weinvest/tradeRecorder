#ifndef _DFC_LOG_H
#define _DFC_LOG_H

#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
using namespace log4cplus;
using namespace log4cplus::helpers;

#define LoadLogConf(cfg)  PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(cfg));

#define TRACE(logger,msg) LOG4CPLUS_TRACE(logger,msg)
#define INFO(logger,msg) LOG4CPLUS_INFO(logger,msg)
#define DEBUG(logger,msg) LOG4CPLUS_DEBUG(logger,msg)
#define WARN(logger,msg)  LOG4CPLUS_WARN(logger,msg)
#define LOG_ERROR(logger,msg) LOG4CPLUS_ERROR(logger,msg)
#define FATAL(logger,msg) LOG4CPLUS_FATAL(logger,msg)

#endif
 
