//
//  Logic.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Logic.h"

namespace interp {
	void waitFor::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "waitFor");
	}
						
	void doRepeat::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "doRepeat");
	}
											 
	void doForever::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "doForever");
	}
											   
	void doIf::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "doIf");
	}
														
	void doIfElse::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "doIfElse");
	}
										 
	void doWaitUntil::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "doWaitUntil");
	}
												 
	void doUntil::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "doUntil");
	}
												 
	void stopScripts::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "stopScripts");
	}
}