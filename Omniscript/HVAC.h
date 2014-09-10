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

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	
	class coolSetpoint: public BoolFunctor {
		int sp_, db_, in_;
	public:
		coolSetpoint(){}
		bool operator()(const block_ptr&);
	};
	
	class heatSetpoint: public BoolFunctor {
		int sp_, db_, in_;
	public:
		heatSetpoint(){}
		bool operator()(const block_ptr&);
	};
}

#endif /* defined(__Omniscript__HVAC__) */
