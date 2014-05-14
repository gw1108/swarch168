// ================================================================================================
// Filename: "NewPlayer.h"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 13, 2014
// ================================================================================================
// Class Description:
// 
// The NewPlayer class will encapsulate the data used when a new player joins the game. The server 
// will send this data to the clients and the clients will use it to update their game state.
// ================================================================================================

#pragma once

#include <string>
#include <SFML\Network\Packet.hpp>

class NewPlayer
{
public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	NewPlayer(void);
	NewPlayer(std::string userName, int playerNum);

	// Prototypes
	void Copy(const NewPlayer &other);

	// Inlined Methods
	std::string GetUsername(void) { return m_username; } 
	int GetAssignedNumber(void) { return m_playerNum; } 
	void SetUsername(std::string username) { m_username = username; }
	void SetPlayerNumber(int playerNum) { m_playerNum = playerNum; }

private:

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	std::string m_username;
	int m_playerNum;

};

// Packet Overloads for LogInData
sf::Packet& operator<<(sf::Packet& packet, NewPlayer& data);
sf::Packet& operator>>(sf::Packet& packet, NewPlayer& data);

