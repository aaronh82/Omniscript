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

#include <memory>
#include <vector>
#include <string>

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	
	class Variable {
		std::string name_;
		std::string type_;
		float value_;
	public:
		Variable(std::string, std::string, float);
		
		std::string name();
		std::string type();
		float value();
		float value(const float&);
	};
	
	class Point : public Variable {
		uint16_t point_id_;
		uint16_t type_id_;
		uint16_t device_id_;
		uint16_t path_id_;
	public:
		Point(std::string, std::string, float, uint, uint, uint, uint);
		
		uint pointId();
		uint typeId();
		uint deviceId();
		uint pathId();
	};
	
	class readVariable: public FloatFunctor {
	public:
		readVariable(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class setVar: public VoidFunctor {
	public:
		setVar(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class changeVar: public VoidFunctor {
	public:
		changeVar(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class readPoint: public FloatFunctor {
	public:
		readPoint(){}
		float operator()(const block_ptr&, Interpreter&);
	};
	
	class setPoint: public VoidFunctor {
	public:
		setPoint(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class changePoint: public VoidFunctor {
	public:
		changePoint(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class findVariable {
		std::string name_;
	public:
		findVariable(const std::string& name): name_(name) {}
		bool operator()(std::shared_ptr<Variable>);
	};
	
	void writePointValue(const float&, std::vector<std::shared_ptr<Point> >::iterator);
}

#endif /* defined(__Omniscript__Variables__) */
