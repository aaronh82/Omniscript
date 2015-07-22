//
//  AlarmHandler.h
//  Omniscript
//
//  Created by Aaron Houghton on 10/31/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __Omniscript__AlarmHandler__
#define __Omniscript__AlarmHandler__

#include "Variables.h"

#include <vector>
#include <ctime>
#include <string>

#endif /* defined(__Omniscript__AlarmHandler__) */

namespace interp {
	
	class Alarm {
	protected:
		std::string key_;
		const std::string type_;
		const std::string name_;
		const std::string description_;
		std::time_t date_created_;
		bool written_;
		
	public:
		Alarm(const std::string&, const std::string&, const std::string&);
		
		virtual bool write();
		std::string key();
		friend bool operator==(const Alarm&, const Alarm&);
	};
	
	class PointAlarm: public Alarm {
		Point point_;
		
	public:
		PointAlarm(const std::string&, const std::string&, const std::string&, const Point&);
		
		bool write();
		friend bool operator==(const Alarm&, const Alarm&);
	};
	
	class AlarmHandler final {
		AlarmHandler();
		AlarmHandler(const AlarmHandler&) = delete;
		AlarmHandler(AlarmHandler&&) = delete;
		AlarmHandler& operator=(const AlarmHandler&) = delete;
		AlarmHandler& operator=(AlarmHandler&&) = delete;
		
		std::vector<std::shared_ptr<Alarm> > alarm_queue;
		bool alarmExists(const Alarm&);
		
	public:
		static AlarmHandler& getHandler();
		void add(std::shared_ptr<Alarm>);
		void cleanUp();
	};
	
}