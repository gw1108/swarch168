#pragma once
#include <SFML\Network.hpp>
#include "GameData.h"
#include <mutex>
#include <thread>
#include "LogIn.h"
#include <list>

class SNetworkController
{
public:
	SNetworkController(void);
	~SNetworkController(void);
	
	void startNetwork(void);
	void stopNetwork(void);
	GameData getNextGameData(void);
	bool isGameDataAvailible(void);
private:
	void run(void);
	void newConnections(sf::TcpSocket* socket);
	void updateConnections(void);
	
	//static members
	static const float CHECK_FREQUENCY;

	LogIn* logInHandler;
	std::thread* m_networkThread;
	bool serverRunning;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::list<sf::TcpSocket*> clients;
	std::vector<GameData> m_data;
	std::mutex m_datalock;
};