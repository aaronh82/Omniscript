//
//  Logger.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 7/29/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
//#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

#include "Logger.h"

namespace Log {
	const size_t maxFileSize = 500 * 1024;
	const unsigned int maxBackupIndex = 9;
	
	Logger::Logger(): m_cpp_logger(log4cpp::Category::getRoot()) {
		log4cpp::Appender* p_appender = new log4cpp::RollingFileAppender("default",
										 "/var/log/omniscript.log",
										 maxFileSize,
										 maxBackupIndex);
		log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
		layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} [%p] %c: %m%n");
		p_appender->setLayout(layout);
		
		m_cpp_logger.setPriority(log4cpp::Priority::INFO);
		m_cpp_logger.addAppender(p_appender);
	}
	
	Logger& Logger::get_logger() {
		static Logger logger;
		return logger;
	}
	
	void Logger::log(LogLevel level, const std::string& msg) {
		log4cpp::Priority::PriorityLevel cpp_level = log4cpp::Priority::NOTSET;
		switch (level) {
			case NOTSET:
			case EMERG:		cpp_level = log4cpp::Priority::EMERG; break;
			case ALERT:		cpp_level = log4cpp::Priority::ALERT; break;
			case CRIT:		cpp_level = log4cpp::Priority::CRIT; break;
			case ERROR:		cpp_level = log4cpp::Priority::ERROR; break;
			case WARN:		cpp_level = log4cpp::Priority::WARN; break;
			case NOTICE:	cpp_level = log4cpp::Priority::NOTICE; break;
			case INFO:		cpp_level = log4cpp::Priority::INFO; break;
			case DEBUGGING:	cpp_level = log4cpp::Priority::DEBUG; break;
				
			default:
				assert(false);
		}
		assert(cpp_level != log4cpp::Priority::NOTSET);
		
		m_cpp_logger << cpp_level << msg;
	}
}
