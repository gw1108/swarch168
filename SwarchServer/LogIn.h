#pragma once
#include "Database.h"

class LogIn
{
public:
	LogIn(void);
	~LogIn(void);

	bool loginUser(std::string username, std::string password);
private:
	unsigned char* getNewSalt();
	std::string hashPassword(std::string password, std::string salt);
	static bool slowEquals(std::string a, std::string b);
	void sha256(char* inputString, char outputBuffer[65]);
	
	Database* database;
	static const int SALTSIZE;
	static const int KEYSIZE;
};

