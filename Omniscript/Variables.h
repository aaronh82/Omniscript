//
//  Variables.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Variables__
#define __Omniscript__Variables__

#include "Logger.h"
#include "Block.h"
#include "BaseFunctor.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	class setVar: public VoidFunctor {
	public:
		setVar(){}
		void operator()(const block_ptr&);
	};
	
	class changeVar: public VoidFunctor {
	public:
		changeVar(){}
		void operator()(const block_ptr&);
	};
	
	class setPoint: public VoidFunctor {
	public:
		setPoint(){}
		void operator()(const block_ptr&);
	};
	
	class changePoint: public VoidFunctor {
	public:
		changePoint(){}
		void operator()(const block_ptr&);
	};
}

#endif /* defined(__Omniscript__Variables__) */
