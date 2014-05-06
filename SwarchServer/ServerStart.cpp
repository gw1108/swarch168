#include <iostream>
#include "ServerEngine.h"

// TODO : Server actually doing something
int main(char* argc, char* argv[])
{
	//create server engine
	ServerEngine serverEngine = ServerEngine();

	//start server engine
	serverEngine.run();

	return 0;
}