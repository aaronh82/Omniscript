//
//  Operators.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Operators__
#define __Omniscript__Operators__

#include "Logger.h"

namespace interp {
	static void add() {
		LOG(Log::DEBUGGING, "add");
	}
	
	static void subtract() {
		LOG(Log::DEBUGGING, "subtract");
	}
	
	static void multiply() {
		LOG(Log::DEBUGGING, "multiply");
	}
	
	static void divide() {
		LOG(Log::DEBUGGING, "divide");
	}
	
	static void randomFromTo() {
		LOG(Log::DEBUGGING, "randomFromTo");
	}
	
	static void	lessThan() {
		LOG(Log::DEBUGGING, "lessThan");
	}
	
	static void equalTo() {
		LOG(Log::DEBUGGING, "equalTo");
	}
	
	static void greaterThan() {
		LOG(Log::DEBUGGING, "greaterThan");
	}
	
	static void logicalAnd() {
		LOG(Log::DEBUGGING, "logicalAnd");
	}
	
	static void logicalOr() {
		LOG(Log::DEBUGGING, "logicalOr");
	}
	
	static void logicalNegation() {
		LOG(Log::DEBUGGING, "logicalNegation");
	}
	
	static void modulo() {
		LOG(Log::DEBUGGING, "modulo");
	}
	
	static void round() {
		LOG(Log::DEBUGGING, "round");
	}
	
	static void computeFunction() {
		LOG(Log::DEBUGGING, "computeFunction");
	}
}

#endif /* defined(__Omniscript__Operators__) */
