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
		float operator()(const block_ptr&);
	};
	
	class subtract: public FloatFunctor {
	public:
		subtract(){}
		float operator()(const block_ptr&);
	};
	
	class multiply: public FloatFunctor {
	public:
		multiply(){}
		float operator()(const block_ptr&);
	};
	
	class divide: public FloatFunctor {
	public:
		divide(){}
		float operator()(const block_ptr&);
	};
	
	class randomFromTo: public FloatFunctor {
	public:
		randomFromTo(){}
		float operator()(const block_ptr&);
	};
	
	class lessThan: public FloatFunctor {
	public:
		lessThan(){}
		float operator()(const block_ptr&);
	};
	
	class equalTo: public FloatFunctor {
	public:
		equalTo(){}
		float operator()(const block_ptr&);
	};
	
	class greaterThan: public FloatFunctor {
	public:
		greaterThan(){}
		float operator()(const block_ptr&);
	};
	
	class logicalAnd: public FloatFunctor {
	public:
		logicalAnd(){}
		float operator()(const block_ptr&);
	};
	
	class logicalOr: public FloatFunctor {
	public:
		logicalOr(){}
		float operator()(const block_ptr&);
	};
	
	class logicalNegation: public FloatFunctor {
	public:
		logicalNegation(){}
		float operator()(const block_ptr&);
	};
	
	class modulo: public FloatFunctor {
	public:
		modulo(){}
		float operator()(const block_ptr&);
	};
	
	class round: public FloatFunctor {
	public:
		round(){}
		float operator()(const block_ptr&);
	};
	
	class computeFunction: public FloatFunctor {
	public:
		computeFunction(){}
		float operator()(const block_ptr&);
	};
}

#endif /* defined(__Omniscript__Operators__) */
