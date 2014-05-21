// ================================================================================================
// Filename: "GameData.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 13, 2014
// ================================================================================================
// Class Description:
// 
// The GameData class is a data structure that will maintain all values needed by the game to 
// work properly. This class will also be used to transmit data from the servers to each client. 
// It will contain the position of players, their direction of movement and information on pellets.
//
// The class will also maintain constants that are used to define certain game rules, bounds, or
// keywords used throughout the game so that the server and client are using the same values.
//
// To increase simplicity, the GameData class will make all members public.
// ================================================================================================

#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML\Network\Packet.hpp>
#include "Player.h"
#include "Pellet.h"

class GameData
{

public:

	// Command Code Enum
	enum LoginResponse {NEW_ACCOUNT, ACCEPTED, WRONG_PW};
	//NEW_PLAYER : server - > client, recieve newplayer class on a new player joining game
	//GAME_UPDATE : server - > client, recieve gameupdate class on updated game.
	//PLAYER_UPDATE : client - > server, recieve direction at time
	enum CommandCode {LOG_IN, INITIALIZE, GAME_UPDATE, PLAYER_UPDATE, NEW_PLAYER};
	

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor/Destructor Prototypes
	GameData(void);
	~GameData(void){}

	// Method Prototypes
	void CopyFrom(const GameData &other);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Static Constants
	static const int SERVER_PORT; 
	static const int ENGINE_SPEED;		// The Loop Conditional to control the engine cycle rate of clients and server
	static const int BOARD_HEIGHT;		// Height of the playable area
	static const int BOARD_WIDTH;		// Width of the playable area
	static const float PELLET_GROW_SIZE;	// The amount a players piece should grow when it eats a pellet 
	static const int MAX_PELLETS = 4;	// Max number of pellets allowed on board
	static const int MAX_PLAYERS = 4;	// Max number of players allowed in server
	static const int S_UPDATE_SPEED;	// Determines the speed in milliseconds of the network thread

	// Player Array
	Player m_players[MAX_PLAYERS];

	// Pellet Data
	Pellet m_pellets[MAX_PELLETS];

	// Gameplay Data
	bool reset;						// true if the game should be reset

private:

};

// Packet Overloads for GameData
sf::Packet& operator<<(sf::Packet& packet, GameData& gameData);
sf::Packet& operator>>(sf::Packet& packet, GameData& gameData);

#endif