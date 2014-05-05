#pragma once
#include <SFML\Network.hpp>
#include "GameData.h"
#include <mutex>
#include <thread>

class SNetworkController
{
public:
	SNetworkController(void);
	~SNetworkController(void);
	
	void startNetwork(void);
	void stopNetwork(void);
	GameData getNextGameData(void);
private:
	void listenUsers(void);
	void waitForUsers(void);
	
	std::thread m_networkThread;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;		//todo make this a POD struct
	std::vector<GameData> m_data;
	std::mutex m_datalock;
};

