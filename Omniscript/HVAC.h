//
//  HVAC.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__HVAC__
#define __Omniscript__HVAC__

#include "Logger.h"

namespace interp {
	static void coolSetpoint() {
		LOG(Log::DEBUGGING, "coolSetpoint");
	}
	
	static void heatSetpoint() {
		LOG(Log::DEBUGGING, "heatSetpoint");
	}
}

#endif /* defined(__Omniscript__HVAC__) */
