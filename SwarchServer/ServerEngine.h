#pragma once
#include "Database.h"
#include "SFML\Network.hpp"
#include "LogIn.h"
#include "SNetworkController.h"

class ServerEngine
{
public:
	ServerEngine(void);
	~ServerEngine(void);
	void run(void);
	void stop(void);
private:
	bool serverIsRunning;
	std::vector<SPlayer> players;

	SNetworkController networkController;
	//vector of players
};