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

#include <string>

using block_ptr = blocks::Block::block_ptr;

namespace interp {
	
	class highTempLimit: public VoidFunctor {
	public:
		highTempLimit(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	class lowTempLimit: public VoidFunctor {
	public:
		lowTempLimit(){}
		void operator()(const block_ptr&, Interpreter&);
	};
	
	struct Alarm {
		std::string name;
		uint alarm_id;
		std::string type;
		uint path_id;
		uint device_id;
		uint point_id;
		time_t date_created;
	};
}

#endif /* defined(__Omniscript__Alarms__) */
