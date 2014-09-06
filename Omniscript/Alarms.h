//
//  Alarms.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Alarms__
#define __Omniscript__Alarms__

#include "Logger.h"

namespace interp {
	static void highTempLimit() {
		LOG(Log::DEBUGGING, "highTempLimit");
	}
	
	static void lowTempLimit() {
		LOG(Log::DEBUGGING, "lowTempLimit");
	}
}

#endif /* defined(__Omniscript__Alarms__) */
