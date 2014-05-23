// ================================================================================================
// Filename: "GamePiece.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 22, 2014
// ================================================================================================
// This is the class implementation file for the GamePiece class. For a class description see the 
// header file "GamePiece.h"
// ================================================================================================

#include "GamePiece.h"

// Initialize Static Constants
const sf::Color GamePiece::PLAYER0_COLOR = sf::Color::Blue;
const sf::Color GamePiece::PLAYER1_COLOR = sf::Color::Red;
const sf::Color GamePiece::PLAYER2_COLOR = sf::Color::Green;
const sf::Color GamePiece::PLAYER3_COLOR = sf::Color::Yellow;

// ===== Constructor ==============================================================================
// The constructor will use the owning players data to set this GamePieces' initial size, color,
// and position.
// 
// Input:
//	[IN] int player				-	player assigned for this GamePiece
//	[IN] sf::Vector2f position	-	the center of the player that this piece represents
//	[IN] float dimension		-	the height and width of the player that this piece represents
//
// Output: none
// ================================================================================================
GamePiece::GamePiece(int player, sf::Vector2f position, float dimension)
{
	Update(position, dimension);
	SetColor(player);
}

// ===== Rebuild ==================================================================================
// Method is used to change the piece to another set-up using the passed parameters. This should be
// called if the owning player is copied or changes.
// 
// Input:
//	[IN] int player				-	player assigned for this GamePiece
//	[IN] sf::Vector2f position	-	the center of the player that this piece represents
//	[IN] float dimension		-	the height and width of the player that this piece represents
//
// Output: none
// ================================================================================================
void GamePiece::Rebuild(int player, sf::Vector2f position, float dimension)
{
	Update(position, dimension);
	SetColor(player);
}

// ===== Update ===================================================================================
// Update is an overloaded function that is used to change the position, dimension, or both of the
// base class rectangleshape. These methods should be used by the owning player whenever its' state
// changed.
// 
// Input:
//	[IN] sf::Vector2f position	-	the center of the player that this piece represents
//	[IN] float dimension		-	the height and width of the player that this piece represents
//
// Output: none
// ================================================================================================
void GamePiece::Update(sf::Vector2f position, float dimension)
{
	Update(dimension);
	Update(position);
}

void GamePiece::Update(sf::Vector2f position)
{
	setPosition(position);
}

void GamePiece::Update(float dimension)
{
	setSize(sf::Vector2f(dimension, dimension));
	setOrigin(sf::Vector2f((dimension / 2), (dimension / 2)));
}

// ===== SetColor =================================================================================
// Sets the rectangle shapes color for rendering based on the passed player number.
// 
// Input:
//	[IN] sint playerNum	-	the assigned player number
//
// Output: none
// ================================================================================================
void GamePiece::SetColor(int playerNum)
{
	if(playerNum == 0)
	{
		setFillColor(PLAYER0_COLOR);
	}
	else if(playerNum == 1)
	{
		setFillColor(PLAYER1_COLOR);
	}
	else if(playerNum == 2)
	{
		setFillColor(PLAYER2_COLOR);
	}
	else if(playerNum == 3)
	{
		setFillColor(PLAYER3_COLOR);
	}
}