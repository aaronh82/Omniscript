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
	
	Interpreter::Interpreter(const std::vector<block_ptr> scripts):scripts_(scripts) {
		initPrims();
	}
	
	void Interpreter::initPrims() {
		// Triggers
		primTable["whenOn"]					= &whenOn;
		primTable["whenSchedule"]			= &whenSchedule;
		primTable["whenBool"]				= &whenBool;
		primTable["whenIReceive"]			= &whenIReceive;
		primTable["broadcast:"]				= &broadcast;
		primTable["doBroadcastAndWait"]		= &doBroadcastAndWait;
		
		// Logic
		primTable["wait:elapsed:from:"]		= &waitFor;
		primTable["doRepeat"]				= &doRepeat;
		primTable["doForever"]				= &doForever;
		primTable["doIf"]					= &doIf;
		primTable["doIfElse"]				= &doIfElse;
		primTable["doWaitUntil"]			= &doWaitUntil;
		primTable["doUntil"]				= &doUntil;
		primTable["stopScripts"]			= &stopScripts;
		
		// Operators
		primTable["+"]						= &add;
		primTable["-"]						= &subtract;
		primTable["*"]						= &multiply;
		primTable["/"]						= &divide;
		primTable["randomFrom:to:"]			= &randomFromTo;
		primTable["<"]						= &lessThan;
		primTable["="]						= &equalTo;
		primTable[">"]						= &greaterThan;
		primTable["&"]						= &logicalAnd;
		primTable["|"]						= &logicalOr;
		primTable["not"]					= &logicalNegation;
		primTable["%"]						= &modulo;
		primTable["rounded"]				= &round;
		primTable["computeFunction:of:"]	= &computeFunction;
		
		// HVAC
		primTable["csetpt:db:in"]			= &coolSetpoint;
		primTable["hsetpt:db:in"]			= &heatSetpoint;

		// Variables
		primTable["setVar:to:"]				= &setVar;
		primTable["changeVar:by:"]			= &changeVar;
		primTable["setPoint:to:"]			= &setPoint;
		primTable["changePoint:by:"]		= &changePoint;
		
		// Alarms
		primTable["highTemp:limit:in:"]		= &highTempLimit;
		primTable["lowTemp:limit:in:"]		= &lowTempLimit;
	}
	
	void Interpreter::start() {
		std::vector<std::shared_future<void> > threads;
		for (auto script : scripts_) {
			if (script->opcode().find("when") == 0) {
				threads.emplace_back((std::async(std::launch::async, &Interpreter::execute, *this, script)));
			} else {
				LOG(Log::ERROR, "A script was not started with a trigger: " + script->opcode());
			}
		}
		for (auto thread : threads) {
			thread.wait();
		}
	}
	
	void Interpreter::execute(block_ptr b) {
		func_map::const_iterator op = primTable.find(b->opcode());
		if (op == primTable.end()) {
			LOG(Log::ERROR, "No function for " + b->opcode() + " was found");
			return;
		}
		if (b->blockArgs().size() > 0) {
			for (auto arg : b->blockArgs()) {
				LOG(Log::DEBUGGING, b->opcode() + " block: " + arg->opcode());
			}
		}
		for (auto arg : b->args()) {
			if (arg.find("block:") != std::string::npos) {
				LOG(Log::DEBUGGING, "Block: " + (b->blockArgs()[stoi(arg.substr(arg.find(":") + 1))])->opcode());
				execute(b->blockArgs()[stoi(arg.substr(arg.find(":") + 1))]);
			} else if (arg.find("nested") != std::string::npos) {
				continue;
			}
		}
		callFunc(op);
		if (b->next()) {
			execute(b->next());
		}
		return;
	}
	
	void Interpreter::callFunc(const func_map::const_iterator& func, ...) {
		(*(func->second))();
	}
	
	
	// Block Functions
}