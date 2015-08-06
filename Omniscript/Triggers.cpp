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
#include <cstdlib>
#include <regex>

#include <unistd.h>
#include <stdio.h>

namespace interp {
	std::mutex m;
	std::condition_variable bcast;
	std::vector<std::shared_ptr<std::string> > msgs;
	std::atomic<bool> ready(false);
	std::atomic<bool> running(false);
	
	
	bool exec(const std::string& cmd) {
		FILE *pipe = popen(cmd.c_str(), "r");
		if (!pipe) {
			LOG(Log::ERROR, "Failed to execute '" + std::string(cmd) + "'");
		}
		char buffer[128];
		std::string res = "";
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL) {
				res += buffer;
			}
		}
		pclose(pipe);
		
		if (res.find("YES") != std::string::npos) {
			return true;
		}
		return false;
	}
		
	bool getSchedStatus(const std::string& name) {
		std::string cmd("/usr/bin/php -f "
						"/var/www/maverick/app/public_htdocs/api.php "
						"target=schedule action=get_status "
						"name=" + name + " output=text");
		return exec(cmd);
	}
	
	void whenOn::operator()(const block_ptr &b, Interpreter &interp) {
		LOG(Log::DEBUGGING, "whenOn");
	}
	
	void whenSchedule::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string name = b->args()[0];
		while (!getSchedStatus(name))
			sleep(60);
		
		LOG(Log::DEBUGGING, "whenSchedule: " + name);
	}
	
	void whenNotSchedule::operator()(const block_ptr &b, Interpreter &interp) {
		const std::string name = b->args()[0];
		while (getSchedStatus(name))
			sleep(60);
		
		LOG(Log::DEBUGGING, "whenNotSchedule: " + name);
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
	
	void doBroadcast::operator()(const block_ptr &b, Interpreter &interp) {
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
	
	void getLastMessage::operator()(const block_ptr &b, Interpreter &interp) {
		// TODO
	}
	
	void doStopThis::operator()(const block_ptr &b, Interpreter &interp) {
		// TODO
	}
	
	void doStopOthers::operator()(const block_ptr &b, Interpreter &interp) {
		// TODO
	}
	
	bool reportGetSched::operator()(const block_ptr &b, Interpreter &interp) {
		std::string name = b->args()[0];
		return getSchedStatus(name);
	}
}