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

namespace script {
	class Script;
}

namespace interp {

	class Interpreter {
		typedef std::map<std::string, std::shared_ptr<BaseFunctor> > func_map;
		func_map primTable;
		script::Script& program_;
		
		void initPrims();
		float callFunc(const func_map::const_iterator&, const block_ptr&);
		
	public:
		Interpreter(script::Script&);
		
		void start();
		float execute(const block_ptr&);
		const std::vector<std::shared_ptr<Variable> >& variables();
		const std::vector<std::shared_ptr<Point> >& points();
	};
	
}

#endif /* defined(__OmniScript__Interpreter__) */
