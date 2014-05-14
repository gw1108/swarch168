// ================================================================================================
// Filename: "NewPlayer.cpp"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 13, 2014
// ================================================================================================
// This is the class implementation file for the NewPlayer class. For a class description see the 
// header file "NewPlayer.h"
// ================================================================================================

#include "NewPlayer.h"

// ===== Default Constructor ======================================================================
// The default constructor will initialize the m_username with and empty string and set the 
// assigned player number to (-1). These values will prevent the client engine from attempting to
// use this newplayer.
//
// Input: none
// Output: none
// ================================================================================================
NewPlayer::NewPlayer(void) :
	m_username(),
	m_playerNum(-1)
{}

// ===== Conversion Constructor ===================================================================
// The Conversion constructor will initialize the new players name and the assigned player number.
//
// Input:
//	[IN] std::string username		- the text to display for this player
//	[IN] std::string playerNum		- the server assigned player number, dictates color
//
// Output: none
// ================================================================================================
NewPlayer::NewPlayer(std::string userName, int playerNum) :
	m_username(userName),
	m_playerNum(playerNum)
{}

// ===== Copy =====================================================================================
// Will copy all members belonging to the passed reference to the calling objects members. Will not
// change the passed object.
//
// Input:
//	[IN] const NewPlayer &other	- the NewPlayer object to be copied
//
// Output: none
// ================================================================================================
void NewPlayer::Copy(const NewPlayer &other)
{
	m_username = other.m_username;
	m_playerNum = other.m_playerNum;
}

// ===== Packet Input Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the NewPlayer 
// class can be input into a packet for delivery to a client. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet to receive the NewPlayer
//	[IN]	NewPlayer& data			- a reference to the NewPlayer
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator<<(sf::Packet& packet, NewPlayer& data)
{
	packet	<< data.GetUsername() << data.GetAssignedNumber();

	return packet;
}

// ===== Packet Output Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the NewPlayer 
// class can be extracted from a packet by the client. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet that contains the NewPlayer
//	[IN]	NewPlayer& data			- a reference to the NewPlayer that will receive the data
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator>>(sf::Packet& packet, NewPlayer& data)
{ 
	std::string username;
	int playerNum;

	packet	>> username >> playerNum;

	data.SetUsername(username);
	data.SetPlayerNumber(playerNum);

	return packet;
}

