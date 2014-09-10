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
	
//	Interpreter::~Interpreter() {
//		for (auto f : primTable) {
//			delete f.second;
//		}
//	}
	
	void Interpreter::initPrims() {
		// Triggers
		primTable["whenOn"]					= new whenOn;
		primTable["whenSchedule"]			= new whenSchedule;
		primTable["whenBool"]				= new whenBool;
		primTable["whenIReceive"]			= new whenIReceive;
		primTable["broadcast:"]				= new broadcast;
		primTable["doBroadcastAndWait"]		= new doBroadcastAndWait;
		
		// Logic
		primTable["wait:elapsed:from:"]		= new waitFor;
		primTable["doRepeat"]				= new doRepeat;
		primTable["doForever"]				= new doForever;
		primTable["doIf"]					= new doIf;
		primTable["doIfElse"]				= new doIfElse;
		primTable["doWaitUntil"]			= new doWaitUntil;
		primTable["doUntil"]				= new doUntil;
		primTable["stopScripts"]			= new stopScripts;

		// Operators
		primTable["+"]						= new add;
		primTable["-"]						= new subtract;
		primTable["*"]						= new multiply;
		primTable["/"]						= new divide;
		primTable["randomFrom:to:"]			= new randomFromTo;
		primTable["<"]						= new lessThan;
		primTable["="]						= new equalTo;
		primTable[">"]						= new greaterThan;
		primTable["&"]						= new logicalAnd;
		primTable["|"]						= new logicalOr;
		primTable["not"]					= new logicalNegation;
		primTable["%"]						= new modulo;
		primTable["rounded"]				= new round;
		primTable["computeFunction:of:"]	= new computeFunction;
		
		// HVAC
		primTable["csetpt:db:in:"]			= new coolSetpoint;
		primTable["hsetpt:db:in:"]			= new heatSetpoint;

		// Variables
		primTable["setVar:to:"]				= new setVar;
		primTable["changeVar:by:"]			= new changeVar;
		primTable["setPoint:to:"]			= new setPoint;
		primTable["changePoint:by:"]		= new changePoint;
		
		// Alarms
		primTable["highTemp:limit:in:"]		= new highTempLimit;
		primTable["lowTemp:limit:in:"]		= new lowTempLimit;
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
	
	void Interpreter::execute(const block_ptr &b) {
		func_map::const_iterator op = primTable.find(b->opcode());
		if (op == primTable.end()) {
			LOG(Log::ERROR, "No function for " + b->opcode() + " was found");
			return;
		}
//		for (auto arg : b->args()) {
//			if (arg.find("block:") != std::string::npos) {
//				LOG(Log::DEBUGGING, b->opcode() + " blockArgs size: " + std::to_string(b->blockArgs().size()));
//				execute(b->blockArgs()[stoi(arg.substr(arg.find(":") + 1))]);
//			}
//		}
		callFunc(op, b);
		if (b->next()) {
			execute(b->next());
		}
		return;
	}
	
	void Interpreter::callFunc(const func_map::const_iterator& func, const block_ptr &b, ...) {
		if (VoidFunctor *p = dynamic_cast<VoidFunctor*>(func->second)) {
			(*p)(b);
		} else if (BoolFunctor *p = dynamic_cast<BoolFunctor*>(func->second)) {
			(*p)(b);
		} else if (FloatFunctor *p = dynamic_cast<FloatFunctor*>(func->second)) {
			(*p)(b);
		}
//		(*(func->second))(b);
	}
	
	
	// Block Functions
}