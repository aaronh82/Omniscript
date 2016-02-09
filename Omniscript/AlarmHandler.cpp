//
//  AlarmHandler.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 10/31/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "AlarmHandler.h"
#include "DBConnection.h"

#include <functional>
#include <algorithm>
#include <typeinfo>

namespace interp {
	bool execAlarm(const std::string& cmd) {
		FILE *pipe = popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(Log::ERROR, "Failed to execute '" + std::string(cmd) + "'");
		}
		char buffer[128];
		std::string res = "";
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL) {
				res += buffer;
			}
		}
		pclose(pipe);

		return res.find("true") != std::string::npos;
//		if (res.find("true") != std::string::npos) {
//			return true;
//		}
//		return false;
	}
	
	AlarmHandler::AlarmHandler() {
		
	}
	
	bool AlarmHandler::alarmExists(const Alarm &alarm) {
		auto findAlarm = [alarm](std::shared_ptr<Alarm> queued){
			return alarm == *queued;
		};
		return std::find_if(alarm_queue.begin(), alarm_queue.end(), findAlarm) != alarm_queue.end();
	}
	
	AlarmHandler& AlarmHandler::getHandler() {
		static AlarmHandler ah;
		return ah;
	}
	
	void AlarmHandler::add(std::shared_ptr<Alarm> alarm) {
		const std::string cmd = "/usr/bin/php -f "
								"/var/www/maverick/app/public_htdocs/api.php "
								"target=alarm action=set "
								"alias=" + alarm->alias() + " "
								"deviceid=" + alarm->deviceId() + " "
								"level=\"" + alarm->level() + "\" "
								"name=\"" + alarm->name() + "\" "
								"text=\"" + alarm->description() + "\"";
		cleanUp();
		if (!alarmExists(*alarm) && alarm->write(cmd)) {
			alarm_queue.push_back(alarm);
		}
	}
	
	void AlarmHandler::clear(const std::string &alias, const std::string &deviceId) {
		const std::string cmd = "/usr/bin/php -f "
								"/var/www/maverick/app/public_htdocs/api.php "
								"target=alarm action=clear "
								"alias=" + alias + " "
								"deviceid=" + deviceId;
		execAlarm(cmd);
	}
	
	void AlarmHandler::cleanUp() {
		for (auto alarm = alarm_queue.begin(); alarm != alarm_queue.end(); ++alarm) {
			const std::string query = "SELECT * FROM alarminstance "
									  "WHERE alarmkey=" + (*alarm)->alias() + " " +
			                          "AND description=\"" +
			                          (*alarm)->description() + "\"";
			std::shared_ptr<sql::ResultSet> res(DB_READ(query));
			if (res->rowsCount() != 0) {
				alarm_queue.erase(alarm);
			}
		}
	}
	
#pragma Alarm Class Definitions
	
	Alarm::Alarm(const std::string &alias,
				 const std::string &level,
				 const std::string &name,
				 const std::string &description,
				 const Point &point):
	alias_(alias),
	level_(level),
	name_(name),
	description_(description),
	point_(point)
	{
		std::time(&date_created_);
//		std::hash<std::string> str_hash;
//		alias_ = str_hash(name) + date_created_;
	}
	
	bool Alarm::write(const std::string &cmd) {
		return execAlarm(cmd);
	}
	
//	PointAlarm::PointAlarm(const std::string &alias,
//						   const std::string &level,
//						   const std::string &name,
//						   const std::string &description,
//						   const Point &point):
//	Alarm(alias, level, name, description),
//	point_(point) {}
//
//	bool PointAlarm::write(const std::string &cmd) {
//		return execAlarm(cmd);
//	}
	
	bool operator==(const Alarm &lhs, const Alarm &rhs) {
		return (lhs.alias_ == rhs.alias_ && lhs.level_ == rhs.level_ &&
			lhs.name_ == rhs.name_); // && lhs.description_ == rhs.description_);
	}

//	bool operator==(const PointAlarm &lhs, const PointAlarm &rhs) {
//		return (lhs.alias_ == rhs.alias_ && lhs.level_ == rhs.level_ &&
//			lhs.name_ == rhs.name_ && lhs.point_ == rhs.point_);
//	}
}