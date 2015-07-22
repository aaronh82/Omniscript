//
//  DBConnection.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 7/28/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Logger.h"
#include "DBConnection.h"

#include <thread>
#include <chrono>

namespace conn {
/*
#pragma mark DBQueue
	
	template<typename Callable, typename... Args>
	void DBQueue::add(std::thread::id id, Callable&& f, Args&&... args) {
		m_.lock();
		std::function<sql::ResultSet*()> func = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
		funcs_.push(std::make_pair(id, func));
		m_.unlock();
	}
	
	void DBQueue::execute() {
		while (true) {
			while (!funcs_.empty()) {
				std::thread::id id = funcs_.front().first;
				std::function<sql::ResultSet*()> func = funcs_.front().second;
				std::shared_ptr<sql::ResultSet> res;
				try {
					res.reset(func());
				} catch (sql::SQLException e) {
					exceptions_.insert({id, e});
				}
				complete_.insert({id, res});
				funcs_.pop();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
	
	std::shared_ptr<sql::ResultSet> DBQueue::wait(std::thread::id id) {
		while (complete_.find(id) == complete_.end() &&
			   exceptions_.find(id) == exceptions_.end()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		};
		auto cId = complete_.find(id);
		std::shared_ptr<sql::ResultSet> res(nullptr);
		if (cId != complete_.end()) {
			res = complete_[id];
			complete_.erase(cId);
		}
		return res;
	}
	
	sql::SQLException DBQueue::except(std::thread::id id) {
		return exceptions_[id];
	}
	
	void DBQueue::delExcept(std::thread::id id) {
		exceptions_.erase(id);
	}
*/	
#pragma mark DBConnection
	
	DBConnection::DBConnection() {
		try {
			driver_ = get_driver_instance();
			connect();
		} catch (sql::SQLException e) {
			LOG(Log::ERROR, "Connection to database failed: " +
				std::string(e.what()));
		}
//		fut_ = std::async(std::launch::async, &DBQueue::execute, &commands_);
	}
	
	void DBConnection::connect() {
		conn_ = std::unique_ptr<sql::Connection>(driver_->connect(server_, username_, password_));
		conn_->setSchema("maverick_data");
		stmt_ = std::unique_ptr<sql::Statement>(conn_->createStatement());
	}

	DBConnection& DBConnection::get_connection() {
		
		static DBConnection conn;
		return conn;
	}
	
	std::shared_ptr<sql::ResultSet> DBConnection::read(const std::string& query) {
//		std::thread::id id = std::this_thread::get_id();
//		commands_.add(id, &sql::Statement::executeQuery, stmt_.get(), query);
//		res_ = commands_.wait(id);
		std::lock_guard<std::mutex> lock(mut_);
		sqlRes res;
		try {
			res = execute(query);
		} catch (sql::SQLException e) {
			LOG(Log::WARN, "Database read failed: " + std::string(e.what()));
			if (e.getErrorCode() == 2006 && conn_->isClosed()) {
                 	       connect();
			}
			return read(query);
		}
		return res;
//		sql::SQLException e = commands_.except(id);
//		commands_.delExcept(id);
//		LOG(Log::WARN, "Database read failed: " + std::string(e.what()));
//		if (e.getErrorCode() == 2006 && conn_->isClosed()) {
//			connect();
//		}
	}
	
	bool DBConnection::write(const std::string& query) {
//		std::thread::id id = std::this_thread::get_id();
//		commands_.add(id, &sql::Statement::executeQuery, stmt_.get(), query);
//		res_ = commands_.wait(id);
//		sql::SQLException e = commands_.except(id);
//		commands_.delExcept(id);
		std::lock_guard<std::mutex> lock(mut_);
		try {
			execute(query);
		} catch (sql::SQLException e) {
//			LOG(Log::WARN, "Failed to execute query: " + query + ": " + e.what());
			if (e.getErrorCode() == 2006 && conn_->isClosed()) {
				connect();
				return write(query);
			} else {
				return false;
			}
		}
//		if (e.getErrorCode() == 0) {
//			return true;
//		}
//		LOG(Log::WARN, "Failed to execute query: " + query + ": " + e.what());
//		if (e.getErrorCode() == 2006 && conn_->isClosed()) {
//			connect();
//		}
//		return false;
		return true;
	}

	DBConnection::sqlRes DBConnection::execute(const std::string& query) {
		try {
			return sqlRes(stmt_->executeQuery(query));
		} catch (sql::SQLException e) {
			throw;
		};
	}
}
