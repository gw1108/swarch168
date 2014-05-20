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

// ===== Constructor ==============================================================================
// The constructor will set-up the GUI and initialize all game values for the client. It will start
// the engine and begin rendering and checking for data from the server.
//
// Input: none
// Output: none
//  ===============================================================================================
ClientEngine::ClientEngine(sf::RenderWindow& mainWindow, const sf::Font& gameFont, int assignedPlayer, std::string userName) :
	m_mainWindow(mainWindow),
	m_gameFont(gameFont),
	m_engineClock(),
	m_running(false),
	m_gameData(),
	m_numPlayers(1),
	m_assignedPlayer(assignedPlayer),
	m_assignedPiece(NULL),
	m_player1(1),
	m_player2(2),
	m_player3(3),
	m_pellets(NULL)
{
	// Point assigned piece to correct player piece
	if(m_assignedPlayer == 1)
	{
		m_assignedPiece = &m_player1;
	}
	else if(m_assignedPlayer == 2)
	{
		m_assignedPiece = &m_player2;
	}
	else if(m_assignedPlayer == 3)
	{
		m_assignedPiece = &m_player3;
	}
	else
	{
		// Assigned Player Invalid
	}

	m_engineClock.restart();

	// Initialize Pellet Array
	m_pellets = new Pellet[GameData::MAX_PELLETS];

	//set the username display
	m_userName = sf::Text(userName, m_gameFont, 30);
}

// ===== Destructor ===============================================================================
// The destructor will ensure all dynamically allocated memory is released.
// ================================================================================================
ClientEngine::~ClientEngine(void)
{
	delete[] m_pellets;
}

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
	GamePiece::Direction moveDirection = GamePiece::DOWN;

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

			/*
			// Check for server update
			if(m_networkControl->DataQueueEmpty())
			{
				PredictOpponents();
			}
			else
			{
				m_gameData = m_networkControl->GetDataUpdate();
				UpdatePellets();
				UpdateOpponents();
			}
			*/

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
					moveDirection = GamePiece::UP;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
				{
					moveDirection = GamePiece::DOWN;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
				{
					moveDirection = GamePiece::LEFT;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D))
				{
					moveDirection = GamePiece::RIGHT;
				}
			} 

			// Move Assigned Piece
			m_assignedPiece->TakeTurn(moveDirection);

			// Check for Pellet collisions, move this to server for MP version
			CheckPelletCollisions();

			// Check for Out of Bounds
			CheckWallCollisions();

			// Render The Game Screen
			Render();

			// Send update to server
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
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::UpdateOpponents()
{
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
}

// ===== CheckPelletCollisions ====================================================================
// Method will check if the assigned players piece has collided with any of the pellets. This
// method will be moved to the server and updated for multiple players once the MP version of the
// game is written.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::CheckPelletCollisions()
{
	sf::FloatRect playerBounds = m_assignedPiece->getGlobalBounds();
	sf::FloatRect pelletBounds;

	for(int pIndex = 0; pIndex < GameData::MAX_PELLETS; pIndex++)
	{
		pelletBounds = m_pellets[pIndex].getGlobalBounds();

		if(playerBounds.intersects(pelletBounds))
		{
			m_assignedPiece->Grow();
			m_pellets[pIndex].Spawn();
		}
	}
}

// ===== CheckWallCollisions ======================================================================
// Method will check if the assigned players piece has collided with any of the walls. If a the 
// player has collided, it will reset the piece and assign it a random position. This method will 
// be moved to the server and updated for multiple players once the MP version of the game is written.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::CheckWallCollisions()
{
	sf::Vector2f playerPos = m_assignedPiece->getPosition();
	float playerDim = m_assignedPiece->GetDimension();

	if(((playerPos.x + (playerDim / 2)) >= GameData::BOARD_WIDTH) ||
		((playerPos.x - (playerDim / 2)) <= 0)	||
		((playerPos.y + (playerDim / 2)) >= GameData::BOARD_HEIGHT) || 
		((playerPos.y - (playerDim / 2)) <= 0))
	{
		m_assignedPiece->ReSpawn();
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
	// Clear Screen
	m_mainWindow.clear(sf::Color(0, 0, 0));

	// Draw Pellets
	for(int pIndex = 0; pIndex < GameData::MAX_PELLETS; pIndex++)
	{
		m_mainWindow.draw(m_pellets[pIndex]);
	}

	// Draw Pieces
	m_mainWindow.draw(*m_assignedPiece);

	// Draw Player User Name
	m_mainWindow.draw(m_userName);

	// Display Screen
	m_mainWindow.display();
}