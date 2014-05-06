#include "Database.h"
#include <iostream>

using namespace std;
Database::Database(char* filename)
{
	db = nullptr;
	open(filename);
}


Database::~Database(void)
{
}

int Database::open(char* filename)
{
	return sqlite3_open(filename, &db);
}

//returns a vector<vector<string>> with each vector<string> being a row
//ie SELECT a, b from table_name
//returns vector<string> row
//a value : row.at(0) | b value : row.at(1)
std::vector<std::vector<std::string>> Database::query(string query)
{
	sqlite3_stmt* statement;
	vector<vector<string>> results;
	const char* cQuery = query.c_str();

	if(sqlite3_prepare_v2(db, cQuery, -1, &statement, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		do
		{
			result = sqlite3_step(statement);
			if(result == SQLITE_ROW)
			{
				vector<string> values;
				for(int col = 0; col < cols; col++)
				{
					char* ptr = (char*)sqlite3_column_text(statement, col);
					if(ptr)
					{
						values.push_back(ptr);
					}
					else
					{
						values.push_back("");
					}
				}
				results.push_back(values);
			}
		}
		while(result == SQLITE_ROW);
		sqlite3_finalize(statement);
	}
	
	string error = sqlite3_errmsg(db);
	if(error != "not an error")
		cout << query << " " << error << endl;

	return results;
}

void Database::close()
{
	sqlite3_close(db);
}

bool Database::hasTable(string tableName)
{
	sqlite3_stmt* statement;
	int rc;
	string query = "SELECT 1 from " + tableName + " LIMIT 1;";
	//if rc == 0 then table exists
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
	return rc == 0;
}