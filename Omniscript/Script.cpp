//
//  Script.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 8/26/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Script.h"
#include "Logger.h"

namespace script {
	
	Script::Script(std::string name, unsigned int id, std::string lastModified,			std::istream *data, bool enabled):
				name_(name), id_(id), enabled_(enabled), restart_(false) {
	
		startingBlocks_ = util::XML::build(data, *this);
		std::time_t curTime = convertTime(lastModified);
			
		if (curTime > lastModified_) {
			lastModified_ = curTime;
		}
	}

	std::time_t Script::convertTime(const std::string& sql_time) {
		struct tm time_info;
		strptime(sql_time.c_str(), "%Y-%m-%d %H:%M:%S", &time_info);
		time_info.tm_isdst = -1;
		return mktime(&time_info);
	}
	
	// Public
	std::string Script::name(std::string n) {
		if (!n.empty()) {
			name_ = n;
		}
		return name_;
	}
	
	std::string Script::name() const {
		return name_;
	}
	
	unsigned int Script::id() {
		return id_;
	}
	
	unsigned int Script::id() const {
		return id_;
	}
	
	const std::vector<var_ptr>& Script::variables() {
		return variables_;
	}
	
	const std::vector<point_ptr>& Script::points() {
		return points_;
	}
	
	std::vector<dev_ptr>& Script::devices() {
		return devices_;
	}
	
	void Script::enable() {
		enabled_ = true;
	}
	
	void Script::disable() {
		enabled_ = false;
	}
	
	bool Script::isEnabled() {
		return enabled_;
	}
	
	bool Script::isEnabled() const {
		return enabled_;
	}
	
	std::map<int, block_ptr> Script::startingBlocks() {
		return startingBlocks_;
	}
	
	bool Script::updated(std::string sql_time) {
		std::time_t new_time = convertTime(sql_time);
		if (new_time > lastModified_) {
			lastModified_ = new_time;
			return true;
		}
		return false;
	}
	
	void Script::updateScript(std::istream *data) {
		startingBlocks_ = util::XML::build(data, *this);
	}
	
	bool Script::needsRestart() {
		return restart_;
	}
}