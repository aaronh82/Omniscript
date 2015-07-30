//
//  Device.h
//  Omniscript
//
//  Created by Aaron Houghton on 7/22/15.
//  Copyright (c) 2015 CCBAC. All rights reserved.
//

#ifndef __Omniscript__Device__
#define __Omniscript__Device__

#include <string>

namespace interp {
	
	enum class DeviceStatus {
		Online,
		Offline,
		Error,
		Deleted
	};

	class Device {
		std::string name_;
		unsigned int id_;
		unsigned int path_;
		
		DeviceStatus state_;
		DeviceStatus prevState_;
		
	public:
		Device(const std::string& name, unsigned int id, unsigned int path):
		name_(name), id_(id), path_(path), state_(DeviceStatus::Offline), prevState_(DeviceStatus::Offline) {}
		
		const std::string& name() { return name_; }
		const unsigned int& id() { return id_; }
		const unsigned int& path() { return path_; }
		
		bool isOffline() const { return state_ == DeviceStatus::Offline; }
		const DeviceStatus& state() const { return state_; }
		void state(DeviceStatus state) { state_ = state; }
		const DeviceStatus& prevState() const { return prevState_; }
		void prevState(DeviceStatus state) { prevState_ = state; }
		
	};
}

#endif /* defined(__Omniscript__Device__) */
