#pragma once
#include "Database.h"
#include "SFML\Network.hpp"
#include "LogIn.h"
#include "SNetworkController.h"
#include "Pellet.h"
#include "GamePiece.h"
#include "Player.h"
#include <unordered_map>

class ServerEngine
{
public:
	ServerEngine(void);
	~ServerEngine(void);
	void run(void);
	void stop(void);
private:
	void CheckPelletCollisions(Player& player);
	void CheckWallCollisions(Player& player);
	void CheckPlayerCollision(Player& player);
	void UpdatePlayerDirection(ServerData data);
	GameData getCurrentGameData(void);

	bool serverIsRunning;
	std::unordered_map<int, Player> players;

	SNetworkController networkController;
	//vector of players

	Pellet* gamePellets;
};