#pragma once
#include "Database.h"
#include "SFML\Network.hpp"

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
	Database* db;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;		//todo make this a POD struct
};