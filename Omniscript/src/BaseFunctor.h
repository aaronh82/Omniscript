//
//  BaseFunctor.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/9/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__BaseFunctor__
#define __Omniscript__BaseFunctor__

#include "Block.h"

#include <memory>

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	class Interpreter;
	
	class BaseFunctor {
	public:
		BaseFunctor(){}
		virtual ~BaseFunctor(){};
	};
	
	class VoidFunctor: public BaseFunctor {
	public:
		VoidFunctor(){}
		virtual void operator()(const block_ptr&, Interpreter&) = 0;
	};
	
	class BoolFunctor: public BaseFunctor {
	public:
		BoolFunctor(){}
		virtual bool operator()(const block_ptr&, Interpreter&) = 0;
	};
	
	class IntFunctor: public BaseFunctor {
	public:
		IntFunctor(){}
		virtual int operator()(const block_ptr&, Interpreter&) = 0;
	};
	
	class FloatFunctor: public BaseFunctor {
	public:
		FloatFunctor(){}
		virtual float operator()(const block_ptr&, Interpreter&) = 0;
	};
}

#endif /* defined(__Omniscript__BaseFunctor__) */
