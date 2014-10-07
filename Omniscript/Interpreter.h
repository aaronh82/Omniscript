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
#include "Script.h"
#include "BaseFunctor.h"
#include "Variables.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {

	class Interpreter {
		typedef std::map<std::string, std::shared_ptr<BaseFunctor> > func_map;
		func_map primTable;
		std::vector<block_ptr> scripts_;
		std::vector<std::shared_ptr<Variable> > variables_;
		std::vector<std::shared_ptr<Point> > points_;
				
		void initPrims();
		float callFunc(const func_map::const_iterator&, const block_ptr&);
//		float callFunc(const func_map::const_iterator&, const block_ptr&, Interpreter&);
		
	public:
		Interpreter(const script::Script&);
		
		void start();
		float execute(const block_ptr&);
		std::vector<std::shared_ptr<Variable> > variables();
		std::vector<std::shared_ptr<Point> > points();
	};
	
}

#endif /* defined(__OmniScript__Interpreter__) */
