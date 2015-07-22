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
#include <map>
#include <ctime>
#include <memory>

#include "Block.h"
#include "Variables.h"
#include "XML_Parser.h"

using block_ptr = blocks::Block::block_ptr;

namespace util {
	class XML;
}

namespace script {
	
	class Script {
		friend class util::XML;
		
		std::string name_;
		unsigned int id_;
		std::map<int, block_ptr> startingBlocks_;
		std::time_t lastModified_ = 0;
		bool enabled_;
		bool restart_;
		
		std::vector<std::shared_ptr<interp::Variable> > variables_;
		std::vector<std::shared_ptr<interp::Point> > points_;
		
		
		std::time_t convertTime(std::string);
		std::string getDate(std::string);
		std::string getTime(std::string);
		int getYear(std::string);
		int getMonth(std::string);
		int getDay(std::string);
		int getHours(std::string);
		int getMinutes(std::string);
		int getSeconds(std::string);
		
	public:
		Script(std::string, unsigned int, std::string, std::istream*, bool);
		
		std::string name(std::string = "");
		std::string name() const;
		unsigned int id();
		unsigned int id() const;
		const std::vector<std::shared_ptr<interp::Variable> >& variables() const;
		const std::vector<std::shared_ptr<interp::Point> >& points() const;
		void enable();
		void disable();
		bool isEnabled();
		bool isEnabled() const;
		std::map<int, block_ptr> startingBlocks();
		bool updated(std::string);
		void updateScript(std::istream*);
		bool needsRestart();
	};
}

#endif /* defined(__OmniScript__Script__) */
