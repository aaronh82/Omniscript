//
//  Logger.h
//  OmniScript
//
//  Created by Aaron Houghton on 7/29/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__Logger__
#define __OmniScript__Logger__

#include <iostream>
#include <string>

#define LOG(level, msg) Log::Logger::get_logger().log(level, msg)

namespace log4cpp {
	class Category;
}

namespace Log {
	
	enum LogLevel {
		EMERG,		// System is unusable (e.g. multiple parts down)
 		ALERT,		// System is unusable (e.g. single part down)
		CRIT,		// Failure in non-primary system (e.g. backup site down)
		ERROR,		// Non-urgent failures; relay to developers
		WARN,		// Not an error, but indicates error will occur if nothing done.
		NOTICE,		// Events that are unusual, but not error conditions.
		INFO,		// Normal operational messages. No action required.
		DEBUGGING,	// Information useful during development for debugging.
		NOTSET
	};

	class Logger final {
		Logger();
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) = delete;
		
		log4cpp::Category& m_cpp_logger;
		
	public:
		static Logger& get_logger();
		void log(Log::LogLevel level, const std::string& msg);
	};
}

#endif /* defined(__OmniScript__Logger__) */
