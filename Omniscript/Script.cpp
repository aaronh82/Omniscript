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
		variables_ = util::JSON::getVars(data);
		points_ = util::JSON::getPoints(data);
		time_t curTime = convertTime(lastModified);
		if (curTime > lastModified_) {
			lastModified_ = curTime;
		}
	}
	
	time_t Script::convertTime(std::string sql_time) {
		std::string date = getDate(sql_time);
		std::string time = getTime(sql_time);
		
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
	std::string Script::name(std::string n) {
		if (!n.empty()) {
			name_ = n;
		}
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
	
	std::vector<std::shared_ptr<interp::Variable> > Script::variables() const {
		return variables_;
	}
	
	std::vector<std::shared_ptr<interp::Point> > Script::points() const {
		return points_;
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
	
	bool Script::updated(std::string sql_time) {
		time_t new_time = convertTime(sql_time);
		if (new_time > lastModified_) {
			lastModified_ = new_time;
			return true;
		}
		return false;
	}
	
	void Script::updateScript(std::istream *data) {
		startingBlocks_ = util::JSON::build(data);
		variables_ = util::JSON::getVars(data);
		points_ = util::JSON::getPoints(data);
	}
	
	bool Script::needsRestart() {
		return restart_;
	}
}