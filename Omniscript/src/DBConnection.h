//
//  DBConnection.h
//  OmniScript
//
//  Created by Aaron Houghton on 7/28/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#ifndef __OmniScript__DBConnection__
#define __OmniScript__DBConnection__

#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <map>
#include <set>
#include <future>

#include <cppconn/driver.h>
#include <cppconn/datatype.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


namespace conn {
/*	typedef std::pair<std::thread::id, std::function<sql::ResultSet*()> > func_pair;
	
	class DBQueue {
		std::mutex m_;
		std::queue<func_pair> funcs_;
		std::map<std::thread::id, std::shared_ptr<sql::ResultSet> > complete_;
		std::map<std::thread::id, sql::SQLException> exceptions_;
		
	public:
		DBQueue() {}
		
		template<typename Callable, typename... Args>
		void add(std::thread::id, Callable&&, Args&&...);
		
		std::shared_ptr<sql::ResultSet> wait(std::thread::id);
		void execute();
		sql::SQLException except(std::thread::id);
		void delExcept(std::thread::id);
	};
*/	
	class DBConnection final {
		typedef std::shared_ptr<sql::ResultSet> sqlRes;

		DBConnection();
		DBConnection(const DBConnection&);
		DBConnection(DBConnection&&);
		DBConnection& operator=(const DBConnection&);
		DBConnection& operator=(DBConnection&&);
		
		const std::string server_ = "tcp://127.0.0.1:3306";
		const std::string username_ = "root";
		const std::string password_ = "mandy24";
				
		sql::Driver* driver_;
		std::unique_ptr<sql::Connection> conn_;
		std::unique_ptr<sql::Statement> stmt_;
		std::shared_ptr<sql::ResultSet> res_;
		
//		DBQueue commands_;
		std::future<void> fut_;
		std::mutex mut_;
		
		void connect();
		sqlRes execute(const std::string&);
	public:
		static DBConnection& get_connection();
		std::shared_ptr<sql::ResultSet> read(const std::string&);
		bool write(const std::string&);
	};
}

inline bool DB_WRITE(const std::string& query) {
	return conn::DBConnection::get_connection().write(query);
}

inline std::shared_ptr<sql::ResultSet> DB_READ(const std::string& query) {
	return conn::DBConnection::get_connection().read(query);
}

#endif /* defined(__OmniScript__DBConnection__) */
