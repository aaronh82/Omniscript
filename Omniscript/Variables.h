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
		unsigned int point_id_;
		unsigned int type_id_;
		unsigned int device_id_;
		std::string device_name_;
		unsigned int path_id_;
	public:
		Point(const std::string&, const std::string&, float, unsigned int, unsigned int, const std::string&, unsigned int);
		
		unsigned int pointId();
		unsigned int typeId();
		unsigned int deviceId();
		std::string deviceName();
		unsigned int pathId();
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
	
	void writePointValue(const float&, std::vector<std::shared_ptr<Point> >::const_iterator);
}

#endif /* defined(__Omniscript__Variables__) */
