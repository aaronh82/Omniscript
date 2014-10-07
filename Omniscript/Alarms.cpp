//
//  Alarms.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Alarms.h"
#include "Interpreter.h"
#include "DBConnection.h"

namespace interp {
	
	void highTempLimit::operator()(const block_ptr &b, Interpreter &interp) {
		if (b->args().size() != 3) {
			throw "The wrong number of arguments was provided to 'highTempLimit'";
		}
		for (auto arg : b->args()) {
			LOG(Log::DEBUGGING, arg);
			if (arg.find("block:") != std::string::npos) {
				LOG(Log::DEBUGGING, "\t" + b->blockArgs()[0]->opcode());
			}
		}
		float limit = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
						stof(b->args()[0]);
		float input = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
						stof(b->args()[1]);
		std::string level = b->args()[2];
		if (input > limit) {
			const std::string query = "INSERT INTO alarm (alarmtypeid, name) "
									  "SELECT alarmtype.alarmtypeid, 'High Temp' "
									  "FROM alarmtype WHERE alarmtype.name='" + level + "'";
			DB_WRITE(query);
		}
		LOG(Log::DEBUGGING, "highTempLimit");
	}
	
	void lowTempLimit::operator()(const block_ptr &b, Interpreter &interp) {
		if (b->args().size() != 3) {
			throw "The wrong number of arguments was provided to 'lowTempLimit'";
		}
		float limit = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
						stof(b->args()[0]);
		float input = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
						stof(b->args()[1]);
		std::string level = b->args()[2];
		if (input < limit) {
			const std::string query = "INSERT INTO alarm (alarmtypeid, name) "
									  "SELECT alarmtype.alarmtypeid, 'High Temp' "
									  "FROM alarmtype WHERE alarmtype.name='" + level + "'";
			DB_WRITE(query);
		}
		LOG(Log::DEBUGGING, "lowTempLimit");
	}
}