#pragma once
#include "Database.h"
#include "SFML\Network.hpp"
#include "LogIn.h"
#include "SNetworkController.h"
#include "Pellet.h"
#include "GamePiece.h"
#include <unordered_map>

class ServerEngine
{
public:
	ServerEngine(void);
	~ServerEngine(void);
	void run(void);
	void stop(void);
private:
	void CheckPelletCollisions(GamePiece& playerPiece);
	void CheckWallCollisions(GamePiece& playerPiece);
	void CheckPlayerCollision(GamePiece& playerPiece);
	void UpdatePlayerDirection(ServerData data);
	GameData getCurrentGameData(void);

	bool serverIsRunning;
	std::unordered_map<int, GamePiece> players;

	SNetworkController networkController;
	//vector of players

	Pellet* gamePellets;
};