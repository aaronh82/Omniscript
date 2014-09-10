//
//  Alarms.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Alarms__
#define __Omniscript__Alarms__

#include "Logger.h"
#include "Block.h"
#include "BaseFunctor.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	
	class highTempLimit: public VoidFunctor {
	public:
		highTempLimit(){}
		void operator()(const block_ptr&);
	};
	
	class lowTempLimit: public VoidFunctor {
	public:
		lowTempLimit(){}
		void operator()(const block_ptr&);
	};
}

#endif /* defined(__Omniscript__Alarms__) */
