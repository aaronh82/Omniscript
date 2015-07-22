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
	
	void highTempLimit::operator()(const block_ptr &b, Interpreter &interp) {
		for (auto arg : b->args()) {
			LOG(Log::DEBUGGING, arg);
			if (arg.find("block:") != std::string::npos) {
				LOG(Log::DEBUGGING, "\t" + b->blockArgs()[0]->opcode());
			}
		}
		const auto limit = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
						stof(b->args()[0]);
		const auto i_index = stoi(b->args()[1].substr(b->args()[1].find(":") + 1));
		const auto input = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[1]);
		std::string level = b->args()[2];
		if (input > limit) {
			const std::string name = "Space temperature("
									 + std::to_string(input) + "F) of "
									 + b->blockArgs()[i_index]->args()[0] +
									 " is above setpoint("
									 + std::to_string(limit) + "F)";
			const auto p_index = std::find_if(interp.points().begin(),
											  interp.points().end(),
											  findVariable(b->blockArgs()[i_index]->args()[0]));
			AlarmHandler::getHandler().add(
					std::make_shared<PointAlarm>(level, name, name, **p_index));
		}
		LOG(Log::DEBUGGING, "highTempLimit");
	}
	
	void lowTempLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const auto limit = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
						stof(b->args()[0]);
		const auto i_index = stoi(b->args()[1].substr(b->args()[1].find(":") + 1));
		const auto input = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[1]);
		std::string level = b->args()[2];
		if (input < limit) {
			const std::string name = "Space temperature("
									 + std::to_string(input) + "F) of "
									 + b->blockArgs()[i_index]->args()[0] +
									 " is above setpoint("
									 + std::to_string(limit) + "F)";
			const auto p_index = std::find_if(interp.points().begin(),
											  interp.points().end(),
											  findVariable(b->blockArgs()[i_index]->args()[0]));
			AlarmHandler::getHandler().add(
					std::make_shared<PointAlarm>(level, name, name, **p_index));
		}
		LOG(Log::DEBUGGING, "lowTempLimit");
	}
	
	void customAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		std::string level = b->args()[0];
		std::string msg = b->args()[1];
		std::vector<block_ptr> ba(b->blockArgs());
		std::queue<block_ptr, std::deque<block_ptr> >
			inputs(std::deque<block_ptr>(ba.begin(),
										 ba.end()));
		char fmt;
		std::string tmp;
		std::string::iterator pos;
		for (std::string::iterator it = msg.begin(); it != msg.end(); ++it) {
			if (*it == '%') {
				pos = it;
				fmt = *(++it);
				
				switch (fmt) {
					case 'v': {
						block_ptr tmp_blk = inputs.front();
						tmp = std::to_string(interp.execute(tmp_blk));
						break;
					}
					case 's':
						tmp = inputs.front()->args()[0];
						break;
						
					default:
						break;
				}
				inputs.pop();
				auto d = std::distance(msg.begin(), it);
				d += (tmp.length() - 1);
				msg.replace(pos, pos + 2, tmp);
				it = msg.begin() + d;
			}
		}
		AlarmHandler::getHandler().add(std::make_shared<Alarm>(level, msg,msg));
		LOG(Log::DEBUGGING, "customAlarm");
	}
	
}