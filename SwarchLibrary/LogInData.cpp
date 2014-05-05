// ================================================================================================
// Filename: "LogInData.cpp"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 4, 2014
// ================================================================================================
// This is the class implementation file for the LogInData class. For a class description see the 
// header file "LogInData.h"
// ================================================================================================

#include "LogInData.h"

// ===== Default Constructor ======================================================================
// The default constructor will initialize the m_username and m_password with empty strings.
//
// Input: none
// Output: none
// ================================================================================================
LogInData::LogInData(void) :
	m_username(),
	m_password()
{}

// ===== Conversion Constructor ===================================================================
// This constructor will use the passed strings to initialize the m_username and m_password members.
//
// Input:
//	[IN] std::string username
//	[IN] std::string password
//
// Output: none
// ================================================================================================
LogInData::LogInData(std::string username, std::string password) :
	m_username(username),
	m_password(password)
{}

// ===== Packet Input Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the LogInData 
// class can be input into a packet for delivery to the server. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet to receive the GameData
//	[IN]	GameData& gameData		- a reference to the GameData
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator<<(sf::Packet& packet, LogInData& data)
{
	packet	<< data.GetUsername() << data.GetPassword();

	return packet;
}

// ===== Packet Output Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the LogInData 
// class can be extracted from a packet by the server. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet that contains the GameData
//	[IN]	GameData& gameData		- a reference to the GameData that will receive the data
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator>>(sf::Packet& packet, LogInData& data)
{
	std::string username;
	std::string password;

	packet	>> username >> password;

	data.SetUsername(username);
	data.SetPassword(password);

	return packet;
}
