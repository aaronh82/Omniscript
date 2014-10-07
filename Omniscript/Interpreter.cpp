//
//  Interpreter.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Interpreter.h"
#include "Triggers.h"
#include "Logic.h"
#include "Operators.h"
#include "HVAC.h"
#include "Variables.h"
#include "Alarms.h"
#include "Logger.h"


namespace interp {
	
	Interpreter::Interpreter(const script::Script &prog):
		scripts_(prog.startingBlocks()), variables_(prog.variables()), points_(prog.points()) {
			initPrims();
	}
	
	void Interpreter::initPrims() {
		// Triggers
		primTable["whenOn"]							= std::make_shared<whenOn>();
		primTable["whenSchedule"]					= std::make_shared<whenSchedule>();
		primTable["whenBool"]						= std::make_shared<whenBool>();
		primTable["whenIReceive"]					= std::make_shared<whenIReceive>();
		primTable["broadcast:"]						= std::make_shared<broadcast>();
		primTable["doBroadcastAndWait"]				= std::make_shared<doBroadcastAndWait>();
		
		// Logic
		primTable["wait:elapsed:from:"]				= std::make_shared<waitFor>();
		primTable["doRepeat"]						= std::make_shared<doRepeat>();
		primTable["doForever"]						= std::make_shared<doForever>();
		primTable["doIf"]							= std::make_shared<doIf>();
		primTable["doIfElse"]						= std::make_shared<doIfElse>();
		primTable["doWaitUntil"]					= std::make_shared<doWaitUntil>();
		primTable["doUntil"]						= std::make_shared<doUntil>();
//		primTable["stopScripts"]					= std::make_shared<stopScripts>();

		// Operators
		primTable["+"]								= std::make_shared<add>();
		primTable["-"]								= std::make_shared<subtract>();
		primTable["*"]								= std::make_shared<multiply>();
		primTable["/"]								= std::make_shared<divide>();
		primTable["randomFrom:to:"]					= std::make_shared<randomFromTo>();
		primTable["<"]								= std::make_shared<lessThan>();
		primTable["="]								= std::make_shared<equalTo>();
		primTable[">"]								= std::make_shared<greaterThan>();
		primTable["&"]								= std::make_shared<logicalAnd>();
		primTable["|"]								= std::make_shared<logicalOr>();
		primTable["not"]							= std::make_shared<logicalNegation>();
		primTable["%"]								= std::make_shared<modulo>();
		primTable["rounded"]						= std::make_shared<roundToNearest>();
		primTable["computeFunction:of:"]			= std::make_shared<computeFunction>();
		primTable["avg"]							= std::make_shared<avg>();
		primTable["maxOf"]							= std::make_shared<maxOf>();
		primTable["minOf"]							= std::make_shared<minOf>();
		primTable["getValOf"]						= std::make_shared<getValueOf>();
		
		// HVAC
		primTable["csetpt:db:in:"]					= std::make_shared<coolSetpoint>();
		primTable["hsetpt:db:in:"]					= std::make_shared<heatSetpoint>();

		// Variables
		primTable["readVariable"]					= std::make_shared<readVariable>();
		primTable["setVar:to:"]						= std::make_shared<setVar>();
		primTable["changeVar:by:"]					= std::make_shared<changeVar>();
		primTable["readPoint"]						= std::make_shared<readPoint>();
		primTable["setPoint:to:"]					= std::make_shared<setPoint>();
		primTable["changePoint:by:"]				= std::make_shared<changePoint>();
		
		// Alarms
		primTable["highTempLimit:in:level:"]		= std::make_shared<highTempLimit>();
		primTable["lowTempLimit:in:level:"]			= std::make_shared<lowTempLimit>();
	}
	
	void Interpreter::start() {
		std::vector<std::shared_future<float> > threads;
		for (auto script : scripts_) {
			if (script->opcode().find("when") == 0) {
				threads.emplace_back(std::async(std::launch::async, &Interpreter::execute, *this, script));
			} else {
				LOG(Log::ERROR, "A script was not started with a trigger: " + script->opcode());
			}
		}
		for (auto thread : threads) {
			thread.wait();
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
	
	std::vector<std::shared_ptr<Variable> > Interpreter::variables() {
		return variables_;
	}
	
	std::vector<std::shared_ptr<Point> > Interpreter::points() {
		return points_;
	}
	
}