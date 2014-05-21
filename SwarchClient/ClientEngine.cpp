// ================================================================================================
// Filename: "ClientEngine.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 4, 2014
// ================================================================================================
// This is the implementation file for the ClientEngine class. For class description see the header
// file "ClientEngine.h"
// ================================================================================================

#include <iostream>
#include "ClientEngine.h"
#include "Player.h"

// ===== Constructor ==============================================================================
// The constructor will set-up the GUI and initialize all game values for the client. It will start
// the engine and begin rendering and checking for data from the server.
//
// Input: none
// Output: none
//  ===============================================================================================
ClientEngine::ClientEngine(sf::RenderWindow& mainWindow, const sf::Font& gameFont, CNetworkController& networkControl,
						   int assignedPlayer, std::string userName) :
	m_mainWindow(mainWindow),
	m_gameFont(gameFont),
	m_networkControl(networkControl),
	m_engineClock(),
	m_running(false),
	m_gameData(),
	m_playerPieces(),
	m_playerNum(assignedPlayer)
{

	for(int index = 0; index < GameData::MAX_PLAYERS; index++)
	{
		m_playerPieces[index].SetPlayerNumber(index);
	}

	m_assignedPiece = &(m_playerPieces[assignedPlayer]);

	m_engineClock.restart();

	//set the username display
	m_userName = sf::Text(userName, m_gameFont, 30);
}

// ===== Destructor ===============================================================================
// The destructor will ensure all dynamically allocated memory is released.
// ================================================================================================
ClientEngine::~ClientEngine(void)
{}

// ===== Run ======================================================================================
// This method will be the "infinite" loop that starts when the engine starts. Every iteration it
// will check for data from server, update the game state based on server data, respond to user
// input, render the main window, and send an update to the server.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::Run(void)
{
	GamePiece::Direction newDirection = GamePiece::DOWN;
	GamePiece::Direction moveDirection = GamePiece::DOWN;
	Player newPlayer;

	m_running = true;

	while(m_running)
	{
		// Limit Engine to ~60 Cycles a Second
		if(m_engineClock.getElapsedTime().asMilliseconds() <= GameData::ENGINE_SPEED)
		{
			Render();
			continue;
		}
		else
		{
			m_engineClock.restart();

			// Check for Data Update
			if(m_networkControl.GetNextData(m_gameData))
			{
				UpdateOpponents(m_gameData.reset);
			}
			else
			{
				PredictOpponents();
			}

			// Loop through events and assign proper values to moveDirection
			sf::Event event;
			while(m_mainWindow.pollEvent(event))
			{
				if((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
				{
					m_running = false;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
				{
					newDirection = GamePiece::UP;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
				{
					newDirection = GamePiece::DOWN;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
				{
					newDirection = GamePiece::LEFT;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D))
				{
					newDirection = GamePiece::RIGHT;
				}
			} 

			if(newDirection != moveDirection)
			{
				// Send Move Command
				m_networkControl.SendDirectionChange(newDirection);
				moveDirection = newDirection;
			}

			// Render The Game Screen
			Render();
		}
	}
}

// ===== UpdatePellets ============================================================================
// This method updates the pelletes positions and handles the adding and removing of pellets. This
// should be called whenever new data is received from the server.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::UpdatePellets()
{
}

// ===== UpdateOpponents ==========================================================================
// This method updates the opponents positions. It will interpolate between the currently predicted
// positions and the updated ones received from the server. This method should be called whenever
// new data is received from the server.
//
// Input:
//	[IN]	bool reset	-	if the players need to be interpolated or not
//
// Output: none
// ================================================================================================
void ClientEngine::UpdateOpponents(bool reset)
{
	int index = 0;

	// Remove When interpolation is complete
	reset = true;

	if(reset)
	{
		for(index = 0; index < GameData::MAX_PLAYERS; index++)
		{
			m_playerPieces[index].setSize(sf::Vector2f(m_gameData.m_players[index].GetDimension(),
													   m_gameData.m_players[index].GetDimension()));

			m_playerPieces[index].setOrigin(sf::Vector2f((m_gameData.m_players[index].GetDimension() / 2),
														 (m_gameData.m_players[index].GetDimension() / 2)));

			m_playerPieces[index].setPosition(m_gameData.m_players[index].GetPosition());
		}
	}
}

// ===== PredictOpponents =========================================================================
// This method attempts to predict the next position of each opponent by using the current position
// and last knows direction of movement. This method should be called during game loops when no
// new data has arrived from the server.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::PredictOpponents()
{
	int index = 0;

	for(index = 0; index < GameData::MAX_PLAYERS; index++)
	{
		m_gameData.m_players[index].TakeTurn();
		m_playerPieces[index].setPosition(m_gameData.m_players[index].GetPosition());
	}
}

// ===== Render ===================================================================================
// Method will render the main client window. This method will need to be updated once the MP
// version of the game is done.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::Render(void)
{
	int index = 0;

	// Clear Screen
	m_mainWindow.clear(sf::Color(0, 0, 0));


	// Draw Pellets
	for(index = 0; index < GameData::MAX_PELLETS; index++)
	{
		m_mainWindow.draw(m_gameData.m_pellets[index]);
	}

	// Draw Pieces
	for(index = 0; index < GameData::MAX_PLAYERS; index++)
	{
		if((m_gameData.m_players[index].IsActive()) && !(m_gameData.m_players[index].IsDead()))
		{
			m_mainWindow.draw(m_playerPieces[index]);
		}
	}

	// Draw Player User Name
	m_mainWindow.draw(m_userName);

	// Display Screen
	m_mainWindow.display();
}