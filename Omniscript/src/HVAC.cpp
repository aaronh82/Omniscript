//
//  HVAC.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "HVAC.h"
#include "Interpreter.h"

#include <future>
#include <vector>

namespace interp {
		
	bool cooling = false;
	bool heating = false;
	
	bool coolSetpoint::operator()(const block_ptr &b, Interpreter &interp) {
		float sp = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					stof(b->args()[0]);
		float db = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		float in = (b->args()[2].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[2].substr(b->args()[2].find(":") + 1))]) :
					stof(b->args()[2]);
		
		LOG(Log::DEBUGGING, "coolSetpoint");
		
		if (cooling && (in < (sp - db))) {
			LOG(Log::DEBUGGING, "Setting cooling to false");
			cooling = false;
		} else if (!cooling && (in > (sp + db))) {
			LOG(Log::DEBUGGING, "Setting cooling to true");
			cooling = true;
		}
		return cooling;
	}
	
	bool heatSetpoint::operator()(const block_ptr &b, Interpreter &interp) {
		float sp = (b->args()[0].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[0].substr(b->args()[0].find(":") + 1))]) :
					stof(b->args()[0]);
		float db = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		float in = (b->args()[2].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[2].substr(b->args()[2].find(":") + 1))]) :
					stof(b->args()[2]);

		
		LOG(Log::DEBUGGING, "heatSetpoint");
		
		if (heating && (in > (sp + db))) {
			heating = false;
		} else if (!heating && (in < (sp - db))) {
			heating = true;
		}
		return heating;
	}
}