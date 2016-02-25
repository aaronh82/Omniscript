//
//  AlarmHandler.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 10/31/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "AlarmHandler.h"
#include "DBConnection.h"
#include "Interpreter.h"

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
	
	AlarmHandler::AlarmHandler(): dist(0.0, std::numeric_limits<double>::max()) {
		std::random_device rd;
		mt.seed(rd());

		// TODO: Gather any existing alarms from the database to avoid repeat instances
		const std::string query("SELECT "
				                        "alarm.alarmid, "
				                        "alarm.alias AS 'alias', "
				                        "alarmlevel.name AS 'level', "
										"alarminstance.name AS 'name', "
				                        "alarminstance.description AS 'description', "
				                        "alarminstance.pointid AS 'pointid', "
				                        "alarminstance.deviceid AS 'deviceid', "
				                        "alarminstance.token AS 'token' "
								"FROM "
				                        "alarm "
								"INNER JOIN "
				                        "alarminstance "
								"ON "
				                        "alarminstance.alarmid=alarm.alarmid "
								"INNER JOIN "
										"alarmlevel "
								"ON "
				                        "alarmlevel.alarmlevelid=alarminstance.devicealarmlevelid "
								"WHERE alarm.alias LIKE \"omni%\"");
		std::shared_ptr<sql::ResultSet> res(DB_READ(query));
		while (res->next()) {
			auto alarm = std::make_shared<Alarm>(res->getString("alias"),
			                        res->getString("level"),
			                        res->getString("name"),
			                        res->getString("description"),
			                        res->getUInt("pointid"),
			                        res->getUInt("deviceid"));
			alarm->setToken(res->getString("token"));
			alarm_queue.push_back(alarm);
		}
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
//		auto alarm = std::make_shared<Alarm>(alias, level, name, description, point);
		alarm->createHash(std::to_string(dist(mt)));
		const std::string cmd = "/usr/bin/php -f "
								"/var/www/maverick/app/public_htdocs/api.php "
								"target=alarm action=set "
								"alias=" + alarm->alias() + " "
								"deviceid=" + alarm->deviceId() + " "
								"pointid=" + alarm->pointId() + " "
								"level=\"" + alarm->level() + "\" "
								"name=\"" + alarm->name() + "\" "
								"description=\"" + alarm->description() + "\" "
								"token=\"" + alarm->getHash().hexdigest() + "\"";
//		cleanUp();
		if (!alarmExists(*alarm) && alarm->write(cmd)) {
			alarm_queue.push_back(alarm);
		}
	}
	
	void AlarmHandler::clear(const std::string &alias,
	                         const std::string &device_id,
	                         const std::string &point_id,
	                         const std::string &level) {
		auto alarm_it = std::find_if(alarm_queue.begin(), alarm_queue.end(),
		             [&] (std::shared_ptr<Alarm> alarm) ->bool {
			             return (alarm->alias() == alias) &&
					             (alarm->deviceId() == device_id) &&
					             (alarm->pointId() == point_id) &&
					             (alarm->level() == level);
		});
		if (alarm_it != alarm_queue.end()) {
			const std::string cmd = "/usr/bin/php -f "
					                        "/var/www/maverick/app/public_htdocs/api.php "
					                        "target=alarm action=clear "
					                        "token=" + (*alarm_it)->getToken();
			execAlarm(cmd);
			alarm_queue.erase(alarm_it);
		}
	}
	
	void AlarmHandler::cleanUp() {
		for (auto alarm = alarm_queue.begin(); alarm != alarm_queue.end(); ++alarm) {
			const std::string query = "SELECT * FROM alarminstance "
									  "WHERE name=" + (*alarm)->name() + " "
			                          "AND deviceid=" + (*alarm)->deviceId() + " "
									  "AND pointid=" + (*alarm)->pointId();
			std::shared_ptr<sql::ResultSet> res(DB_READ(query));
			if (res->rowsCount() != 0) {
				alarm_queue.erase(alarm);
			}
		}
	}
	
#pragma mark Alarm Class Definitions
	
	Alarm::Alarm(const std::string &alias,
				 const std::string &level,
				 const std::string &name,
				 const std::string &description,
				 const unsigned int point_id,
				 const unsigned int device_id):
	alias_(alias),
	level_(level),
	name_(name),
	description_(description),
	point_id_(point_id),
	device_id_(device_id)
	{
		std::time(&date_created_);
//		std::hash<std::string> str_hash;
//		alias_ = str_hash(name) + date_created_;
	}

	void Alarm::createHash(const std::string &rn) {
		hash_.update(rn.c_str(), rn.size());
		hash_.finalize();
		token_ = hash_.hexdigest();
		LOG(Log::LogLevel::DEBUGGING, "Alarm hash: " + hash_.hexdigest());
	}

	void Alarm::setToken(const std::string &token) {
		token_ = token;
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
		        lhs.name_ == rhs.name_); // &&
//		        lhs.hash_.hexdigest() == rhs.hash_.hexdigest());
	}

//	bool operator==(const PointAlarm &lhs, const PointAlarm &rhs) {
//		return (lhs.alias_ == rhs.alias_ && lhs.level_ == rhs.level_ &&
//			lhs.name_ == rhs.name_ && lhs.point_ == rhs.point_);
//	}

}