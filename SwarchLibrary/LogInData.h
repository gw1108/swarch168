// ================================================================================================
// Filename: "LogInData.h"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 4, 2014
// ================================================================================================
// Class Description:
// 
// The LogInData class is used to hold two strings that will be sent to the server and checked 
// against the database. 
// ================================================================================================

#pragma once

#include <string>
#include <SFML\Network\Packet.hpp>

class LogInData
{
public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	LogInData(void);
	LogInData(std::string username, std::string password);

	// Inlined Methods
	std::string GetUsername(void) { return m_username; } 
	std::string GetPassword(void) { return m_password; } 
	void SetUsername(std::string username) { m_username = username; }
	void SetPassword(std::string password) { m_password = password; }

private:

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	std::string m_username;
	std::string m_password;

};

// Packet Overloads for LogInData
sf::Packet& operator<<(sf::Packet& packet, LogInData& data);
sf::Packet& operator>>(sf::Packet& packet, LogInData& data);