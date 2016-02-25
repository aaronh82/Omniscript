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
#include "Device.h"
#include "Variables.h"
#include "XML_Parser.h"

using block_ptr = blocks::Block::block_ptr;


namespace util {
	class XML;
}

namespace script {
	typedef std::shared_ptr<interp::Variable> var_ptr;
	typedef std::shared_ptr<interp::Point> point_ptr;
	typedef std::shared_ptr<interp::Device> dev_ptr;
	
	class Script {
		friend class util::XML;
		
		std::string name_;
		unsigned int id_;
		std::map<int, block_ptr> startingBlocks_;
		std::time_t lastModified_ = 0;
		bool enabled_;
		bool restart_;
		
		std::vector<var_ptr> variables_;
		std::vector<point_ptr> points_;
		std::vector<dev_ptr> devices_;

		std::time_t convertTime(const std::string&);
		
	public:
		Script(std::string, unsigned int, std::string, std::istream*, bool);
		
		void name(std::string);
		std::string name() const;
		unsigned int id();
		unsigned int id() const;
		const std::vector<var_ptr>& variables();
		const std::vector<point_ptr>& points();
		std::vector<dev_ptr>& devices();
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
