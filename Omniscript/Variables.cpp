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
#include "Device.h"

#include <algorithm>
#include <cmath>
#include <thread>

namespace interp {
#pragma mark Variable defs
	// Variable defs
	Variable::Variable(std::string name, std::string type, float val):
		name_(name), type_(type), value_(val), prevValue_(val),
		cov_timestamp_(std::time(nullptr)) {}
	
	std::string Variable::name() {
		return name_;
	}
	
	std::string Variable::type() {
		return type_;
	}
	
	float Variable::value() {
		return value_;
	}
	
	void Variable::value(const float f) {
		if (prevValue_ != value_) prevValue_ = value_;
		value_ = f;
	}
	
	float Variable::prevValue() {
		return prevValue_;
	}

	void Variable::prevValue(const float f) {
		prevValue_ = f;
	}
	
	std::time_t Variable::lastCOV() {
		return cov_timestamp_;
	}

	void Variable::updateCOV(std::time_t t) {
		cov_timestamp_ = t;
	}
	
#pragma mark Point defs
	// Point defs
	Point::Point(const std::string &name, const std::string &type, float val, unsigned int type_id, dev_ptr device):
		Variable(name, type, val), type_id_(type_id), device_(device), unwritten_count_(0) {
			std::string cmd = "SELECT point.pointid "
							  "FROM point "
							  "INNER JOIN device "
							  "ON point.deviceid=device.deviceid "
							  "WHERE device.deviceid='" +
									std::to_string(device_->id()) + "' "
							  "AND point.name='" + name + "'";
			std::shared_ptr<sql::ResultSet> res(DB_READ(cmd));
			if (res->next()) {
				point_id_ = res->getInt("pointid");
			}
		}
	
	unsigned int Point::pointId() {
		return point_id_;
	}

	void Point::pointId(const int id) {
		point_id_ = id;
	}
	
	unsigned int Point::typeId() {
		return type_id_;
	}

	const Point::dev_ptr Point::device() {
		return device_;
	}
	
	unsigned int Point::deviceId() {
		return device_->id();
	}
	
	std::string Point::deviceName() {
		return device_->name();
	}
	
	unsigned int Point::pathId() {
		return device_->path();
	}
	
	int Point::unwrittenCount() {
		return unwritten_count_;
	}
	
	void Point::incUnwritten() {
		++unwritten_count_;
	}
	
	void Point::resetUnwritten() {
		unwritten_count_ = 0;
	}

	std::time_t Point::lastPoll() {
		return poll_timestamp_;
	}

	void Point::updatePollTimestamp(std::time_t t) {
		poll_timestamp_ = t;
	}
	
#pragma mark Variable Functors
	// Variable Functors
	float readVariable::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::const_iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		LOG(Log::DEBUGGING, "readVariable(" + (*var)->name() + "): " +
			std::to_string(round((*var)->value())));
		return round((*var)->value());
	}
	
	void setVar::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Variable> >::const_iterator var = std::find_if(interp.variables().begin(), interp.variables().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		(*var)->value(x);
		LOG(Log::DEBUGGING, "setVar(" + (*var)->name() + "): " +
			std::to_string(x));
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
	time_t convertTime(const std::string& sql_time) {
		struct tm time_info;
		strptime(sql_time.c_str(), "%Y-%m-%d %H:%M:%S", &time_info);
		time_info.tm_isdst = -1;
		return mktime(&time_info);
	}
	
	void readPnt(Point& p) {
		const std::string query = "SELECT point.pointid, point.val, point.prevval, point.poll_timestamp, point.cov_timestamp "
								  "FROM point "
								  "INNER JOIN device "
								  "ON point.deviceid=device.deviceid "
								  "WHERE device.name='" + p.deviceName() + "'"
								  "AND point.name='" + p.name() + "'";
		try {
			std::shared_ptr<sql::ResultSet> res = DB_READ(query);
			if (res->next()) {
				if (res->isNull("val")) {
					LOG(Log::INFO, "readPoint(" + p.name() + ") is NULL");
					return;
				} else {
					LOG(Log::DEBUGGING, "readPoint(" + p.name() + "): " +
						std::to_string(res->getDouble("val")));
					p.pointId(res->getInt("pointid"));
					p.value(res->getDouble("val"));
					p.prevValue(res->getDouble("prevval"));
					p.updatePollTimestamp(convertTime(res->getString("poll_timestamp")));
					p.updateCOV(convertTime(res->getString("cov_timestamp")));
				}
			}
		} catch (sql::SQLException e) {
			readPnt(p);
		}
	}
	
	float readPoint::operator()(const block_ptr &b, Interpreter &interp) {
		const std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[0]));
		readPnt(**point);
		return (*point)->value();
	}
	
	void setPoint::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<std::shared_ptr<Point> > points = interp.points();
		std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(points.begin(), points.end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		readPnt(**point);
		if (!(*point)->device()->isOffline()) {
			if ((fabsf(x - (*point)->value()) > 0.001 || (*point)->unwrittenCount() == 100) ||
				(*point)->device()->prevState() == DeviceStatus::Offline) {
				writePointValue(x, point);
				(*point)->resetUnwritten();
				(*point)->device()->prevState(DeviceStatus::Online);
			} else {
				(*point)->incUnwritten();
			}
		}
		LOG(Log::DEBUGGING, "setPoint(" + (*point)->name() + "): " +
			std::to_string(x));
	}
	
	void changePoint::operator()(const block_ptr &b, Interpreter &interp) {
		std::vector<std::shared_ptr<Point> >::const_iterator point = std::find_if(interp.points().begin(), interp.points().end(), findVariable(b->args()[0]));
		float x = (b->args()[1].find("block:") != std::string::npos) ?
					interp.execute(b->blockArgs()[stoi(b->args()[1].substr(b->args()[1].find(":") + 1))]) :
					stof(b->args()[1]);
		readPnt(**point);
		writePointValue(x + (*point)->value(), point);
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

	bool operator==(const Point &lhs, const Point &rhs) {
		return (lhs.name_ == rhs.name_ && lhs.type_id_ == rhs.type_id_ &&
			lhs.device_ == rhs.device_ && lhs.point_id_ == rhs.point_id_);
	}

}