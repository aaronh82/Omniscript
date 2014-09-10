//
//  Primitives.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 8/29/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Triggers.h"
#include "DBConnection.h"

#include <mutex>
#include <condition_variable>

namespace interp {
	std::mutex m;
	std::condition_variable bcast;
	std::string msg = "";
	bool ready = false;
	bool running = false;
	
	void whenOn::operator()(const block_ptr &b) {
		LOG(Log::DEBUGGING, "whenOn");
	}
	
	void whenSchedule::operator()(const block_ptr &b) {
		if (b->args().size() != 1) {
			throw "The wrong number of arguments was provided to 'whenSchedule'";
		}
//		DB_READ("SELECT (active, name")
		LOG(Log::DEBUGGING, "whenSchedule");
	}
	
	void whenBool::operator()(const block_ptr &b) {
		if (b->args().size() != 1) {
			throw "The wrong number of arguments was provided to 'whenBool'";
		}
		
		LOG(Log::DEBUGGING, "whenBool");
	}
	
	void whenIReceive::operator()(const block_ptr &b) {
		if (b->args().size() != 1) {
			throw "The wrong number of arguments was provided to 'whenIReceive'";
		}
		running = true;
		std::unique_lock<std::mutex> lock(m);
		while (!ready) {
			if (b->args()[0] == msg) {
				bcast.wait(lock);
				LOG(Log::DEBUGGING, "whenIReceive:" + b->args()[0]);
			}
		}
		msg = "";
		ready = false;
		running = false;
	}
	
	void broadcast::operator()(const block_ptr &b) {
		if (b->args().size() != 1) {
			throw "The wrong number of arguments was provided to 'broadcast'";
		}
		LOG(Log::DEBUGGING, "broadcast:" + b->args()[0]);
		msg = b->args()[0];
		std::unique_lock<std::mutex> lock(m);
		ready = true;
		bcast.notify_all();
	}
	
	void doBroadcastAndWait::operator()(const block_ptr &b) {
		if (b->args().size() != 1) {
			throw "The wrong number of arguments was provided to 'broadcast and wait'";
		}
		LOG(Log::DEBUGGING, "doBroadcastAndWait");
		msg = b->args()[0];
		std::unique_lock<std::mutex> lock(m);
		ready = true;
		bcast.notify_all();
		while (running);
	}
}