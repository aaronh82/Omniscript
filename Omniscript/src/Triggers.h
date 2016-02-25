//
//  Primitives.h
//  OmniScript
//
//  Created by Aaron Houghton on 8/29/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__Primitives__
#define __OmniScript__Primitives__

#include "Logger.h"
#include "Block.h"
#include "BaseFunctor.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	class whenOn: public VoidFunctor {
	public:
		whenOn(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class whenSchedule: public VoidFunctor {
	public:
		whenSchedule(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class whenNotSchedule: public VoidFunctor {
	public:
		whenNotSchedule(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class whenBool: public VoidFunctor {
	public:
		whenBool(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class whenIReceive: public VoidFunctor {
	public:
		whenIReceive(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doBroadcast: public VoidFunctor {
	public:
		doBroadcast(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doBroadcastAndWait: public VoidFunctor {
	public:
		doBroadcastAndWait(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class getLastMessage: public VoidFunctor {
	public:
		getLastMessage(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doStopThis: public VoidFunctor {
	public:
		doStopThis(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class doStopOthers: public VoidFunctor {
	public:
		doStopOthers(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class reportGetSched: public BoolFunctor {
	public:
		reportGetSched(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
}

#endif /* defined(__OmniScript__Primitives__) */
