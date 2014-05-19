#pragma once
#include <SFML\Network.hpp>
#include "GameData.h"
#include <mutex>
#include <thread>
#include "LogIn.h"
#include <list>
#include "SPlayer.h"

class SNetworkController
{
public:
	SNetworkController(std::vector<SPlayer>& players);
	~SNetworkController(void);
	
	void startNetwork(void);
	void stopNetwork(void);
	GameData getNextGameData(void);
	bool isGameDataAvailible(void);
private:
	void run(void);
	void newConnections(sf::TcpSocket*& player);
	void updateConnections(void);
	int getNewClientNumber(void);
	void freeClientNumber(int clientNumber);

	LogIn* logInHandler;
	std::thread* m_networkThread;
	bool serverRunning;
	sf::TcpListener listener;
	std::list<std::pair<sf::TcpSocket*, SPlayer>> clients;
	std::vector<GameData> m_data;
	std::mutex m_datalock;
	int currentPlayerID;
	std::vector<bool> availableSpots;
};