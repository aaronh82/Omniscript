//
//  Logic.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Logic__
#define __Omniscript__Logic__

#include "Logger.h"

namespace interp {
	static void waitFor() {
		LOG(Log::DEBUGGING, "waitFor");
	}
	
	static void doRepeat() {
		LOG(Log::DEBUGGING, "doRepeat");
	}
	
	static void doForever() {
		LOG(Log::DEBUGGING, "doForever");
	}
	
	static void doIf() {
		LOG(Log::DEBUGGING, "doIf");
	}
	
	static void doIfElse() {
		LOG(Log::DEBUGGING, "doIfElse");
	}
	
	static void doWaitUntil() {
		LOG(Log::DEBUGGING, "doWaitUntil");
	}
	
	static void doUntil() {
		LOG(Log::DEBUGGING, "doUntil");
	}
	
	static void stopScripts() {
		LOG(Log::DEBUGGING, "stopScripts");
	}
}

#endif /* defined(__Omniscript__Logic__) */
