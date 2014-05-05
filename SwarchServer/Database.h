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
	std::vector<std::vector<std::string>> query(char* query);
	std::vector<std::vector<std::string>> query(std::string query);

	void close();
	bool hasTable(std::string tableName);
private:
	sqlite3* db;
};