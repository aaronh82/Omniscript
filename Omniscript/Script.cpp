//
//  Script.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 8/26/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Script.h"
#include "Logger.h"
#include "JSON.h"

namespace script {
	Script::Script(std::string name, uint id, std::string lastModified, std::istream *data, bool enabled):
					name_(name), id_(id), enabled_(enabled) {
		startingBlocks_ = util::JSON::build(data);
		time_t curTime = convertTime(lastModified);
		if (curTime > lastModified_) {
			lastModified_ = curTime;
		}
	}
	
	time_t Script::convertTime(std::string sqlTime) {
		std::string date = getDate(sqlTime);
		std::string time = getTime(sqlTime);
		
		std::unique_ptr<tm> time_info(new tm);
		
		time_info->tm_year = getYear(date);
		time_info->tm_mon = getMonth(date);
		time_info->tm_mday = getDay(date);
		time_info->tm_hour = getHours(time);
		time_info->tm_min = getMinutes(time);
		time_info->tm_sec = getSeconds(time);
		time_info->tm_isdst = 0;
		
		return mktime(time_info.get());
	}
	
	std::string Script::getDate(std::string s) {
		return s.substr(0, s.find(" "));
	}
	
	std::string Script::getTime(std::string s) {
		return s.substr(s.rfind(" ") + 1);
	}
	
	int Script::getYear(std::string s) {
		return std::stoi(s.substr(0, s.find("-"))) - 1900;
	}
	
	int Script::getMonth(std::string s) {
		size_t start = s.find("-") + 1;
		return std::stoi(s.substr(start, s.find("-", start) - start)) - 1;
	}
	
	int Script::getDay(std::string s) {
		size_t first = s.find("-") + 1;
		size_t start = s.find("-", first) + 1;
		return std::stoi(s.substr(start, s.find("-", start)));
	}
	
	int Script::getHours(std::string s) {
		return std::stoi(s.substr(0, s.find(":")));
	}
	
	int Script::getMinutes(std::string s) {
		size_t start = s.find(":") + 1;
		return std::stoi(s.substr(start, s.find(":", start) - start));
	}
	
	int Script::getSeconds(std::string s) {
		size_t first = s.find(":") + 1;
		size_t start = s.find(":", first) + 1;
		return std::stoi(s.substr(start, s.find(":", start)));
	}
	
	// Public
	std::string Script::name() {
		return name_;
	}
	
	std::string Script::name() const {
		return name_;
	}
	
	uint Script::id() {
		return id_;
	}
	
	uint Script::id() const {
		return id_;
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
	
	const std::vector<block_ptr> Script::startingBlocks() const {
		return startingBlocks_;
	}
}