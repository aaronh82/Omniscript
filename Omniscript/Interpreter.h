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
#include "Triggers.h"
#include "Logic.h"
#include "Operators.h"
#include "HVAC.h"
#include "Variables.h"
#include "Alarms.h"
#include "Logger.h"
#include "DBConnection.h"
#include "Device.h"

using block_ptr = blocks::Block::block_ptr;

namespace script {
	class Script;
}

namespace interp {
	typedef std::shared_ptr<Variable> var_ptr;
	typedef std::shared_ptr<Point> point_ptr;
	typedef std::shared_ptr<Device> dev_ptr;

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
		const std::vector<var_ptr>& variables();
		const std::vector<point_ptr>& points();
		const std::vector<dev_ptr>& devices();
	};
	
}

#endif /* defined(__OmniScript__Interpreter__) */
