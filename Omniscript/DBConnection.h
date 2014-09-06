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
#include <initializer_list>

#include <cppconn/driver.h>
#include <cppconn/datatype.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

//#define DB_WRITE(query)	conn::DBConnection::get_connection().write(query)
//#define DB_READ(query) conn::DBConnection::get_connection().read(query)

namespace conn {
	class DBConnection final {
		DBConnection();
		DBConnection(const DBConnection&);
		DBConnection(DBConnection&&);
		DBConnection& operator=(const DBConnection&);
		DBConnection& operator=(DBConnection&&);
		
//		std::string concat(std::initializer_list<std::string>, const char = '\0');
		
		const std::string server_ = "tcp://localhost:3306";
		const std::string username_ = "root";
		const std::string password_ = "mandy24";
				
		sql::Driver* driver_;
		std::unique_ptr<sql::Connection> conn_;
		std::unique_ptr<sql::Statement> stmt_;
		std::shared_ptr<sql::ResultSet> res_;
		
	public:
		static DBConnection& get_connection();
		std::shared_ptr<sql::ResultSet> read(const std::string&);
		void write(const std::string&);
	};
}

inline void DB_WRITE(const std::string& query) {
	conn::DBConnection::get_connection().write(query);
}

inline std::shared_ptr<sql::ResultSet> DB_READ(const std::string& query) {
	return conn::DBConnection::get_connection().read(query);
}

#endif /* defined(__OmniScript__DBConnection__) */
