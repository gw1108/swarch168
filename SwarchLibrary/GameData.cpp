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
const int GameData::ENGINE_SPEED = 20;
const int GameData::BOARD_WIDTH = 800;
const int GameData::BOARD_HEIGHT = 600;
const float GameData::PELLET_GROW_SIZE = 4;
const int GameData::S_UPDATE_SPEED = 100;


// ===== Default Constructor ======================================================================
// The default constructor will be used to zero all values prior to the GameState being used by 
// the engine.
//
// Input: none
// Output: none
// ================================================================================================
GameData::GameData(void) :
	m_players(),
	m_pellets(),
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
void GameData::CopyFrom(const GameData &other)
{
	int index = 0;

	for(index = 0; index < MAX_PLAYERS; index++)
	{
		m_players[index].CopyFrom(other.m_players[index]);
	}

	for(index = 0; index < MAX_PELLETS; index++)
	{
		m_pellets[index].setPosition(other.m_pellets[index].getPosition());
	}

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
	int index = 0;

	for(index = 0; index < GameData::MAX_PLAYERS; index++)
	{
		packet << gameData.m_players[index];
	}

	for(index = 0; index < GameData::MAX_PELLETS; index++)
	{
		packet << gameData.m_pellets[index];
	}
		
	packet << gameData.reset;

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
	int index = 0;

	for(index = 0; index < GameData::MAX_PLAYERS; index++)
	{
		packet >> gameData.m_players[index];
	}

	for(index = 0; index < GameData::MAX_PELLETS; index++)
	{
		packet >> gameData.m_pellets[index];
	}

	packet >> gameData.reset;

	return packet;
}