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
#include <ctime>

namespace interp {
#pragma Alarm Type Definitions
	
	void highLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "345";
		const auto limit = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
						stof(b->args()[1]);
		const auto i_index = stoi(b->args()[0].substr(b->args()[0].find(":") + 1));
		const auto input = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[0]);
		std::string level = b->args()[2];

		if (input > limit) {
			const std::string name = "High Limit";
			const auto point = std::find_if(interp.points().begin(),
											  interp.points().end(),
											  findVariable(b->blockArgs()[i_index]->args()[0]));
			const std::string description = "" + (*point)->name() + "("
									 + std::to_string(input) + "F) of "
									 + (*point)->deviceName() +
									 " is above limit("
									 + std::to_string(limit) + "F)";
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
		const auto i_index = stoi(b->args()[0].substr(b->args()[0].find(":") + 1));
		const auto input = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[0]);
		const auto level = b->args()[2];

		if (input < limit) {
			const std::string name = "Low Limit";
			const auto point = std::find_if(interp.points().begin(),
											interp.points().end(),
											findVariable(b->blockArgs()[i_index]->args()[0]));
			const std::string description = (*point)->name() + "("
									 + std::to_string(input) + "F) of "
									 + (*point)->deviceName() +
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
		const auto name = "Runtime Limit";
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
		const auto point = std::find_if(interp.points().begin(),
						interp.points().end(),
						findVariable(b->blockArgs()[i_index]->args()[0]));
		if ((input == onValue) && ((*point)->lastPoll() - (*point)->lastCOV()) > limit) {
			const auto description = (*point)->name() +
				" has been running longer than " +
				std::to_string(limit) + " seconds.";
			AlarmHandler::getHandler().add(
				std::make_shared<PointAlarm>(alarmid, level, name, description, **point));
		}
	}
	
	void statusAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "348";
		const auto name = "Status Alarm";
		const auto expected = (b->args()[1].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
			stof(b->args()[1]);
		const auto i_index = stoi(b->args()[0].substr(b->args()[0].find(":") + 1));
		const auto input = (b->args()[0].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[i_index]) :
			stof(b->args()[0]);
		const auto delay = (b->args()[2].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[i_index]) :
			stof(b->args()[2]);
		auto level = b->args()[3];
		const auto point = std::find_if(interp.points().begin(),
			interp.points().end(),
			findVariable(b->blockArgs()[i_index]->args()[0]));
		if ((input != expected) && ((std::time(nullptr) - (*point)->lastCOV()) > delay)) {
			const auto description = (*point)->name() +
				" hasn't changed state in more than " +
				std::to_string(expected) + " seconds.";
			AlarmHandler::getHandler().add(
				std::make_shared<PointAlarm>(alarmid, level, name, description, **point));
		}
	}

	void staleValueAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "349";
		const std::string name = "Stale Value Alarm";
		const auto i_index = stoi(b->args()[0].substr(b->args()[0].find(":") + 1));
		const auto input = (b->args()[0].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[i_index]) :
			stof(b->args()[0]);
		const auto delay = (b->args()[1].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
			stof(b->args()[1]);
		const auto level = b->args()[2];
		const auto point = std::find_if(interp.points().begin(),
			interp.points().end(),
			findVariable(b->blockArgs()[i_index]->args()[0]));
		if (std::time(nullptr) - (*point)->lastCOV() > delay) {
			const auto description = "The value(" + std::to_string((*point)->value())
				+ ") of " + (*point)->name() + " has gone stale for longer than "
				+ std::to_string(delay);
		}
	}
	
	void customAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alarmid = "350";
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