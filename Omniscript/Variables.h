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
#include "Device.h"

#include <memory>
#include <vector>
#include <string>
#include <chrono>

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	
	class Variable {
		std::string name_;
		std::string type_;
		float value_;
		float prevValue_;
		std::chrono::system_clock::time_point lastCOV_;
		
	public:
		Variable(std::string, std::string, float);
		
		std::string name();
		std::string type();
		float value();
		float value(const float&);
		float prevValue();
		std::chrono::system_clock::time_point lastCOV();
	};
	
	class Point : public Variable {
		typedef std::shared_ptr<Device> dev_ptr;
		
		unsigned int point_id_;
		unsigned int type_id_;
		dev_ptr device_;
		int unwritten_count_;

	public:
		Point(const std::string&, const std::string&, float, unsigned int, dev_ptr);
		
		unsigned int pointId();
		unsigned int typeId();
		const dev_ptr device();
		unsigned int deviceId();
		std::string deviceName();
		unsigned int pathId();
		int unwritten_count();
		void inc_unwritten();
		void reset_unwritten();
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
