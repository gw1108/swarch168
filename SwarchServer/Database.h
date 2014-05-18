#pragma once
#include "sqlite3.h"
#include <vector>
#include <string>

class Database
{
public:
	Database(const char* filename = Database::DEFAULT_DATABASE_NAME.c_str());
	~Database(void);

	static const std::string DEFAULT_DATABASE_NAME;

	int open(const char* filename);
	std::vector<std::vector<std::string>> query(const std::string query);
	void close();
	std::vector<std::vector<std::string>> getUsersWhere(std::string usernameCondition);
	std::vector<std::vector<std::string>> insertUserInto(std::string userName, std::string password);
private:
	bool hasTable(const std::string tableName);
	std::vector<std::vector<std::string>> stepThroughQuery(sqlite3_stmt* statement);

	sqlite3* db;
};