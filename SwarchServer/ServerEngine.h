#pragma once
#include "Database.h"
#include "SFML\Network.hpp"
#include "LogIn.h"

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
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;		//todo make this a POD struct
};