//
//  main.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 6/16/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <future>
#include <chrono>
#include <map>
#include <vector>
#include <memory>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Interpreter.h"
#include "DBConnection.h"
#include "Logger.h"
#include "Script.h"

std::map<uint, std::shared_future<void>> pid_list;

void process(const script::Script& prog) {
	uint id = prog.id();
	if (prog.isEnabled()) {
		if (pid_list.find(id) == pid_list.end()) {
			LOG(Log::DEBUGGING, prog.name() + " is " + (prog.isEnabled() ? "Enabled" : "Disabled"));
			interp::Interpreter interp(prog);
			std::shared_future<void> f(std::async(std::launch::async, &interp::Interpreter::start, &interp));
			sleep(1);
			if (f.valid()) {
				pid_list.insert(std::pair<uint, std::shared_future<void> >(id, f));
			} else {
				LOG(Log::WARN, "Thread for " + prog.name() + " did not initialize");
			}
		}
		
		for (auto thread : pid_list) {
			if ((thread.second).wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				LOG(Log::DEBUGGING, "Restarting " + prog.name() + "(" + std::to_string(thread.first) + ")");
				interp::Interpreter interp(prog);
				thread.second = std::async(std::launch::async, &interp::Interpreter::start, &interp);
				sleep(1);
			}
			//		} else {
			//			LOG(Log::DEBUGGING, "Thread " + std::to_string(thread.first) + " is still running");
			//		}
		}
	}
}

int main(int argc, const char * argv[])
{
	LOG(Log::INFO, "Starting Omniscript...");
	pid_t pid, sid;
		
	pid = fork();
	if (pid < 0) {
		LOG(Log::CRIT, "Failed to fork process!");
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	
	umask(0);
	
	sid = setsid();
	if (sid < 0) {
		LOG(Log::DEBUGGING, "Failed to get SUID");
		exit(EXIT_FAILURE);
	}
	
	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	/* Daemon-specific initialization goes here */
	std::shared_ptr<sql::ResultSet> res;
	std::vector<std::shared_ptr<script::Script> > programs;
	
	auto inPrograms = [&](std::shared_ptr<script::Script> prog) {
		return res->getUInt("id") == prog->id(); };
	
	while (true) {
		res = DB_READ("SELECT * FROM `programs`");
		while (res->next()) {
			std::vector<std::shared_ptr<script::Script> >::iterator it =
					std::find_if(programs.begin(), programs.end(), inPrograms);
			if (it == programs.end()) {
				programs.push_back(std::make_shared<script::Script> (
													res->getString("name"),
													res->getUInt("id"),
													res->getString("last_modified"),
													res->getBlob("data"),
													res->getBoolean("enabled")));
			} else if ((*it)->updated(res->getString("last_modified"))){
				(*it)->name(res->getString("name"));
				(*it)->updateScript(res->getBlob("data"));
				res->getBoolean("enabled") ? (*it)->enable() : (*it)->disable();
				
			}
		}
		
		// If program is deleted from the database, remove from programs vector.
//		if (res->rowsCount() < programs.size()) {
//			for (auto prog : programs) {
//				res->first();
//				while (res->next()) {
//					if
//				}
//			}
//		}
		
		for (auto prog : programs) {
			process(*prog);
		}
		sleep(30);
	}
	
	exit(EXIT_SUCCESS);
}

