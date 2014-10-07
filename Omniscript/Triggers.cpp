//
//  Primitives.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 8/29/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Triggers.h"
#include "DBConnection.h"
#include "Interpreter.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <string>
#include <algorithm>

namespace interp {
	std::mutex m;
	std::condition_variable bcast;
	std::vector<std::shared_ptr<std::string> > msgs;
	std::atomic<bool> ready(false);
	std::atomic<bool> running(false);
	
	void whenOn::operator()(const block_ptr &b, Interpreter &interp) {
		LOG(Log::DEBUGGING, "whenOn");
	}
	
	void whenSchedule::operator()(const block_ptr &b, Interpreter &interp) {

//		DB_READ("SELECT (active, name")
		LOG(Log::DEBUGGING, "whenSchedule");
	}
	
	void whenBool::operator()(const block_ptr &b, Interpreter &interp) {
		bool condition = interp.execute(b->blockArgs()[0]);
		while (!condition);
		LOG(Log::DEBUGGING, "whenBool");
	}
	
	void whenIReceive::operator()(const block_ptr &b, Interpreter &interp) {
		running = true;
		
		auto inMsgs = [&](std::shared_ptr<std::string> s) {
			return s->compare(b->args()[0]) == 0; };
		std::vector<std::shared_ptr<std::string> >::iterator it;
		if ((it = std::find_if(msgs.begin(), msgs.end(), inMsgs)) != msgs.end()) {
			std::shared_ptr<std::string> msg(*it);
			std::unique_lock<std::mutex> lock(m);
			while (!ready) {
				if (b->args()[0] == *msg) {
					bcast.wait(lock);
					LOG(Log::DEBUGGING, "whenIReceive:" + b->args()[0]);
				}
			}
			
			if ((*it).unique()) {
				msgs.erase(it);
				ready = false;
				running = false;
			}
		}
	}
	
	void broadcast::operator()(const block_ptr &b, Interpreter &interp) {
		LOG(Log::DEBUGGING, "broadcast:" + b->args()[0]);
		std::unique_lock<std::mutex> lock(m);
		msgs.emplace_back(std::make_shared<std::string>(b->args()[0]));
		ready = true;
		bcast.notify_all();
	}
	
	void doBroadcastAndWait::operator()(const block_ptr &b, Interpreter &interp) {
		LOG(Log::DEBUGGING, "doBroadcastAndWait");
		std::unique_lock<std::mutex> lock(m);
		msgs.emplace_back((std::make_shared<std::string>(b->args()[0])));
		ready = true;
		bcast.notify_all();
		auto inMsgs = [&](std::shared_ptr<std::string> s) {
			return s->compare(b->args()[0]) == 0; };
		while (std::find_if(msgs.begin(), msgs.end(), inMsgs) != msgs.end());
	}
}