//
//  Alarms.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Alarms.h"
#include "DBConnection.h"
#include "AlarmHandler.h"
#include "Variables.h"

#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <ctime>

namespace interp {
// Alarm Type Definitions
	
	void highLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alias = "omni-high-limit";
		const auto limit = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
						stof(b->args()[0]);
		const auto i_index = stoi(b->args()[1].substr(b->args()[1].find(":") + 1));
		const auto input = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[1]);
		std::string level = b->args()[2];
		const auto point = std::find_if(interp.points().begin(),
		                                interp.points().end(),
		                                findVariable(b->blockArgs()[i_index]->args()[0]));

		if (input > limit) {
			const std::string name = "High Limit";
			const std::string description = "" + (*point)->name() + "("
									 + std::to_string(input) + ") of "
									 + (*point)->deviceName() +
									 " is above limit("
									 + std::to_string(limit) + ")";
			AlarmHandler::getHandler().add(
					std::make_shared<Alarm>(alias, level, name, description,
					                        **point));
		} else {
			AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
		}
		LOG(Log::DEBUGGING, "highLimit");
	}

	void highLimitDelay::operator()(const block_ptr &b, Interpreter &interp) {
		static bool active(false);
		if (!active) {
			std::thread t([&] () {
				const std::string alias = "omni-high-limit-delay";
				const auto limit = (b->args()[0].find("block:") != std::string::npos) ?
								   interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
								   stof(b->args()[0]);
				const auto i_index = stoi(b->args()[1].substr(b->args()[1].find(":") + 1));
				const auto input = [&]() -> float { 
										return (b->args()[1].find("block:") != std::string::npos) ?
										interp.execute(b->blockArgs()[i_index]) :
										stof(b->args()[1]);
								   };
				const auto delay = (b->args()[2].find("block:") != std::string::npos) ?
								   interp.execute(b->blockArgs().back()) :
								   stof(b->args()[2]);
				std::string level = b->args()[3];
				const auto point = std::find_if(interp.points().begin(),
				                                interp.points().end(),
				                                findVariable(b->blockArgs()[i_index]->args()[0]));
				const std::time_t start = std::time(nullptr);
				while (input() > limit) {
					active = true;
					if (std::time(nullptr) - start > delay) {
						const std::string name = "High Limit Delay";
						const std::string description = "" + (*point)->name() + "("
														+ std::to_string(input()) + ") of "
														+ (*point)->deviceName() +
														" is above limit("
														+ std::to_string(limit) + ")";
						AlarmHandler::getHandler().add(
								std::make_shared<Alarm>(alias, level, name,
								                        description, **point));
						LOG(Log::DEBUGGING, "highLimitDelay");
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
				
				AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
				active = false;
				return;
			});
			t.detach();
		}
	}
	
	void lowLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alias = "omni-low-limit";
		const auto limit = (b->args()[0].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
						stof(b->args()[0]);
		const auto i_index = stoi(b->args()[1].substr(b->args()[1].find(":") + 1));
		const auto input = (b->args()[1].find("block:") != std::string::npos) ?
						interp.execute(b->blockArgs()[i_index]) :
						stof(b->args()[1]);
		const auto level = b->args()[2];
		const auto point = std::find_if(interp.points().begin(),
		                                interp.points().end(),
		                                findVariable(b->blockArgs()[i_index]->args()[0]));

		if (input < limit) {
			const std::string name = "Low Limit";
			const std::string description = (*point)->name() + "("
									 + std::to_string(input) + ") of "
									 + (*point)->deviceName() +
									 " is below limit("
									 + std::to_string(limit) + ")";
			AlarmHandler::getHandler().add(
					std::make_shared<Alarm>(alias, level, name, description,
					                        **point));
		} else {
			AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
		}
		LOG(Log::DEBUGGING, "lowLimit");
	}

	void lowLimitDelay::operator()(const block_ptr &b, Interpreter &interp) {
		static bool active(false);
		if (!active) {
			std::thread t([&]() {
				const std::string alias = "omni-low-limit-delay";
				const auto limit = (b->args()[0].find("block:") != std::string::npos) ?
								   interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
								   stof(b->args()[0]);
				const auto i_index = stoi(b->args()[1].substr(b->args()[1].find(":") + 1));
				const auto input = [&] () -> float {
										return (b->args()[1].find("block:") != std::string::npos) ?
										interp.execute(b->blockArgs()[i_index]) :
										stof(b->args()[1]);
								   };
				const auto delay = (b->args()[2].find("block:") != std::string::npos) ?
								   interp.execute(b->blockArgs().back()) :
								   stof(b->args()[2]);
				std::string level = b->args()[3];
				const auto point = std::find_if(interp.points().begin(),
				                                interp.points().end(),
				                                findVariable(b->blockArgs()[i_index]->args()[0]));
				const std::time_t start = std::time(nullptr);
				while (input() < limit) {
					active = true;
					if (std::time(nullptr) - start > delay) {
						const std::string name = "Low Limit Delay";
						const std::string description = "" + (*point)->name() + "("
														+ std::to_string(input()) + ") of "
														+ (*point)->deviceName() +
														" is below limit("
														+ std::to_string(limit) + ")";
						AlarmHandler::getHandler().add(
								std::make_shared<Alarm>(alias, level, name,
								                        description, **point));
						LOG(Log::DEBUGGING, "lowLimitDelay");
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
				
				AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
				active = false;
				return;
			});
			t.detach();
		}
	}
	
	void runtimeLimit::operator()(const block_ptr &b, Interpreter &interp) {
		const auto alias = "omni-runtime";
		const auto name = "Runtime Limit";
		const auto delay = (b->args()[1].find("block:") != std::string::npos) ?
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
		if ((input == onValue) && ((*point)->lastPoll() - (*point)->lastCOV()) > delay) {
			const auto description = (*point)->name() +
			                         " has been running longer than " +
			                         std::to_string(delay) + " seconds.";
			AlarmHandler::getHandler().add(
					std::make_shared<Alarm>(alias, level, name, description,
					                        **point));
		} else {
			AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
		}
	}
	
	void statusAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alias = "omni-status";
		const auto name = "Status Alarm";
		const auto expected = (b->args()[1].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
			stof(b->args()[1]);
		const auto i_index = stoi(b->args()[0].substr(b->args()[0].find(":") + 1));
		const auto input = (b->args()[0].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs()[i_index]) :
			stof(b->args()[0]);
		const auto delay = (b->args()[2].find("block:") != std::string::npos) ?
			interp.execute(b->blockArgs().back()) :
			stof(b->args()[2]);
		auto level = b->args()[3];
		const auto point = std::find_if(interp.points().begin(),
			interp.points().end(),
			findVariable(b->blockArgs()[i_index]->args()[0]));
		if ((input != expected) && ((std::time(nullptr) - (*point)->lastCOV()) > delay)) {
			const auto description = (*point)->name() +
					" of " + (*point)->deviceName() +
				" hasn't changed to expected state in more than " +
				std::to_string(delay) + " seconds.";
			AlarmHandler::getHandler().add(
					std::make_shared<Alarm>(alias, level, name, description,
					                        **point));
		} else {
			AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
		}
	}

	void staleValueAlarm::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string alias = "omni-stale-value";
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
				+ ") of " + (*point)->name() + " of " + (*point)->deviceName() + " has gone stale for longer than "
				+ std::to_string(delay);
			AlarmHandler::getHandler().add(
					std::make_shared<Alarm>(alias, level, name, description,
					                        **point));
		} else {
			AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
		}
	}
	
	void customAlarm::operator()(const block_ptr &b, Interpreter &interp) {
//		const std::string alias = "omni-user-defined";
//		const std::string name = "User-Defined Alarm";
//		std::string msg = b->args()[0];
//		std::string level = b->args()[2];
//		std::vector<block_ptr> ba(b->blockArgs());
//		std::queue<block_ptr, std::deque<block_ptr> >
//			inputs(std::deque<block_ptr>(ba.begin(),
//										 ba.end()));
//		std::string fmt;
//		std::string tmp;
//		std::string::iterator pos;
//		for (std::string::iterator it = msg.begin(); it != msg.end(); ++it) {
//			if (*it == '%') {
//				pos = it;
//				fmt = *(++it);
//
//				if (fmt == "value") {
//					block_ptr tmp_blk = inputs.front();
//					tmp = std::to_string(interp.execute(tmp_blk));
//					break;
//				} else if (fmt == "name") {
//					tmp = inputs.front()->args()[0];
//					break;
//				}
//
//				inputs.pop();
//				auto d = std::distance(msg.begin(), it);
//				d += (tmp.length() - 1);
//				msg.replace(pos, pos + 2, tmp);
//				it = msg.begin() + d;
//			}
//		}
//		AlarmHandler::getHandler().add(std::make_shared<Alarm>(alias, level, name, msg));
//		LOG(Log::DEBUGGING, "userDefinedAlarm");
	}

//	void clearCustomAlarm::operator()(const block_ptr &ptr,
//	                                  Interpreter &interpreter) {
//		AlarmHandler::getHandler().clear(alias, std::to_string((*point)->device()->id()));
//	}
}