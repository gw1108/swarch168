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
	void UpdateGamePieces(bool reset);
	void PredictOpponents(void);
	void SetUpTextFields(void);
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
	
	// Player Names
	sf::Text m_userNames[GameData::MAX_PLAYERS];

	// Most Recent Data
	GameData m_gameData;

	// Assigned Player Number
	int m_playerNum;
};

#endif