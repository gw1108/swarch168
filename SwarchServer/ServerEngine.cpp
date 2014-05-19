#include "ServerEngine.h"
#include <iostream>
#include <ctime>

using namespace std;

ServerEngine::ServerEngine(void)
	:serverIsRunning(false),
	players(),
	networkController(players)
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
	std::clock_t start;
	double duration;
	start = std::clock();
	while(serverIsRunning)
	{
		//play the game on server and update people
		duration = (std::clock() - start)/(double) CLOCKS_PER_SEC;
		if(duration > 10)
		{
			serverIsRunning = false;
		}
	}
	networkController.stopNetwork();
	cout << "finished running" << endl;
}

void ServerEngine::stop(void)
{
	cout << "server stopping" << endl;
	serverIsRunning = false;
}