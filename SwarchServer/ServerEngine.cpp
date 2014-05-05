#include "ServerEngine.h"

using namespace std;

const unsigned short ServerEngine::DEFAULT_PORT = 53000;

ServerEngine::ServerEngine(void)
{
	currentPort = DEFAULT_PORT;
	logInHandler = new LogIn();
}

ServerEngine::~ServerEngine(void)
{
	delete logInHandler;
}


void ServerEngine::run(void)
{
	serverIsRunning = true;
	
	while(serverIsRunning)
	{
		//listen for people
		networkController.waitForUsers();

		//update people
	}
}
