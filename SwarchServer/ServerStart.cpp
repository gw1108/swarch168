#include "sqlite3.h"
#include <iostream>
#include "ServerEngine.h"

// TODO : Server actually doing something
int main(char* argc, char* argv[])
{
	//create server engine
	ServerEngine serverEngine = ServerEngine();

	while(true)
	{
		//server engine wait for people
		serverEngine.waitForUsers();

		//start server engine
		serverEngine.run();
	}
	return 0;
}