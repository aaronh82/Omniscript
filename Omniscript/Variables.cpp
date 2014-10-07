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
	Point::Point(std::string name, std::string type, float val, uint point_id, uint type_id, uint device_id, uint path_id):
		Variable(name, type, val), point_id_(point_id), type_id_(type_id), device_id_(device_id), path_id_(path_id) {}
	
	uint Point::pointId() {
		return point_id_;
	}
	
	uint Point::typeId() {
		return type_id_;
	}
	
	uint Point::deviceId() {
		return device_id_;
	}
	
	uint Point::pathId() {
		return path_id_;
	}
	
#pragma mark Variable Functors
	// Variable Functors
	float readVariable::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::const_iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		LOG(Log::DEBUGGING, "readVariable: " + std::to_string((*var)->value()));
		return (*var)->value();
	}
	
	void setVar::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		(*var)->value(x);
		LOG(Log::DEBUGGING, "setVar");
	}
	
	void changeVar::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		(*var)->value((*var)->value() + x);
		LOG(Log::DEBUGGING, "changeVar");
	}
	
#pragma mark Point Functors
	// Point Functors
	float readPoint::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[0]));
		const std::string query = "SELECT val FROM point WHERE pointid='" + std::to_string((*point)->pointId()) + "'";
		std::shared_ptr<sql::ResultSet> res = DB_READ(query);
		LOG(Log::DEBUGGING, "readPoint: " + std::to_string(res->getDouble("val")));
		return res->getDouble("val");
	}
	
	void setPoint::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<std::shared_ptr<Point> >::iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[1]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		writePointValue(x, point);
		LOG(Log::DEBUGGING, "setPoint");
	}
	
	void changePoint::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<std::shared_ptr<Point> >::iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[1]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		writePointValue(x, point);
		LOG(Log::DEBUGGING, "changePoint");
	}
	
#pragma mark Helper Functions
	// Helper functions
	bool findVariable::operator()(std::shared_ptr<Variable> var) {
		return var->name().compare(name_) == 0;
	}
	
	void writePointValue(const float &x, std::vector<std::shared_ptr<Point> >::iterator p) {
		DB_WRITE("INSERT INTO command_queue (pointid, val, issued_ts) VALUES ('" + std::to_string((*p)->pointId()) + "', '" + std::to_string(x) + "', NOW())");
	}
}