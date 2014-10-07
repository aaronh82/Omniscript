//
//  Logic.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Logic.h"
#include "Interpreter.h"

#include <unistd.h>
#include <thread>
#include <sstream>

namespace interp {

	void waitFor::operator()(const block_ptr &b, Interpreter &interp) {
		int x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stoi(b->args()[0]);
		LOG(Log::DEBUGGING, "waitFor");
		sleep(x);
	}
						
	void doRepeat::operator()(const block_ptr &b, Interpreter &interp) {
		int x = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[std::stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					std::stoi(b->args()[0]);
		LOG(Log::DEBUGGING, "doRepeat");
		for (int i = 0; i < x; ++i) {
			interp.execute(b->nestedBlocks()[0]);
		}
	}
											 
	void doForever::operator()(const block_ptr &b, Interpreter &interp) {
		LOG(Log::DEBUGGING, "doForever");
		while (true) {
			interp.execute(b->nestedBlocks()[0]);
		}
	}
											   
	void doIf::operator()(const block_ptr &b, Interpreter &interp) {
		bool condition = interp.execute(b->blockArgs()[0]);
		if (condition) {
			interp.execute(b->nestedBlocks()[0]);
		}
		LOG(Log::DEBUGGING, "doIf");
	}
														
	void doIfElse::operator()(const block_ptr &b, Interpreter &interp) {
		bool condition = interp.execute(b->blockArgs()[0]);
		if (condition) {
			interp.execute(b->nestedBlocks()[0]);
		} else {
			interp.execute(b->nestedBlocks()[1]);
		}
		LOG(Log::DEBUGGING, "doIfElse");
	}
										 
	void doWaitUntil::operator()(const block_ptr &b, Interpreter &interp) {
		bool condition = interp.execute(b->blockArgs()[0]);
		while (!condition);
		LOG(Log::DEBUGGING, "doWaitUntil");
	}
												 
	void doUntil::operator()(const block_ptr &b, Interpreter &interp) {
		bool condition = interp.execute(b->blockArgs()[0]);
		while (!condition) {
			interp.execute(b->nestedBlocks()[0]);
		}
		LOG(Log::DEBUGGING, "doUntil");
	}
												 
//	void stopScripts::operator()(const block_ptr &b, Interpreter &interp) {
//		auto id = std::this_thread::get_id();
//		std::stringstream ss;
//		ss << id;
//		LOG(Log::DEBUGGING, "stopScripts: " + ss.str());
//	}
}