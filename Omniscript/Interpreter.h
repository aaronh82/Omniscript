//
//  Interpreter.h
//  OmniScript
//
//  Created by Aaron Houghton on 6/30/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__Interpreter__
#define __OmniScript__Interpreter__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <future>

#include <cstdarg>

#include "Block.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {

	class Interpreter {
		typedef void (*func)();
		typedef std::map<std::string, func> func_map;
		func_map primTable;
		std::vector<block_ptr> scripts_;
				
		void initPrims();
		void execute(block_ptr);
		void callFunc(const func_map::const_iterator&, ...);
		
	public:
		Interpreter(const std::vector<block_ptr>);
		~Interpreter() {};
		
		void start();
	};
	
}

#endif /* defined(__OmniScript__Interpreter__) */
