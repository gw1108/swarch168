#pragma once
#include "sqlite3.h"
#include <vector>
#include <string>

class Database
{
public:
	Database(char* filename);
	~Database(void);

	int open(char* filename);
	std::vector<std::vector<std::string>> query(std::string query);
	void close();
	std::vector<std::vector<std::string>> getUsersWhere(std::string usernameCondition);
	std::vector<std::vector<std::string>> insertUserInto(std::string userName, std::string password);
private:
	bool hasTable(std::string tableName);

	sqlite3* db;
};