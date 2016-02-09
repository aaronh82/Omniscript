//
//  Interpreter.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Interpreter.h"

#include <chrono>
#include <unistd.h>

namespace interp {
	
	Interpreter::Interpreter(script::Script &prog): program_(prog) {
			initPrims();
	}
	
	void Interpreter::initPrims() {
		// Triggers
		primTable["whenOn"]					= std::make_shared<whenOn>();
		primTable["whenSchedule"]			= std::make_shared<whenSchedule>();
		primTable["whenNotSchedule"]		= std::make_shared<whenNotSchedule>();
		primTable["whenBool"]				= std::make_shared<whenBool>();
		primTable["whenIReceive"]			= std::make_shared<whenIReceive>();
		primTable["doBroadcast:"]			= std::make_shared<doBroadcast>();
		primTable["doBroadcastAndWait"]		= std::make_shared<doBroadcastAndWait>();
		primTable["getLastMessage"]			= std::make_shared<getLastMessage>();
		primTable["doStopThis"]				= std::make_shared<doStopThis>();
		primTable["doStopOthers"]			= std::make_shared<doStopOthers>();
		
		// Logic
		primTable["doWait"]					= std::make_shared<doWait>();
		primTable["doWaitUntil"]			= std::make_shared<doWaitUntil>();
		primTable["doRepeat"]				= std::make_shared<doRepeat>();
		primTable["doForever"]				= std::make_shared<doForever>();
		primTable["doIf"]					= std::make_shared<doIf>();
		primTable["doIfElse"]				= std::make_shared<doIfElse>();
		primTable["doUntil"]				= std::make_shared<doUntil>();

		// Operators
		primTable["reportSum"]				= std::make_shared<add>();
		primTable["reportDifference"]		= std::make_shared<subtract>();
		primTable["reportProduct"]			= std::make_shared<multiply>();
		primTable["reportQuotient"]			= std::make_shared<divide>();
		primTable["reportRandom"]			= std::make_shared<randomFromTo>();
		primTable["reportLessThan"]			= std::make_shared<lessThan>();
		primTable["reportEquals"]			= std::make_shared<equalTo>();
		primTable["reportGreaterThan"]		= std::make_shared<greaterThan>();
		primTable["reportAnd"]				= std::make_shared<logicalAnd>();
		primTable["reportOr"]				= std::make_shared<logicalOr>();
		primTable["reportNot"]				= std::make_shared<logicalNegation>();
		primTable["reportModulus"]			= std::make_shared<modulo>();
		primTable["reportRound"]			= std::make_shared<roundToNearest>();
		primTable["reportMonadic"]			= std::make_shared<computeFunction>();
		primTable["reportTrue"]				= std::make_shared<reportTrue>();
		primTable["reportFalse"]			= std::make_shared<reportFalse>();
		primTable["avg"]					= std::make_shared<avg>();
		primTable["maxOf"]					= std::make_shared<maxOf>();
		primTable["minOf"]					= std::make_shared<minOf>();
		primTable["getValOf"]				= std::make_shared<getValueOf>();
//		primTable["reportJSFunction"]		= std::make_shared<reportJSFunction>();
		
		// HVAC
		primTable["coolSetpoint"]			= std::make_shared<coolSetpoint>();
		primTable["heatSetpoint"]			= std::make_shared<heatSetpoint>();

		// Variables
		primTable["readVariable"]			= std::make_shared<readVariable>();
		primTable["doSetVar"]				= std::make_shared<setVar>();
		primTable["changeVar"]				= std::make_shared<changeVar>();
		primTable["readPoint"]				= std::make_shared<readPoint>();
		primTable["doSetPoint"]				= std::make_shared<setPoint>();
		primTable["doChangePoint"]			= std::make_shared<changePoint>();
		
		// Alarms
		primTable["highLimit"]				= std::make_shared<highLimit>();
		primTable["highLimitDelay"]			= std::make_shared<highLimitDelay>();
		primTable["lowLimit"]				= std::make_shared<lowLimit>();
		primTable["lowLimitDelay"]			= std::make_shared<lowLimitDelay>();
		primTable["runtimeLimit"]			= std::make_shared<runtimeLimit>();
		primTable["statusAlarm"]			= std::make_shared<statusAlarm>();
		primTable["customAlarm"]			= std::make_shared<customAlarm>();
	}
	
	void Interpreter::start() {
		std::map<const int, std::shared_future<float> > threads;
		for (auto script : program_.startingBlocks()) {
			if (script.second->opcode().find("when") == 0) {
				std::shared_future<float> f(std::async(std::launch::async,
											&Interpreter::execute, *this, script.second));
				if (f.valid())
					threads.insert({script.first, f});
			} else {
				LOG(Log::ERROR, "A script failed to start: " + script.second->opcode());
			}
		}
		
		while (!program_.needsRestart()) {
			for (auto &thread : threads) {
				if ((thread.second).wait_for(std::chrono::milliseconds(0))
					== std::future_status::ready) {
					const int idx(thread.first);
					thread.second = std::shared_future<float>(std::async(std::launch::async, &Interpreter::execute, *this, program_.startingBlocks()[idx]));
				}
				
			}
			
			for (auto& device: program_.devices()) {
				std::shared_ptr<sql::ResultSet> res =
				DB_READ("SELECT devicestatus.name "
						"FROM `devicestatus` "
						"INNER JOIN `device` "
						"ON device.devicestatusid=devicestatus.devicestatusid "
						"WHERE device.deviceid='" + std::to_string(device->id()) + "'");
				if (res->next()) {
					std::string state = res->getString("name");
					device->prevState(device->state());
					if (state == "Offline")
						device->state(DeviceStatus::Offline);
					else if (state == "Online")
						device->state(DeviceStatus::Online);
					else if (state == "Error")
						device->state(DeviceStatus::Error);
					else if (state == "Deleted")
						device->state(DeviceStatus::Deleted);
				}
			}
			sleep(5);
//			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}
	
	float Interpreter::execute(const block_ptr &b) {
		func_map::const_iterator op = primTable.find(b->opcode());
		if (op == primTable.end()) {
			LOG(Log::ERROR, "No function for " + b->opcode() + " was found");
			return -1;
		}
		
		float res = callFunc(op, b);
		
		if (b->next()) {
			execute(b->next());
		}
		return res;
	}
	
	float Interpreter::callFunc(const func_map::const_iterator& func, const block_ptr &b) {
		if (std::shared_ptr<VoidFunctor> p = std::dynamic_pointer_cast<VoidFunctor>(func->second)) {
			(*p)(b, *this);
		} else if (std::shared_ptr<BoolFunctor> p = std::dynamic_pointer_cast<BoolFunctor>(func->second)) {
			return (*p)(b, *this);
		} else if (std::shared_ptr<FloatFunctor> p = std::dynamic_pointer_cast<FloatFunctor>(func->second)) {
			return (*p)(b, *this);
		} else if (std::shared_ptr<IntFunctor> p = std::dynamic_pointer_cast<IntFunctor>(func->second)) {
			return (*p)(b, *this);
		}
		return 0;
	}
	
	const std::vector<var_ptr>& Interpreter::variables() {
		return program_.variables();
	}
	
	const std::vector<point_ptr>& Interpreter::points() {
		return program_.points();
	}
	
	const std::vector<dev_ptr>& Interpreter::devices() {
		return program_.devices();
	}
	
}
