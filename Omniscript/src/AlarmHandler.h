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
#include "md5.h"
#include "Interpreter.h"

#include <vector>
#include <ctime>
#include <string>
#include <random>
#include <functional>

#endif /* defined(__Omniscript__AlarmHandler__) */

using dist_t = std::uniform_real_distribution<>;

namespace interp {
	
	class Alarm {
	protected:
		const std::string alias_;
		const std::string level_;
		const std::string name_;
		const std::string description_;
		const unsigned int point_id_;
		const unsigned int device_id_;
		MD5 hash_;
		std::string token_;
		std::time_t date_created_;
		
	public:
		Alarm(const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const unsigned int,
		      const unsigned int);

		const std::string& alias() { return alias_; }
		const std::string& name() { return name_; }
		const std::string& description() { return description_; }
		const std::string& level() { return level_; }
		const std::string pointId() { return std::to_string(point_id_); }
		const std::string deviceId() { return std::to_string(device_id_); }
		const MD5& getHash() { return hash_; }
		void createHash(const std::string&);
		const std::string& getToken() { return token_; }
		void setToken(const std::string &);
		bool write(const std::string&);
		friend bool operator==(const Alarm&, const Alarm&);
	};
	
//	class PointAlarm: public Alarm {
//		Point point_;
//
//	public:
//		PointAlarm(const std::string&,
//		           const std::string&,
//		           const std::string&,
//		           const std::string&,
//		           const Point&);
//
//		const std::string& deviceId() { return std::to_string(point_.deviceId()); }
//		bool write(const std::string&);
//		friend bool operator==(const PointAlarm&, const PointAlarm&);
//	};
	
	class AlarmHandler final {
		AlarmHandler();
		AlarmHandler(const AlarmHandler&) = delete;
		AlarmHandler(AlarmHandler&&) = delete;
		AlarmHandler& operator=(const AlarmHandler&) = delete;
		AlarmHandler& operator=(AlarmHandler&&) = delete;

		std::mt19937 mt;
		dist_t dist;
		std::vector<std::shared_ptr<Alarm> > alarm_queue;
		bool alarmExists(const Alarm&);
		
	public:
		static AlarmHandler& getHandler();
		void add(std::shared_ptr<Alarm>);
		void clear(const std::string&,
		           const std::string&,
		           const std::string&,
		           const std::string&);
		void cleanUp();
	};
	
}