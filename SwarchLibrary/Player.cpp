// ================================================================================================
// Filename: "Player.cpp"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 20, 2014
// ================================================================================================
// This is the class implementation file for the Player class. For a class description see the 
// header file "Player.h"
// ================================================================================================

#include "Player.h"

// ===== Default Constructor ======================================================================
// The default constructor will initialize the username to an empty string and set player number to
// (-1). If this players number attempts to be used inside the players array by the engine the
// will be out of bounds and crash. The player is always initialized as dead, and should be 
// resurrected at the start of the next round.
//
// Input: none
// Output: none
// ================================================================================================
Player::Player(void) :
	m_username(),
	m_playerNum(-1),
	m_position(),
	m_active(false),
	m_dead(true)
{}

// ===== Conversion Constructor ===================================================================
// The Conversion constructor will initialize the new players name, the assigned player number, its
// position and its' active state. The player is always initialized as dead, and should be 
// resurrected at the start of the next round.
//
// Input:
//	[IN] std::string username		- the text to display for this player
//	[IN] int playerNum				- the server assigned player number, dictates color
//	[IN] Postion position			- a struct containing the x,y coordinates of this player
//	[IN] bool active				- dictates if the play is currently active on the board
//
// Output: none
// ================================================================================================
Player::Player(std::string userName, int playerNum, Position position, bool active) :
	m_username(userName),
	m_playerNum(playerNum),
	m_position(position),
	m_active(active),
	m_dead(true)
{}

// ===== CopyFrom =================================================================================
// Will copy all members belonging to the passed reference to the calling objects members. Will not
// change the passed object.
//
// Input:
//	[IN] const Player &other	- the Player object to be copied
//
// Output: none
// ================================================================================================
void Player::CopyFrom(const Player &other)
{
	m_username = other.m_username;
	m_playerNum = other.m_playerNum;
	m_position.m_xCoordinate = other.m_position.m_xCoordinate;
	m_position.m_yCoordinate = other.m_position.m_yCoordinate;
	m_active = other.m_active;
	m_dead = other.m_dead;
}

// ===== Packet Input Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the Player 
// class can be input into a packet. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet to receive the Player
//	[IN]	Player& data			- a reference to the Player
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator<<(sf::Packet& packet, Player& data)
{
	packet	<< data.GetUsername() 
			<< data.GetAssignedNumber()
			<< data.GetPosition().m_xCoordinate
			<< data.GetPosition().m_yCoordinate
			<< data.IsActive()
			<< data.IsDead();

	return packet;
}

// ===== Packet Output Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the Player 
// class can be extracted from a packet.. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet that contains the Player
//	[IN]	Player& data			- a reference to the Player that will receive the data
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator>>(sf::Packet& packet, Player& data)
{ 
	std::string username;
	int playerNum;
	int xCord;
	int yCord;
	bool state;
	bool dead;

	packet	>> username 
			>> playerNum
			>> xCord
			>> yCord
			>> state;

	data.SetUsername(username);
	data.SetPlayerNumber(playerNum);
	data.SetPosition(xCord, yCord);
	data.SetActive(state);
	data.SetDead(dead);

	return packet;
}