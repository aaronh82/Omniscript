//
//  Variables.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Variables.h"

namespace interp {
	void setVar::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "setVar");
	}
	
	void changeVar::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "changeVar");
	}
	
	void setPoint::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "setPoint");
	}
	
	void changePoint::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "changePoint");
	}
}