//
//  Variables.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Variables__
#define __Omniscript__Variables__

#include "Logger.h"
#include <unistd.h>

namespace interp {
	static void setVar() {
		LOG(Log::DEBUGGING, "setVar");
	}
	
	static void changeVar() {
		LOG(Log::DEBUGGING, "changeVar");
	}
	
	static void setPoint() {
		LOG(Log::DEBUGGING, "setPoint");
	}
	
	static void changePoint() {
		LOG(Log::DEBUGGING, "changePoint");
	}
}

#endif /* defined(__Omniscript__Variables__) */
