// ================================================================================================
// Filename: "Pellet.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 16, 2014
// ================================================================================================
// This is the class implementation file for the Pellet class. For a class description see the 
// header file "Pellet.h"
// ================================================================================================

#include "Pellet.h"
#include "GameData.h"

// Initialize Static Constants
const float Pellet::DIMENSION = 10;
const sf::Color Pellet::COLOR = sf::Color(255, 140, 0);	// Orange

// ===== Constructor ==============================================================================
// The constructor will use class constants to define the size of the sf::RectangleShape parent.
// It will abstract away most of the set-up required for an sf::RectangleShape. The initial position
// will be set using the Spawn method.
// 
// Input:
//	[IN] int player	-	player assigned for this GamePiece, 1 is always this clients user
//
// Output: none
//  ===============================================================================================
Pellet::Pellet(void)
{
	setSize(sf::Vector2f(DIMENSION, DIMENSION));
	setOrigin(sf::Vector2f((DIMENSION / 2), (DIMENSION / 2)));
	setFillColor(COLOR);
	Spawn();
}

// ===== Spawn ====================================================================================
// Method will randomly choose an X and Y coordinate for the pellet and assign its position. This
// should be called when the pellet is first created and whenever the pellet is eaten by a player.
// 
// Input: none
// Output: none
//  ===============================================================================================
void Pellet::Spawn(void)
{
	float wallBuffer = 20;
	float xCoord = 0;
	float yCoord = 0;

	xCoord = (float)((rand() % (int)(GameData::BOARD_WIDTH)));	

	if(xCoord <= wallBuffer)
	{
		xCoord = wallBuffer;
	}
	else if(xCoord >= (GameData::BOARD_WIDTH - wallBuffer))
	{
		xCoord = (GameData::BOARD_WIDTH - wallBuffer);
	}

	yCoord = (float)((rand() % (int)(GameData::BOARD_HEIGHT)));	

	if(yCoord <= wallBuffer)
	{
		yCoord = wallBuffer;
	}
	else if(yCoord >= (GameData::BOARD_HEIGHT - wallBuffer))
	{
		yCoord = (GameData::BOARD_HEIGHT - wallBuffer);
	}

	sf::Vector2f position(xCoord, yCoord);

	setPosition(position);
}

// ===== Packet Input Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the Pellet 
// class can be input into a packet. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet to receive the Pellet
//	[IN]	Pellet& data			- a reference to the Pellet
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator<<(sf::Packet& packet, Pellet& data)
{
	packet	<< data.getGlobalBounds().left
			<< data.getGlobalBounds().top;

	return packet;
}

// ===== Packet Output Overload ====================================================================
// This method overloads the stream operator used with the sf::Packet class so that the Pellet 
// class can be extracted from a packet.. 
//
// Input:
//	[IN/OUT] sf::Packet& packet		- the packet that contains the Pellet
//	[IN]	Pellet& data			- a reference to the Pellet that will receive the data
//
// Output:
//	[OUT] sf::Packet& packet		- will return the passed packet reference
// ================================================================================================
sf::Packet& operator>>(sf::Packet& packet, Pellet& data)
{ 
	float xCord;
	float yCord;

	packet	>> xCord
			>> yCord;

	data.setPosition(xCord, yCord);

	return packet;
}