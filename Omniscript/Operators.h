//
//  Operators.h
//  Omniscript
//
//  Created by Aaron Houghton on 9/2/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Operators__
#define __Omniscript__Operators__

#include "Logger.h"
#include "Block.h"
#include "BaseFunctor.h"

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	class add: public FloatFunctor {
	public:
		add(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class subtract: public FloatFunctor {
	public:
		subtract(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class multiply: public FloatFunctor {
	public:
		multiply(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class divide: public FloatFunctor {
	public:
		divide(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class randomFromTo: public IntFunctor {
	public:
		randomFromTo(){}
		int operator()(const block_ptr&, Interpreter&);
	};
	
	class lessThan: public BoolFunctor {
	public:
		lessThan(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class equalTo: public BoolFunctor {
	public:
		equalTo(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class greaterThan: public BoolFunctor {
	public:
		greaterThan(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class logicalAnd: public BoolFunctor {
	public:
		logicalAnd(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class logicalOr: public BoolFunctor {
	public:
		logicalOr(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class logicalNegation: public BoolFunctor {
	public:
		logicalNegation(){}
		bool operator()(const block_ptr&, Interpreter&);
	};
	
	class modulo: public IntFunctor {
	public:
		modulo(){}
		int operator()(const block_ptr&, Interpreter&);
	};
	
	class roundToNearest: public FloatFunctor {
	public:
		roundToNearest(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class computeFunction: public FloatFunctor {
	public:
		computeFunction(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class maxOf: public VoidFunctor {
	public:
		maxOf(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class minOf: public VoidFunctor {
	public:
		minOf(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class avg: public VoidFunctor {
	public:
		avg(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class getValueOf: public FloatFunctor {
	public:
		getValueOf(){}
		float operator()(const block_ptr&, Interpreter&);
	};
}

#endif /* defined(__Omniscript__Operators__) */
