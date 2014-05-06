#include "ServerEngine.h"
#include <iostream>

using namespace std;

ServerEngine::ServerEngine(void)
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
		//update people
	}
	networkController.stopNetwork();
	cout << "finished running" << endl;
}

void ServerEngine::stop(void)
{
	cout << "server stopping" << endl;
	serverIsRunning = false;
}