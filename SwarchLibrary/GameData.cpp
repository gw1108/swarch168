// ================================================================================================
// Filename: "GameData.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 16, 2014
// ================================================================================================
// This is the class implementation file for the GameData class. For a class description see the 
// header file "GameData.h"
// ================================================================================================

#include "GameData.h"
#include "GamePiece.h"

// Set Constants
const int GameData::SERVER_PORT = 88585;
const int GameData::ENGINE_SPEED = 17;
const int GameData::BOARD_WIDTH = 800;
const int GameData::BOARD_HEIGHT = 600;
const int GameData::PELLET_GROW_SIZE = 4;
const int GameData::MAX_PELLETS = 4;


// ===== Default Constructor ======================================================================
// The default constructor will be used to zero all values prior to the GameState being used by 
// the engine.
//
// Input: none
// Output: none
// ================================================================================================
GameData::GameData(void) :
	player1X(BOARD_WIDTH / 2),			
	player1Y(BOARD_HEIGHT / 2),				
	player1Direction(GamePiece::DOWN), 	
	player1Died(false),

	player2X(BOARD_WIDTH / 2),			
	player2Y(BOARD_HEIGHT / 2),				
	player2Direction(GamePiece::DOWN), 	
	player2Died(false),

	player3X(BOARD_WIDTH / 2),			
	player3Y(BOARD_HEIGHT / 2),				
	player3Direction(GamePiece::DOWN), 	
	player3Died(false),

	gameWon(false),
	winningPlayer(0),
	startGame(false)
{}

// ===== Packet Input Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the GameData 
// class can be input into a packet for delivery to a client. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet to receive the GameData
//	[IN]	GameData& gameData		- a reference to the GameData
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator<<(sf::Packet& packet, GameData& gameData)
{
	// TODO
	return packet;
}

// ===== Packet Output Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the GameData 
// class can be extracted from a packet by a client 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet that contains the GameData
//	[IN]	GameData& gameData		- a reference to the GameData that will receive the data
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator>>(sf::Packet& packet, GameData& gameData)
{
	// TODO
	return packet;
}