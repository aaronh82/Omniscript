//
//  HVAC.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "HVAC.h"

namespace interp {
	bool coolSetpoint::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "coolSetpoint");
		return true;
	}
	
	bool heatSetpoint::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "heatSetpoint");
		return true;
	}
}