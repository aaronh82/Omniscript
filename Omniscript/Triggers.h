//
//  Primitives.h
//  OmniScript
//
//  Created by Aaron Houghton on 8/29/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__Primitives__
#define __OmniScript__Primitives__

#include "Logger.h"

namespace interp {
	static void whenOn() {
		LOG(Log::DEBUGGING, "whenOn");
	}
	
	static void whenSchedule() {
		LOG(Log::DEBUGGING, "whenSchedule");
	}
	
	static void whenBool() {
		LOG(Log::DEBUGGING, "whenBool");
	}
	
	static void whenIReceive() {
		LOG(Log::DEBUGGING, "whenIReceive");
	}
	
	static void broadcast() {
		LOG(Log::DEBUGGING, "broadcast");
	}
	
	static void doBroadcastAndWait() {
		LOG(Log::DEBUGGING, "doBroadcastAndWait");
	}
}

#endif /* defined(__OmniScript__Primitives__) */
