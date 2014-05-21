#include "ServerEngine.h"
#include <iostream>

using namespace std;

ServerEngine::ServerEngine(void)
	:serverIsRunning(false),
	players(),
	networkController()
{
	for(int i = 0; i < GameData::MAX_PLAYERS; ++i)
	{
		Player player;
		GamePiece piece;
		players.insert(std::make_pair(i, std::make_pair(player, piece)));
	}

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
					GamePiece piece(data.m_playerNumber);
					piece.ReSpawn();
					Player player;
					player.SetUsername(data.m_playerName);
					player.SetActive(true);
					player.SetPlayerNumber(data.m_playerNumber);
					player.SetPosition(piece.getPosition());
					players.at(data.m_playerNumber) = std::make_pair(player, piece);
					cout << "Player " << data.m_playerNumber << " logged in to engine" << endl;
				}
				else if(data.m_command == ServerData::ServerCommands::LOG_OUT)
				{
					players.at(data.m_playerNumber).first.SetActive(false);
				}
			}

			//update people
			for(auto it = players.begin(); it != players.end(); ++it)
			{
				if((*it).second.first.IsActive())
				{
					(*it).second.first.TakeTurn();

					//check for collision against pellets and walls
					CheckPelletCollisions((*it).second.first);

					CheckWallCollisions((*it).second.first);

					CheckPlayerCollision((*it).second.first);
				}
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
void ServerEngine::CheckPelletCollisions(Player& playerPiece)
{
	sf::FloatRect playerBounds = playerPiece.GetRectangle();
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
void ServerEngine::CheckWallCollisions(Player& playerPiece)
{
	sf::Vector2f playerPos = playerPiece.GetPosition();
	float playerDim = playerPiece.GetDimension();

	if(((playerPos.x + playerDim) >= GameData::BOARD_WIDTH) ||
		(playerPos.x <= 0)	||
		((playerPos.y + playerDim) >= GameData::BOARD_HEIGHT) || 
		(playerPos.y <= 0))
	{
		cout << "Player " << playerPiece.GetAssignedNumber() << " has foolishly hit a wall and died " << endl;
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
void ServerEngine::CheckPlayerCollision(Player& playerPiece)
{
	for(auto it = players.begin(); it != players.end(); it++)
	{
		if((*it).second.first.GetAssignedNumber() != playerPiece.GetAssignedNumber())
		{
			//not the same check collisions
			if( (*it).second.first.GetRectangle().intersects(playerPiece.GetRectangle()) )
			{
				Player* smallPiece;
				Player* largePiece;
				//they collided eliminate the smaller one
				if(playerPiece.GetDimension() < (*it).second.first.GetDimension())
				{
					smallPiece = &playerPiece;
					largePiece = &((*it).second.first);
				}
				else
				{
					smallPiece = &((*it).second.first);
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
		if((*it).second.first.GetAssignedNumber() == data.m_playerNumber)
		{
			(*it).second.first.SetDirection(static_cast<GamePiece::Direction>(data.direction));
		}
	}
}

GameData ServerEngine::getCurrentGameData(void)
{
	GameData currentGameData;

	for(auto it = players.begin(); it != players.end(); ++it)
	{
		currentGameData.m_players[(*it).first].CopyFrom((*it).second.first);
	}

	for(int i = 0; i < GameData::MAX_PELLETS; ++i)
	{
		currentGameData.m_pellets[i] = gamePellets[i];
	}

	currentGameData.reset = false;

	return currentGameData;
}
