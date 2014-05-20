// ================================================================================================
// Filename: "GameData.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 13, 2014
// ================================================================================================
// This is the class implementation file for the GameData class. For a class description see the 
// header file "GameData.h"
// ================================================================================================

#include "GameData.h"
#include "GamePiece.h"

// Set Constants
const int GameData::SERVER_PORT = 30000;
const int GameData::ENGINE_SPEED = 17;
const int GameData::BOARD_WIDTH = 800;
const int GameData::BOARD_HEIGHT = 600;
const int GameData::PELLET_GROW_SIZE = 4;
const int GameData::MAX_PELLETS = 4;
const int GameData::MAX_PLAYERS = 4;
const int GameData::S_UPDATE_SPEED = 1500;


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
	player1Dead(false),
	player1Score(0),

	player2X(BOARD_WIDTH / 2),			
	player2Y(BOARD_HEIGHT / 2),				
	player2Direction(GamePiece::DOWN), 	
	player2Dead(false),
	player2Score(0),

	player3X(BOARD_WIDTH / 2),			
	player3Y(BOARD_HEIGHT / 2),				
	player3Direction(GamePiece::DOWN), 	
	player3Dead(false),
	player3Score(0),

	player4X(BOARD_WIDTH / 2),			
	player4Y(BOARD_HEIGHT / 2),				
	player4Direction(GamePiece::DOWN), 	
	player4Dead(false),
	player4Score(0),

	pellet1X(0),
	pellet1Y(0),
	pellet2X(0),
	pellet2Y(0),
	pellet3X(0),
	pellet3Y(0),
	pellet4X(0),
	pellet4Y(0),

	reset(false)
{}

// ===== Copy =====================================================================================
// Will copy all members belonging to the passed reference to the calling objects members. Will not
// change the passed object.
//
// Input:
//	[IN] const GameData& other	- the GameData object to be copied
//
// Output: none
// ================================================================================================
void GameData::Copy(const GameData &other)
{
	player1X = other.player1X;		
	player1Y = other.player1Y;				
	player1Direction = other.player1Direction;
	player1Dead = other.player1Dead;
	player1Score = other.player1Score;

	player2X = other.player2X;		
	player2Y = other.player2Y;				
	player2Direction = other.player2Direction;
	player2Dead = other.player2Dead;
	player2Score = other.player2Score;

	player3X = other.player3X;		
	player3Y = other.player3Y;				
	player3Direction = other.player3Direction;
	player3Dead = other.player3Dead;
	player3Score = other.player3Score;

	player4X = other.player4X;		
	player4Y = other.player4Y;				
	player4Direction = other.player4Direction;
	player4Dead = other.player4Dead;
	player4Score = other.player4Score;

	pellet1X = other.pellet1X;
	pellet1Y = other.pellet1Y;
	pellet2X = other.pellet2X;
	pellet2Y = other.pellet2Y;
	pellet3X = other.pellet3X;
	pellet3Y = other.pellet3Y;
	pellet4X = other.pellet4X;
	pellet4Y = other.pellet4Y;

	reset = other.reset;
}

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
	packet << gameData.player1X 
		<< gameData.player1Y 
		<< gameData.player1Direction 
		<< gameData.player1Dead 
		<< gameData.player1Score

		<< gameData.player2X 
		<< gameData.player2Y 
		<< gameData.player2Direction 
		<< gameData.player2Dead 
		<< gameData.player2Score 

		<< gameData.player3X 
		<< gameData.player3Y 
		<< gameData.player3Direction 
		<< gameData.player3Dead 
		<< gameData.player3Score 

		<< gameData.player4X 
		<< gameData.player4Y 
		<< gameData.player4Direction 
		<< gameData.player4Dead 
		<< gameData.player4Score 
		
		<< gameData.pellet1X 
		<< gameData.pellet1Y 
		<< gameData.pellet2X 
		<< gameData.pellet2Y 
		<< gameData.pellet3X 
		<< gameData.pellet3Y 
		<< gameData.pellet4X 
		<< gameData.pellet4Y 
		
		<< gameData.reset;

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
	packet >> gameData.player1X 
		>> gameData.player1Y 
		>> gameData.player1Direction 
		>> gameData.player1Dead 
		>> gameData.player1Score

		>> gameData.player2X 
		>> gameData.player2Y 
		>> gameData.player2Direction 
		>> gameData.player2Dead 
		>> gameData.player2Score 

		>> gameData.player3X 
		>> gameData.player3Y 
		>> gameData.player3Direction 
		>> gameData.player3Dead 
		>> gameData.player3Score 

		>> gameData.player4X 
		>> gameData.player4Y 
		>> gameData.player4Direction 
		>> gameData.player4Dead 
		>> gameData.player4Score 
		
		>> gameData.pellet1X 
		>> gameData.pellet1Y 
		>> gameData.pellet2X 
		>> gameData.pellet2Y 
		>> gameData.pellet3X 
		>> gameData.pellet3Y 
		>> gameData.pellet4X 
		>> gameData.pellet4Y 
		
		>> gameData.reset;

	return packet;
}