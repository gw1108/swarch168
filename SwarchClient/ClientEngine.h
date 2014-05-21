// ================================================================================================
// Filename: "ClientEngine.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 4, 2014
// ================================================================================================
// Class Description:
// 
// The ClientEngine class will control all game logic and updates to the game data.
// ================================================================================================ 

#ifndef CLIENTENGINE_H
#define CLIENTENGINE_H

#include <SFML\System\Clock.hpp>
#include <SFML\Graphics.hpp>
#include "CNetworkController.h"
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
	ClientEngine(sf::RenderWindow& mainWindow, const sf::Font& gameFont, CNetworkController& networkControl,
				 int assignedPlayer, std::string userName);
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
	void UpdateOpponents(bool reset);
	void PredictOpponents(void);
	void Render(void);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Window Members
	sf::RenderWindow &m_mainWindow;
	const sf::Font &m_gameFont;

	// Engine Members
	CNetworkController& m_networkControl;
	sf::Clock m_engineClock;
	bool m_running;
	
	// User Data
	sf::Text m_userName;

	// Most Recent Data
	GameData m_gameData;

	// Game Pieces
	int m_playerNum;
	GamePiece *m_assignedPiece;
	GamePiece m_playerPieces[GameData::MAX_PLAYERS];
};

#endif