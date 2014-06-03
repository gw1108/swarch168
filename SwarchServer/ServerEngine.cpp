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
		players.insert(std::make_pair(i, player));
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
	sf::Clock engineClock = sf::Clock();

	while(serverIsRunning)
	{
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
				Player player;
				player.ReSpawn();
				player.SetUsername(data.m_playerName);
				player.SetActive(true);
				player.SetPlayerNumber(data.m_playerNumber);
				players.at(data.m_playerNumber) = player;
				cout << "Player " << data.m_playerNumber << " logged in to engine" << endl;
			}
			else if(data.m_command == ServerData::ServerCommands::LOG_OUT)
			{
				players.at(data.m_playerNumber).SetActive(false);
			}
		}

		//update people
		for(auto it = players.begin(); it != players.end(); ++it)
		{
			if((*it).second.IsActive())
			{
				(*it).second.TakeTurn();

				//check for collision against pellets and walls
				CheckPelletCollisions((*it).second);

				CheckWallCollisions((*it).second);

				CheckPlayerCollision((*it).second);
			}
		}

		//update game data
		//TODO : make reset work
		UpdateGameData();

		//send update to players
		networkController.sendGameUpdate(m_currentGameData);

		int sleepTime = GameData::ENGINE_SPEED - engineClock.getElapsedTime().asMilliseconds();
		if(sleepTime < 0)
		{
			sleepTime = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		engineClock.restart();
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
// Method will check for collisions between all players and all pellets.
//
// Input: none
// Output: none
// ================================================================================================
void ServerEngine::CheckPelletCollisions(Player& player)
{
	for(int pelletIndex = 0; pelletIndex < GameData::MAX_PELLETS; pelletIndex++)
	{
		if(player.CollidesWith(gamePellets[pelletIndex]))
		{
			player.Grow();
			player.SetScore(player.GetScore() + 1);
			gamePellets[pelletIndex].Spawn();
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
void ServerEngine::CheckWallCollisions(Player& player)
{
	if(player.OutOfBounds())
	{
		cout << "Player " << player.GetAssignedNumber() << " has foolishly hit a wall and died " << endl;
		player.ReSpawn();
		if(player.GetScore() > 0)
		{
			player.SetScore(player.GetScore() - 1);
		}
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
void ServerEngine::CheckPlayerCollision(Player& player)
{
	for(auto it = players.begin(); it != players.end(); it++)
	{
		if((*it).second.GetAssignedNumber() != player.GetAssignedNumber())
		{
			//not the same check collisions
			if( (*it).second.CollidesWith(player))
			{
				Player* smallPiece;
				Player* largePiece;
				//they collided eliminate the smaller one
				if(player.GetDimension() == (*it).second.GetDimension()
					 && player.GetScore() != (*it).second.GetScore())
				{
					if(player.GetScore() > (*it).second.GetScore())
					{
						largePiece = &player;
						smallPiece = &(*it).second;
					}
					else
					{
						largePiece = &player;
						smallPiece = &(*it).second;
					}
				}
				else if(player.GetDimension() < (*it).second.GetDimension())
				{
					smallPiece = &player;
					largePiece = &((*it).second);
				}
				else
				{
					smallPiece = &((*it).second);
					largePiece = &player;
				}
				largePiece->Grow(smallPiece->GetDimension());
				smallPiece->ReSpawn();
				largePiece->SetScore(largePiece->GetScore()+smallPiece->GetScore() + 2);
				smallPiece->SetScore(smallPiece->GetScore() / 2);
			}
		}
	}
}


void ServerEngine::UpdatePlayerDirection(ServerData data)
{
	//update player directions
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		if((*it).second.GetAssignedNumber() == data.m_playerNumber)
		{
			(*it).second.SetDirection(static_cast<Player::Direction>(data.direction));
		}
	}
}


void ServerEngine::UpdateGameData(bool reset)
{
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		m_currentGameData.m_players[(*it).first].CopyFrom((*it).second);
	}

	for(int i = 0; i < GameData::MAX_PELLETS; ++i)
	{
		m_currentGameData.m_pellets[i] = gamePellets[i];
	}

	m_currentGameData.reset = reset;
}