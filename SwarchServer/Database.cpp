#include "Database.h"
#include <iostream>

using namespace std;

const std::string Database::DEFAULT_DATABASE_NAME = "Database.db";

Database::Database(const char* filename)
{
	db = nullptr;
	open(filename);
	if(!hasTable("Users"))
	{
		cout << "Table not found. New table created : " << endl; 
		query("CREATE TABLE Users (UserName varchar(255), Passwords varchar(255))");//, Salt VARBINARY(255));");
	}
}


Database::~Database(void)
{
}

int Database::open(const char* filename)
{
	return sqlite3_open(filename, &db);
}

//returns a vector<vector<string>> with each vector<string> being a row
//ie SELECT a, b from table_name
//returns vector<string> row
//a value : row.at(0) | b value : row.at(1)
std::vector<std::vector<std::string>> Database::query(const string query)
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

//returns true if table exists
bool Database::hasTable(const string tableName)
{
	sqlite3_stmt* statement;
	int rc;
	string query = "SELECT 1 from " + tableName + " LIMIT 1;";
	//if rc == 0 then table exists
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
	return rc == 0;
}

vector<vector<string>> Database::getUsersWhere(string usernameCondition)
{
	return query("SELECT * FROM Users WHERE UserName = '" + usernameCondition + "';");
}

vector<vector<string>> Database::insertUserInto(string userName, string userPassword)
{
	sqlite3_stmt* statement;
	vector<vector<string>> results;
	const char* cQuery = "insert into Users (UserName, Passwords) VALUES (?,?)";

	if(sqlite3_prepare_v2(db, cQuery, -1, &statement, 0) == SQLITE_OK)
	{

		sqlite3_bind_text(statement, 1, userName.c_str(), strlen(userName.c_str()), 0);
		sqlite3_bind_text(statement, 2, userPassword.c_str(), strlen(userPassword.c_str()), 0);
		results = stepThroughQuery(statement);
	}
	
	string error = sqlite3_errmsg(db);
	if(error != "not an error")
		cout << "ERROR: " << error << endl;

	return results;
}

vector<vector<string>> Database::stepThroughQuery(sqlite3_stmt* statement)
{
	vector<vector<string>> results;
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
	return results;
}