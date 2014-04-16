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
	float xCoord = 0;
	float yCoord = 0;

	xCoord = ((rand() % (GameData::BOARD_WIDTH)));	

	if(xCoord <= 10)
	{
		xCoord = 10;
	}
	else if(xCoord >= (GameData::BOARD_WIDTH - 10))
	{
		xCoord = (GameData::BOARD_WIDTH - 10);
	}

	yCoord = ((rand() % (GameData::BOARD_HEIGHT)));	

	if(yCoord <= 10)
	{
		yCoord = 10;
	}
	else if(yCoord >= (GameData::BOARD_HEIGHT - 10))
	{
		yCoord = (GameData::BOARD_HEIGHT - 10);
	}

	sf::Vector2f position(xCoord, yCoord);

	setPosition(position);
}