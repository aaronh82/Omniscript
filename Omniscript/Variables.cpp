//
//  Variables.cpp
//  Omniscript
//
//  Created by Aaron Houghton on 9/1/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Variables.h"
#include "DBConnection.h"
#include "Interpreter.h"

#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>
#include <unistd.h>

namespace interp {
#pragma mark Variable defs
	// Variable defs
	Variable::Variable(std::string name, std::string type, float val):
		name_(name), type_(type), value_(val) {}
	
	std::string Variable::name() {
		return name_;
	}
	
	std::string Variable::type() {
		return type_;
	}
	
	float Variable::value() {
		return value_;
	}
	
	float Variable::value(const float &f) {
		value_ = f;
		return value_;
	}
	
#pragma mark Point defs
	// Point defs
	Point::Point(const std::string &name, const std::string &type, float val, unsigned int type_id, unsigned int device_id, const std::string &device_name, unsigned int path_id):
		Variable(name, type, val), type_id_(type_id), device_id_(device_id), device_name_(device_name), path_id_(path_id) {
			std::string cmd = "SELECT point.pointid "
							  "FROM point "
							  "INNER JOIN device "
							  "ON point.deviceid=device.deviceid "
							  "WHERE device.name='" + device_name + "' "
							  "AND point.name='" + name + "'";
			std::shared_ptr<sql::ResultSet> res(DB_READ(cmd));
			if (res->next()) {
				point_id_ = res->getInt("pointid");
			}
		}
	
	unsigned int Point::pointId() {
		return point_id_;
	}
	
	unsigned int Point::typeId() {
		return type_id_;
	}
	
	unsigned int Point::deviceId() {
		return device_id_;
	}
	
	std::string Point::deviceName() {
		return device_name_;
	}
	
	unsigned int Point::pathId() {
		return path_id_;
	}
	
#pragma mark Variable Functors
	// Variable Functors
	float readVariable::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::const_iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		LOG(Log::DEBUGGING, "readVariable: " + std::to_string(round((*var)->value())));
		return round((*var)->value());
	}
	
	void setVar::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::const_iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		(*var)->value(x);
//		LOG(Log::DEBUGGING, "setVar");
	}
	
	void changeVar::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::const_iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		(*var)->value((*var)->value() + x);
		LOG(Log::DEBUGGING, "changeVar");
	}
	
#pragma mark Point Functors
	// Point Functors
	float readPnt(std::string dName, std::string pName) {
		const std::string query = "SELECT point.val "
								  "FROM point "
								  "INNER JOIN device "
								  "ON point.deviceid=device.deviceid "
								  "WHERE device.name='" + dName + "'"
								  "AND point.name='" + pName + "'";
		try {
			std::shared_ptr<sql::ResultSet> res = DB_READ(query);
			if (res->next()) {
				LOG(Log::DEBUGGING, "readPoint: " +
					std::to_string(res->getDouble("val")));
				return res->getDouble("val");
			}
		} catch (sql::SQLException e) {
			return readPnt(dName, pName);
		}
	}
	
	float readPoint::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[0]));
		return readPnt((*point)->deviceName(), (*point)->name());
	}
	
	void setPoint::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<std::shared_ptr<Point> > points = interp.points();
		std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(points.begin(), points.end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		float cmp = readPnt((*point)->deviceName(), (*point)->name());
		if (fabsf(x - cmp) > 0.01 && int(x) != int(cmp)) {
			writePointValue(x, point);
			sleep(1);
		}
		LOG(Log::DEBUGGING, "setPoint");
	}
	
	void changePoint::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		writePointValue(x + readPnt((*point)->deviceName(), (*point)->name()), point);
		sleep(1);
		LOG(Log::DEBUGGING, "changePoint");
	}
	
#pragma mark Helper Functions
	// Helper functions
	bool findVariable::operator()(std::shared_ptr<Variable> var) {
		return var->name().compare(name_) == 0;
	}
	
	void writePointValue(const float &x, std::vector<std::shared_ptr<Point> >::const_iterator p) {
		DB_WRITE("INSERT INTO commandqueue (commandqueuestatusid, name, pointid,"
				 " priorityid, val, is_override, timeout, issued_userid, issued_ts)"
				 " VALUES (1, 'Omniscript Command', '"
				 + std::to_string((*p)->pointId()) + "', 0, '"
				 + std::to_string(x) + "', 0, 180, 0, NOW())");
	}
}
