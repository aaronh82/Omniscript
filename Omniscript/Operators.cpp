//
//  Operators.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Operators.h"

namespace interp {
	float add::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "add");
		return 0;
	}
	
	float subtract::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "subtract");
		return 0;
	}
	
	float multiply::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "multiply");
		return 0;
	}
	
	float divide::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "divide");
		return 0;
	}
	
	float randomFromTo::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "randomFromTo");
		return 0;
	}
	
	float lessThan::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "lessThan");
		return 0;
	}
	
	float equalTo::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "equalTo");
		return 0;
	}
	
	float greaterThan::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "greaterThan");
		return 0;
	}
	
	float logicalAnd::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "logicalAnd");
		return 0;
	}
	
	float logicalOr::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "logicalOr");
		return 0;
	}
	
	float logicalNegation::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "logicalNegation");
		return 0;
	}
	
	float modulo::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "modulo");
		return 0;
	}
	
	float round::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "round");
		return 0;
	}
	
	float computeFunction::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "computeFunction");
		return 0;
	}
}