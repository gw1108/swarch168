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
	void waitForUsers(void);

	static const unsigned short DEFAULT_PORT;

private:
	bool serverIsRunning;
	unsigned short currentPort;
	LogIn* logInHandler;
	SNetworkController networkController;
};