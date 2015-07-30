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
		if (!alarmExists(*alarm) && alarm->write()) {
			alarm_queue.push_back(alarm);
		}
	}
	
	void AlarmHandler::cleanUp() {
		for (auto alarm = alarm_queue.begin(); alarm != alarm_queue.end(); ++alarm) {
			const std::string query = "SELECT * FROM alarm WHERE alarmkey="
									  + (*alarm)->key();
			std::shared_ptr<sql::ResultSet> res(DB_READ(query));
			if (res->rowsCount() != 0) {
				alarm_queue.erase(alarm);
			}
		}
	}
	
#pragma Alarm Class Definitions
	
	Alarm::Alarm(const std::string &type,
				 const std::string &name,
				 const std::string &description):
	type_(type),
	name_(name),
	description_(description),
	written_(false)
	{
		std::time(&date_created_);
		std::hash<std::string> str_hash;
		key_ = str_hash(name) + date_created_;
	}
	
	bool Alarm::write() {
		bool success = false;
		if (!written_) {
			const std::string query = "INSERT INTO alarminstance (alarmkey, "
			"alarmtypeid, "
			"alarmstatusid, "
			"alarmlevelid, "
			"name, "
			"description, "
			"datecreated) "
			"SELECT " + key_ + ", "
			"alarmtype.alarmtypeid, "
			"1, "
			"alarmlevel.alarmlevelid, "
			+ name_ + ", "
			+ description_ + ", "
			+ std::to_string(date_created_) + " "
			"FROM alarmtype "
			"WHERE alarmtype.name='" + type_ + "' "
			"AND alarmlevel.name='" + type_ + "'";
			success = DB_WRITE(query);
			written_ = true;
		}
		return success;
	}
	
	std::string Alarm::key() {
		return key_;
	}
	
	PointAlarm::PointAlarm(const std::string &type,
						   const std::string &name,
						   const std::string &description,
						   const Point &point):
	Alarm(type, name, description),
	point_(point) {}
	
	bool PointAlarm::write() {
		bool success = false;
		if (!written_) {
			const std::string query = "INSERT INTO alarminstance (alarmkey, "
			"alarmtypeid, "
			"alarmstatusid, "
			"alarmlevelid, "
			"name, "
			"description, "
			"pathid, "
			"deviceid, "
			"pointid, "
			"datecreated) "
			"SELECT " + key_ + ", "
			"alarmtype.alarmtypeid, "
			"1, "
			"alarmlevel.alarmlevelid, "
			+ name_ + ", "
			+ description_ + ", "
			+ std::to_string(point_.pathId()) + ", "
			+ std::to_string(point_.deviceId()) + ", "
			+ std::to_string(point_.pointId()) + ", "
			+ std::to_string(date_created_) + " "
			"FROM alarmtype "
			"WHERE alarmtype.name='" + type_ + "' "
			"AND alarmlevel.name='" + type_ + "'";
			success = DB_WRITE(query);
			written_ = true;
		}
		return success;
	}
	
	bool operator==(const Alarm &lhs, const Alarm &rhs) {
		return (lhs.key_ == rhs.key_ && lhs.type_ == rhs.type_ &&
				lhs.name_ == rhs.name_ && lhs.description_ == rhs.description_);
	}
}