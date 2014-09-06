//
//  Script.h
//  OmniScript
//
//  Created by Aaron Houghton on 8/26/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__Script__
#define __OmniScript__Script__

#include <iostream>
#include <vector>
#include <ctime>

#include "Block.h"

using block_ptr = blocks::Block::block_ptr;

namespace script {
	class Script {
		std::string name_;
		uint id_;
		std::vector<block_ptr> startingBlocks_;
		time_t lastModified_ = 0;
		bool enabled_;
		
		time_t convertTime(std::string);
		std::string getDate(std::string);
		std::string getTime(std::string);
		int getYear(std::string);
		int getMonth(std::string);
		int getDay(std::string);
		int getHours(std::string);
		int getMinutes(std::string);
		int getSeconds(std::string);
		
	public:
		Script(std::string, uint, std::string, std::istream*, bool);
		
		std::string name();
		std::string name() const;
		uint id();
		uint id() const;
		void enable();
		void disable();
		bool isEnabled();
		bool isEnabled() const;
		const std::vector<block_ptr> startingBlocks() const;
	};
}

#endif /* defined(__OmniScript__Script__) */
