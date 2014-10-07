//
//  Logic.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Logic__
#define __Omniscript__Logic__

#include "Logger.h"
#include "Block.h"
#include "BaseFunctor.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	class waitFor: public VoidFunctor {
	public:
		waitFor(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doRepeat: public VoidFunctor {
	public:
		doRepeat(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doForever: public VoidFunctor {
	public:
		doForever(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doIf: public VoidFunctor {
	public:
		doIf(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doIfElse: public VoidFunctor {
	public:
		doIfElse(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doWaitUntil: public VoidFunctor {
	public:
		doWaitUntil(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doUntil: public VoidFunctor {
	public:
		doUntil(){};
		void operator()(const block_ptr&, Interpreter&);
	};
	
//	class stopScripts: public VoidFunctor {
//	public:
//		stopScripts(){};
//		void operator()(const block_ptr&, Interpreter&);
//	};
}

#endif /* defined(__Omniscript__Logic__) */
