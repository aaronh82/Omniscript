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
#include "AlarmHandler.h"
#include "Variables.h"

#include <string>
#include <queue>
#include <vector>
#include <algorithm>

namespace interp {
#pragma Alarm Type Definitions
	
	void highLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "345";
//		for (auto arg : b->args()) {
//			LOG(Log::DEBUGGING, arg);
//			if (arg.find("block:") != std::string::npos) {
//				LOG(Log::DEBUGGING, "\t" + b->blockArgs()[0]->opcode());
//			}
//		}
		const auto limit = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
						stof(b->args()[1]);
		const auto i_index = stoi(b->args()[2].substr(b->args()[2].find(":") + 1));
		const auto input = (b->args()[2].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[2]);
		std::string level = b->args()[0];
//		const auto point = std::find_if(interp.points().begin(),
//										interp.points().end(),
//										findVariable(b->args()[2]));
		if (input > limit) {
			const std::string name = "High Limit";
			const auto point = std::find_if(interp.points().begin(),
											  interp.points().end(),
											  findVariable(b->blockArgs()[i_index]->args()[0]));
			const std::string description = "" + (*point)->name() + "("
									 + std::to_string(input) + "F) of "
									 + (*point)->deviceName() +
//									 + b->blockArgs()[i_index]->args()[0] +
									 " is above limit("
									 + std::to_string(limit) + "F)";
//			const std::string cmd = "/usr/bin/php -f "
//									"/var/www/maverick/app/public_htdocs/api.php "
//									"target=alarm action=set "
//									"key=" + alarmid + " "
//									"text=" + description;
			AlarmHandler::getHandler().add(
					std::make_shared<PointAlarm>(alarmid, level, name, description, **point));
		} else {
			AlarmHandler::getHandler().clear(alarmid);
		}
		LOG(Log::DEBUGGING, "highTempLimit");
	}
	
	void lowLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "346";
		const auto limit = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
						stof(b->args()[1]);
		const auto i_index = stoi(b->args()[2].substr(b->args()[2].find(":") + 1));
		const auto input = (b->args()[2].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[2]);
		const auto level = b->args()[0];
//		const std::vector<std::shared_ptr<Point> >::const_iterator point =
//		const auto point = std::find_if(interp.points().begin(),
//										interp.points().end(),
//										findVariable(b->args()[2]));
		if (input < limit) {
			const std::string name = "Low Limit";
			const auto point = std::find_if(interp.points().begin(),
											interp.points().end(),
											findVariable(b->blockArgs()[i_index]->args()[0]));
			const std::string description = (*point)->name() + "("
									 + std::to_string(input) + "F) of "
									 + (*point)->deviceName() +
//									 + b->blockArgs()[i_index]->args()[0] +
									 " is below limit("
									 + std::to_string(limit) + "F)";
			AlarmHandler::getHandler().add(
					std::make_shared<PointAlarm>(alarmid, level, name, description, **point));
		} else {
			AlarmHandler::getHandler().clear(alarmid);
		}
		LOG(Log::DEBUGGING, "lowTempLimit");
	}
	
	void runtimeLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const auto alarmid = "347";
		const auto limit = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
						stof(b->args()[1]);
		const auto i_index = stoi(b->args()[0].substr(b->args()[0].find(":") + 1));
		const auto input = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[0]);
		const auto onValue =(b->args()[2].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[2]);
		auto level = b->args()[3];
	}
	
	void statusAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "348";
	}
	
	void customAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "349";
		const std::string name = "Custom Alarm";
		std::string level = b->args()[0];
		std::string msg = b->args()[1];
		std::vector<block_ptr> ba(b->blockArgs());
		std::queue<block_ptr, std::deque<block_ptr> >
			inputs(std::deque<block_ptr>(ba.begin(),
										 ba.end()));
		std::string fmt;
		std::string tmp;
		std::string::iterator pos;
		for (std::string::iterator it = msg.begin(); it != msg.end(); ++it) {
			if (*it == '%') {
				pos = it;
				fmt = *(++it);
				
				if (fmt == "value") {
					block_ptr tmp_blk = inputs.front();
					tmp = std::to_string(interp.execute(tmp_blk));
					break;
				} else if (fmt == "name") {
					tmp = inputs.front()->args()[0];
					break;
				}

				inputs.pop();
				auto d = std::distance(msg.begin(), it);
				d += (tmp.length() - 1);
				msg.replace(pos, pos + 2, tmp);
				it = msg.begin() + d;
			}
		}
		AlarmHandler::getHandler().add(std::make_shared<Alarm>(alarmid, level, name, msg));
		LOG(Log::DEBUGGING, "customAlarm");
	}
	
}