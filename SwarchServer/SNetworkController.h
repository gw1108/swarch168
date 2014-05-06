#pragma once
#include <SFML\Network.hpp>
#include "GameData.h"
#include <mutex>
#include <thread>
#include "LogIn.h"

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
	void waitForUsers(void);
	void removeDisconnectedClients(void);
	
	//static members
	static const float CHECK_FREQUENCY;

	LogIn* logInHandler;
	std::thread* m_networkThread;
	bool serverRunning;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;
	std::vector<sf::TcpSocket*> clientsToRemove;
	std::vector<GameData> m_data;
	std::mutex m_datalock;
};

