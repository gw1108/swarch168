// ================================================================================================
// Filename: "ClientEngine.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: April 16, 2014
// ================================================================================================
// Class Description:
// 
// The ClientEngine class will control all game logic and updates to the game data.
// ================================================================================================ 

#ifndef CLIENTENGINE_H
#define CLIENTENGINE_H

#include <SFML\System\Clock.hpp>
#include <SFML\Graphics.hpp>
#include "GamePiece.h"
#include "GameData.h"
#include "Pellet.h"

class ClientEngine
{

public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor/Destructor Prototypes
	ClientEngine(int assignedPlayer);
	~ClientEngine(void);

	// Prototypes
	void Run(void);

	// Inlined
	void Stop(void){ m_running = false; }

private:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Prototypes
	void UpdatePellets(void);
	void UpdateOpponents(void);
	void PredictOpponents(void);
	void CheckPelletCollisions(void);
	void CheckWallCollisions(void);
	void Render(void);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Engine Related Members
	sf::Clock m_engineClock;
	bool m_running;

	// Most Recent Data
	GameData m_gameData;

	// Game Pieces
	int m_numPlayers;
	int m_assignedPlayer;
	GamePiece *m_assignedPiece;
	GamePiece m_player1;
	GamePiece m_player2;
	GamePiece m_player3;

	// Pellets
	Pellet *m_pellets;

	// Window Members
	sf::RenderWindow *m_mainWindow;
	sf::Font *m_gameFont;
};

#endif