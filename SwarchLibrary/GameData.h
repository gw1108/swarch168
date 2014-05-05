// ================================================================================================
// Filename: "GameData.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 16, 2014
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

class GameData
{

public:

	// Command Code Enum
	enum LoginResponse {NEW_ACCOUNT, ACCEPTED, WRONG_PW};
	enum CommandCode {LOG_IN, INITIALIZE, GAME_UPDATE, PLAYER_UPDATE};

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor/Destructor Prototypes
	GameData(void);
	~GameData(void){}

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Static Constants
	static const int SERVER_PORT; 
	static const int ENGINE_SPEED;		// The Loop Conditional to control the engine cycle rate of clients and server
	static const int BOARD_HEIGHT;		// Height of the playable area
	static const int BOARD_WIDTH;		// Width of the playable area
	static const int PELLET_GROW_SIZE;	// The amount a players piece should grow when it eats a pellet 
	static const int MAX_PELLETS;		// Max number of pellets allowed on board

	// Player One Data
	float player1X;						// X-Coordinate of the player
	float player1Y;						// Y-Coordinate of the player
	int player1Direction;				// Direction of movement
	bool player1Died;					// True if the player needs to be reset

	// Player Two Data
	float player2X;						// X-Coordinate of the player
	float player2Y;						// Y-Coordinate of the player
	int player2Direction;				// Direction of movement
	bool player2Died;					// True if the player needs to be reset

	// Player Three Data
	float player3X;						// X-Coordinate of the player
	float player3Y;						// Y-Coordinate of the player
	int player3Direction;				// Direction of movement
	bool player3Died;					// True if the player needs to be reset

	// Pellet Data
	// TODO

	// Gameplay Data
	bool gameWon;						// True if the game has been won
	int winningPlayer;					// The winning player
	bool startGame;						// Tells the client to start their game

private:

};

// Packet Overloads for GameData
sf::Packet& operator<<(sf::Packet& packet, GameData& gameData);
sf::Packet& operator>>(sf::Packet& packet, GameData& gameData);

#endif