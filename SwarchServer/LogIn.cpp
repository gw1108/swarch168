#include "LogIn.h"
#include <vector>
#include <Windows.h>
#include <wincrypt.h>
//#include <openssl\sha.h>
#include <iostream>

using namespace std;

const int LogIn::SALTSIZE = 32;
const int LogIn::KEYSIZE = 32;

LogIn::LogIn(void)
{
	database = new Database();
	//table will have 2 columns	UserName | Passwords
	//table will be called Users
}


LogIn::~LogIn(void)
{
	database->close();
	delete database;
}

//logins in the user
//if the user doesn't exist in the database automatically create a new user
//returns a logindata int code
GameData::LoginResponse LogIn::loginUser(string username, string password)
{
	//check user in database
	vector<vector<string>> output;
	output = database->getUsersWhere(username);

	if(output.empty())
	{
		//create the user
		//create a random salt
		//unsigned char* userSalt = getNewSalt();
		//TODO DELETE
		//userSalt = reinterpret_cast<BYTE*>("THESALT");
		//string hashedPassword = hashPassword(password, reinterpret_cast<char*>(userSalt));

		//string command = "INSERT INTO Users (UserName, Passwords, Salt) VALUES ('" + username + "','" + hashedPassword + "','";
		database->insertUserInto(username, password);
		//asuming no errors let the user log in
		return GameData::LoginResponse::NEW_ACCOUNT;
	}
	else
	{
		//user exists check credentials
		string dbPassword = output.at(0).at(1);
		//string userSalt = output.at(0).at(2);

		//string inputtedPassword = hashPassword(password, userSalt);
		if(slowEquals(password, dbPassword))
		{
			return GameData::LoginResponse::ACCEPTED;
		}
		else
		{
 			return GameData::LoginResponse::WRONG_PW;
		}
	}
}

unsigned char* LogIn::getNewSalt()
{
	//returns salt
	HCRYPTPROV hProvider = 0;
	BYTE pbData[SALTSIZE];

	!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);

	CryptGenRandom(hProvider, SALTSIZE, pbData);
	return pbData;
}

string LogIn::hashPassword(string password, string salt)
{
	//returns hashed password
	char buffer[65];
	//append password and salt together
	string appended = salt + password;
	sha256(appended.c_str(), buffer);

	//return password
	string newPassword = buffer;
	return newPassword;
}

//returns tru if the two strings are equal in length constant time
bool LogIn::slowEquals(string a, string b)
{
	int diff = 0;
	for(int i = 0; i < a.length() && i < b.length(); i++)
	{
		if(a.at(i) != b.at(i))
		{
			diff++;
		}
	}
	return diff == 0;
}

void LogIn::sha256(const char* inputString, char outputBuffer[65])
{
    /*unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, inputString, strlen(inputString));
    SHA256_Final(hash, &sha256);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf_s(outputBuffer + (i * 2), 65, "%02x", hash[i]);
    }
    outputBuffer[64] = 0;*/
}