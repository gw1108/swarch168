#include "ServerEngine.h"
#include <iostream>

using namespace std;

ServerEngine::ServerEngine(void)
	:serverIsRunning(false),
	networkController()
{
}

ServerEngine::~ServerEngine(void)
{
}

void ServerEngine::run(void)
{
	cout << "starting to run" << endl;
	serverIsRunning = true;
	networkController.startNetwork();

	while(serverIsRunning)
	{
		//play the game on server and update people
	}
	networkController.stopNetwork();
	cout << "finished running" << endl;
}

void ServerEngine::stop(void)
{
	cout << "server stopping" << endl;
	serverIsRunning = false;
}