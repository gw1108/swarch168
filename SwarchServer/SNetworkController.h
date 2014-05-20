#pragma once
#include <SFML\Network.hpp>
#include "GameData.h"
#include <mutex>
#include <thread>
#include "LogIn.h"
#include <list>
#include "SPlayer.h"
#include "ServerData.h"
#include "Player.h"

class SNetworkController
{
public:
	SNetworkController();
	~SNetworkController(void);
	
	void startNetwork(void);
	void stopNetwork(void);
	ServerData getNextGameData(void);
	bool isGameDataAvailible(void);
	void sendGameUpdate(GameData data);

private:
	void run(void);
	void newConnections(sf::TcpSocket*& player);
	void updateConnections(void);
	int getNewClientNumber(void);
	void freeClientNumber(int clientNumber);
	void pushServerData(ServerData data);
	void sendNewPlayer(Player newPlayer);

	LogIn* logInHandler;
	std::thread* m_networkThread;
	bool serverRunning;
	sf::TcpListener listener;
	std::list<std::pair<sf::TcpSocket*, SPlayer>> clients;
	std::vector<ServerData> m_data;
	std::mutex m_datalock;
	int currentPlayerID;
	std::vector<bool> availableSpots;
};