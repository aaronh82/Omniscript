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
#include "BaseFunctor.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {

	class Interpreter {
		typedef std::map<std::string, BaseFunctor*> func_map;
		func_map primTable;
		std::vector<block_ptr> scripts_;
				
		void initPrims();
		void execute(const block_ptr&);
		void callFunc(const func_map::const_iterator&, const block_ptr&, ...);
		
	public:
		Interpreter(const std::vector<block_ptr>);
//		~Interpreter();
		
		void start();
	};
	
}

#endif /* defined(__OmniScript__Interpreter__) */
