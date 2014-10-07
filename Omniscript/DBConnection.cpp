//
//  DBConnection.cpp
//  OmniScript
//
//  Created by Aaron Houghton on 7/28/14.
//  Copyright (c) 2014 CCBAC. All rights reserved.
//

#include "Logger.h"
#include "DBConnection.h"

namespace conn {
	
	DBConnection::DBConnection() {
		try {
			driver_ = get_driver_instance();
			conn_ = std::unique_ptr<sql::Connection>(driver_->connect(server_, username_, password_));
			conn_->setSchema("maverick_data");
			stmt_ = std::unique_ptr<sql::Statement>(conn_->createStatement());
		} catch (sql::SQLException e) {
			LOG(Log::ERROR, "Connection to database failed: " + std::string(e.what()));
		}
	}

	DBConnection& DBConnection::get_connection() {
		
		static DBConnection conn;
		return conn;
	}
	
	std::shared_ptr<sql::ResultSet> DBConnection::read(const std::string& query) {
		
		try {
			std::shared_ptr<sql::ResultSet> res(stmt_->executeQuery(query));
			res_ = res;
		} catch (sql::SQLException e) {
			LOG(Log::WARN, "Database read failed: " + std::string(e.what()));
		}
		return res_;
	}
	
	void DBConnection::write(const std::string& query) {
		try {
			stmt_->execute(query);
		} catch (sql::SQLException e) {
			LOG(Log::WARN, "Failed to execute query: " + query + ": " + e.what());
		}
//		std::string fields_str(concat(fields));
//		std::string values_str(concat(values, '\''));
//		if (fields.size() != values.size()) {
//			LOG(Log::WARN, "Field(" + fields_str + ") and Value(" + values_str +
//				") arguments do not match in size.");
//			return;
//		}
//		try {
//			LOG(Log::DEBUGGING, "Writing to DB now.");
//			std::unique_ptr<sql::PreparedStatement> pstmt;
//			pstmt.reset(conn_->prepareStatement("INSERT INTO ?(?) VALUES (?)"));
//			pstmt->setString(1, table);
//			pstmt->setString(2, "name");
//			pstmt->setString(3, "new_test");
//			pstmt->execute();
//		} catch (sql::SQLException e) {
//			LOG(Log::ERROR, std::string("Write to database failed: %s", e.what()));
//		}
	}
	
//	std::string DBConnection::concat(std::initializer_list<std::string> list, const char c) {
//		std::string res;
//		for (std::initializer_list<std::string>::const_iterator it = list.begin();
//			 it != list.end(); ++it) {
//			if (it != list.begin()) {
//				res += ", ";
//			}
//			res += (c + *it + c);
//		}
//		return res;
//	}
	
}