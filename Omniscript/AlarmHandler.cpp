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
		
		if (res.find("true") != std::string::npos) {
			return true;
		}
		return false;
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
								"key=" + alarm->id() + " "
								"level=\"" + alarm->level() + "\" "
								"name=\"" + alarm->name() + "\" "
								"text=\"" + alarm->description() + "\"";
		if (!alarmExists(*alarm) && alarm->write(cmd)) {
			alarm_queue.push_back(alarm);
		}
	}
	
	void AlarmHandler::clear(const std::string &key) {
		const std::string cmd = "/usr/bin/php -f "
								"/var/www/maverick/app/public_htdocs/api.php "
								"target=alarm action=clear "
								"key=" + key;
		execAlarm(cmd);
	}
	
	void AlarmHandler::cleanUp() {
		for (auto alarm = alarm_queue.begin(); alarm != alarm_queue.end(); ++alarm) {
			const std::string query = "SELECT * FROM alarm WHERE alarmkey="
									  + (*alarm)->id();
			std::shared_ptr<sql::ResultSet> res(DB_READ(query));
			if (res->rowsCount() != 0) {
				alarm_queue.erase(alarm);
			}
		}
	}
	
#pragma Alarm Class Definitions
	
	Alarm::Alarm(const std::string &id,
				 const std::string &level,
				 const std::string &name,
				 const std::string &description):
	key_(id),
	level_(level),
	name_(name),
	description_(description)
	{
		std::time(&date_created_);
//		std::hash<std::string> str_hash;
//		key_ = str_hash(name) + date_created_;
	}
	
	bool Alarm::write(const std::string &cmd) {
		return execAlarm(cmd);
//		bool success = false;
//		if (!written_) {
//			const std::string query = "INSERT INTO alarminstance (alarmkey, "
//			"alarmtypeid, "
//			"alarmstatusid, "
//			"alarmlevelid, "
//			"name, "
//			"description, "
//			"datecreated) "
//			"SELECT " + key_ + ", "
//			"alarmtype.alarmtypeid, "
//			"1, "
//			"alarmlevel.alarmlevelid, "
//			+ name_ + ", "
//			+ description_ + ", "
//			+ std::to_string(date_created_) + " "
//			"FROM alarmtype "
//			"WHERE alarmtype.name='" + type_ + "' "
//			"AND alarmlevel.name='" + type_ + "'";
//			success = DB_WRITE(query);
//			written_ = true;
//		}
//		return success;
	}
	
	PointAlarm::PointAlarm(const std::string &id,
						   const std::string &level,
						   const std::string &name,
						   const std::string &description,
						   const Point &point):
	Alarm(id, level, name, description),
	point_(point) {}
	
	bool PointAlarm::write(const std::string &cmd) {
		return execAlarm(cmd);
//		if (!written_) {
//			const std::string query = "INSERT INTO alarminstance (alarmkey, "
//			"alarmtypeid, "
//			"alarmstatusid, "
//			"alarmlevelid, "
//			"name, "
//			"description, "
//			"pathid, "
//			"deviceid, "
//			"pointid, "
//			"datecreated) "
//			"SELECT " + key_ + ", "
//			"alarmtype.alarmtypeid, "
//			"1, "
//			"alarmlevel.alarmlevelid, "
//			+ name_ + ", "
//			+ description_ + ", "
//			+ std::to_string(point_.pathId()) + ", "
//			+ std::to_string(point_.deviceId()) + ", "
//			+ std::to_string(point_.pointId()) + ", "
//			+ std::to_string(date_created_) + " "
//			"FROM alarmtype "
//			"WHERE alarmtype.name='" + type_ + "' "
//			"AND alarmlevel.name='" + type_ + "'";
//			success = DB_WRITE(query);
//			written_ = true;
//		}
//		return success;
	}
	
	bool operator==(const Alarm &lhs, const Alarm &rhs) {
		return (lhs.key_ == rhs.key_ && lhs.level_ == rhs.level_ &&
				lhs.name_ == rhs.name_ && lhs.description_ == rhs.description_);
	}
}