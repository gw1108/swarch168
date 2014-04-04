// ================================================================================================
// Filename: "ClientEngine.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: April 04, 2014
// ================================================================================================
// Class Description:
// 
// The ClientEngine class will control all game logic and updates to the game data.
// ================================================================================================ 

#ifndef CLIENTENGINE_H
#define CLIENTENGINE_H

#include <SFML\System\Clock.hpp>
#include <SFML\Graphics.hpp>

class ClientEngine
{

public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor/Destructor Prototypes
	ClientEngine(void);
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
	void UpdateGameData();
	void Render(void);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Engine Related Members
	sf::Clock m_engineClock;
	bool m_running;

	// Game Pieces

	// Window Members
	sf::RenderWindow *m_mainWindow;
	sf::Font m_gameFont;
};

#endif