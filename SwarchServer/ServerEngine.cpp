#include "ServerEngine.h"
#include <iostream>

using namespace std;

ServerEngine::ServerEngine(void)
	:serverIsRunning(false),
	players(),
	networkController()
{
	//setup game pellets
	gamePellets = new Pellet[GameData::MAX_PELLETS];
	for(int i = 0; i < GameData::MAX_PELLETS; ++i)
	{
		gamePellets[i].Spawn();
	}
}

ServerEngine::~ServerEngine(void)
{
	delete[] gamePellets;
}

void ServerEngine::run(void)
{
	cout << "starting to run" << endl;
	serverIsRunning = true;
	networkController.startNetwork();
	sf::Clock engineClock;

	while(serverIsRunning)
	{
		if(engineClock.getElapsedTime().asMilliseconds() <= GameData::ENGINE_SPEED)
		{
			continue;
		}
		else
		{
			engineClock.restart();
			//update player directions
			while(networkController.isGameDataAvailible())
			{
				ServerData data = networkController.getNextGameData();
				if(data.m_command == ServerData::ServerCommands::PLAYER_UPDATE)
				{
					UpdatePlayerDirection(data);
					cout << "Recieved update from player " << data.m_playerNumber << endl;
				}
				else if(data.m_command == ServerData::ServerCommands::LOG_IN)
				{
					GamePiece newPlayer(data.m_playerNumber);
					players.insert(std::make_pair(data.m_playerNumber, newPlayer));
					cout << "Player " << data.m_playerNumber << " logged in to engine" << endl;
				}
				else if(data.m_command == ServerData::ServerCommands::LOG_OUT)
				{
					players.erase(data.m_playerNumber);
					cout << "Player " << data.m_playerNumber << " logged out of engine" << endl;
				}
			}

			//update people
			for(auto it = players.begin(); it != players.end(); ++it)
			{
				(*it).second.TakeTurn();

				//check for collision against pellets and walls
				CheckPelletCollisions((*it).second);

				CheckWallCollisions((*it).second);

				CheckPlayerCollision((*it).second);
			}

			//send update to players
			networkController.sendGameUpdate(getCurrentGameData());
		}

	}
	networkController.stopNetwork();
	cout << "finished running" << endl;
}

void ServerEngine::stop(void)
{
	cout << "server stopping" << endl;
	serverIsRunning = false;
}

// ===== CheckPelletCollisions ====================================================================
// Method will check if the player's piece has collided with any of the pellets.
//
// Input: none
// Output: none
// ================================================================================================
void ServerEngine::CheckPelletCollisions(GamePiece& playerPiece)
{
	sf::FloatRect playerBounds = playerPiece.getGlobalBounds();
	sf::FloatRect pelletBounds;

	for(int pIndex = 0; pIndex < GameData::MAX_PELLETS; pIndex++)
	{
		pelletBounds = gamePellets[pIndex].getGlobalBounds();

		if(playerBounds.intersects(pelletBounds))
		{
			playerPiece.Grow();
			gamePellets[pIndex].Spawn();
		}
	}
}

// ===== CheckWallCollisions ======================================================================
// Method will check if the assigned players piece has collided with any of the walls. If a the 
// player has collided, it will reset the piece and assign it a random position.
//
// Input: none
// Output: none
// ================================================================================================
void ServerEngine::CheckWallCollisions(GamePiece& playerPiece)
{
	sf::Vector2f playerPos = playerPiece.getPosition();
	float playerDim = playerPiece.GetDimension();

	if(((playerPos.x + (playerDim / 2)) >= GameData::BOARD_WIDTH) ||
		((playerPos.x - (playerDim / 2)) <= 0)	||
		((playerPos.y + (playerDim / 2)) >= GameData::BOARD_HEIGHT) || 
		((playerPos.y - (playerDim / 2)) <= 0))
	{
		cout << "Player " << playerPiece.getPlayerID() << " has foolishly hit a wall and died " << endl;
		playerPiece.ReSpawn();
	}
}

// ===== CheckPlayerCollision======================================================================
// Method will check if the player piece has collided with any other player. If the 
// player has collided, it will reset the smaller piece and assign it a random position.
// The larger piece will grow.
//
// Input: none
// Output: none
// ================================================================================================
void ServerEngine::CheckPlayerCollision(GamePiece& playerPiece)
{
	for(auto it = players.begin(); it != players.end(); it++)
	{
		if((*it).second.getPlayerID() != playerPiece.getPlayerID())
		{
			//not the same check collisions
			if( (*it).second.getGlobalBounds().intersects(playerPiece.getGlobalBounds()) )
			{
				GamePiece* smallPiece;
				GamePiece* largePiece;
				//they collided eliminate the smaller one
				if(playerPiece.GetDimension() < (*it).second.GetDimension())
				{
					smallPiece = &playerPiece;
					largePiece = &((*it).second);
				}
				else
				{
					smallPiece = &((*it).second);
					largePiece = &playerPiece;
				}
				largePiece->Grow(smallPiece->GetDimension());
				smallPiece->ReSpawn();
			}
		}
	}
}


void ServerEngine::UpdatePlayerDirection(ServerData data)
{
	//update player directions
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		if((*it).second.getPlayerID() == data.m_playerNumber)
		{
			(*it).second.m_direction = static_cast<GamePiece::Direction>(data.direction);
		}
	}
}

GameData ServerEngine::getCurrentGameData(void)
{
	GameData currentGameData;
	currentGameData.pellet1X = gamePellets[0].getPosition().x;
	currentGameData.pellet1Y = gamePellets[0].getPosition().y;
	currentGameData.pellet2X = gamePellets[1].getPosition().x;
	currentGameData.pellet2Y = gamePellets[1].getPosition().y;
	currentGameData.pellet3X = gamePellets[2].getPosition().x;
	currentGameData.pellet3Y = gamePellets[2].getPosition().y;
	currentGameData.pellet4X = gamePellets[3].getPosition().x;
	currentGameData.pellet4Y = gamePellets[3].getPosition().y;

	if(players.find(1) != players.end())
	{
		GamePiece& currentPiece = (*players.find(1)).second;

		currentGameData.player1X = currentPiece.getPosition().x;
		currentGameData.player1Y = currentPiece.getPosition().y;
		currentGameData.player1Dead = false;
		currentGameData.player1Direction = currentPiece.m_direction;
		currentGameData.player1Score = currentPiece.GetDimension();
	}
	
	if(players.find(2) != players.end())
	{
		GamePiece& currentPiece = (*players.find(2)).second;

		currentGameData.player2X = currentPiece.getPosition().x;
		currentGameData.player2Y = currentPiece.getPosition().y;
		currentGameData.player2Dead = false;
		currentGameData.player2Direction = currentPiece.m_direction;
		currentGameData.player2Score = currentPiece.GetDimension();
	}

	if(players.find(3) != players.end())
	{
		GamePiece& currentPiece = (*players.find(3)).second;

		currentGameData.player3X = currentPiece.getPosition().x;
		currentGameData.player3Y = currentPiece.getPosition().y;
		currentGameData.player3Dead = false;
		currentGameData.player3Direction = currentPiece.m_direction;
		currentGameData.player3Score = currentPiece.GetDimension();
	}

	if(players.find(4) != players.end())
	{
		GamePiece& currentPiece = (*players.find(4)).second;

		currentGameData.player4X = currentPiece.getPosition().x;
		currentGameData.player4Y = currentPiece.getPosition().y;
		currentGameData.player4Dead = false;
		currentGameData.player4Direction = currentPiece.m_direction;
		currentGameData.player4Score = currentPiece.GetDimension();
	}

	return currentGameData;
}
