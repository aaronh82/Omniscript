//
//  HVAC.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__HVAC__
#define __Omniscript__HVAC__

#include "Logger.h"
#include "Block.h"
#include "BaseFunctor.h"

#include <string>

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	
	class coolSetpoint: public BoolFunctor {
	public:
		coolSetpoint(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class heatSetpoint: public BoolFunctor {
	public:
		heatSetpoint(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
}

#endif /* defined(__Omniscript__HVAC__) */
