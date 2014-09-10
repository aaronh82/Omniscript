//
//  Alarms.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Alarms.h"

namespace interp {
	void highTempLimit::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "highTempLimit");
	}
	
	void lowTempLimit::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "lowTempLimit");
	}
}