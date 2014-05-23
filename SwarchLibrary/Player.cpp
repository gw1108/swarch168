// ================================================================================================
// Filename: "Player.cpp"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 22, 2014
// ================================================================================================
// This is the class implementation file for the Player class. For a class description see the 
// header file "Player.h"
// ================================================================================================

#include <iostream>
#include "Player.h"
#include "GameData.h"

// Initialize Static Constants
const float Player::START_DIMENSION = 20;
const float Player::BASE_MOVE_RATE = 3;

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
	m_direction(UP),
	m_moveRate(BASE_MOVE_RATE),
	m_currentDimension(START_DIMENSION),
	m_piece(m_playerNum, m_position, m_currentDimension),
	m_active(false),
	m_dead(false)
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
Player::Player(std::string userName, int playerNum, sf::Vector2f position, bool active) :
	m_username(userName),
	m_playerNum(playerNum),
	m_position(position),
	m_direction(UP),
	m_moveRate(BASE_MOVE_RATE),
	m_currentDimension(START_DIMENSION),
	m_piece(m_playerNum, m_position, m_currentDimension),
	m_active(active),
	m_dead(false)
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
	m_position = other.m_position;
	m_direction = other.m_direction;
	m_moveRate = other.m_moveRate;
	m_currentDimension = other.m_currentDimension;
	m_piece.Rebuild(m_playerNum, m_position, m_currentDimension);
	m_active = other.m_active;
	m_dead = other.m_dead;
}

// ===== ReSpawn ==================================================================================
// Will set the players position to a random location within the bounds of the game board and reset
// the size of the players piece back to the starting dimension. It will also unflag m_dead.
//
// Input: none
// Output: none
// ================================================================================================
void Player::ReSpawn(void)
{
	// Randomly Select New Position
	float xCoord = 0;
	float yCoord = 0;

	xCoord = (float)((rand() % (GameData::BOARD_WIDTH)));	

	if(xCoord <= 10 + Player::START_DIMENSION)
	{
		xCoord = 10;
	}
	else if(xCoord >= (GameData::BOARD_WIDTH - 10 - Player::START_DIMENSION))
	{
		xCoord = (GameData::BOARD_WIDTH - 10.f);
	}

	yCoord = (float)((rand() % (GameData::BOARD_HEIGHT)));	

	if(yCoord <= 10 + Player::START_DIMENSION)
	{
		yCoord = 10;
	}
	else if(yCoord >= (GameData::BOARD_HEIGHT - 10 - Player::START_DIMENSION))
	{
		yCoord = (GameData::BOARD_HEIGHT - 10.f);
	}

	// Reset Size and Set new Position
	m_currentDimension = START_DIMENSION;
	SetPosition(xCoord, yCoord);
	CalculateSpeed();

	// Update GamePiece
	m_piece.Update(m_position, m_currentDimension);

	// Revive Player
	m_dead = false;
}

// ===== TakeTurn =================================================================================
// The TakeTurn method will adjust the position by the m_moveRate in the m_direction. This should 
// be called on every game loop.
//
// Input: none
// Output: none
// ================================================================================================
void Player::TakeTurn(void)
{
	if(m_direction == UP)
	{
		m_position.y -= m_moveRate; 
	}
	else if(m_direction == DOWN)
	{
		m_position.y += m_moveRate; 
	}
	else if(m_direction == RIGHT)
	{
		m_position.x += m_moveRate;
	}
	else if(m_direction == LEFT)
	{
		m_position.x -= m_moveRate;  
	}
	else
	{
		// Error with Direction; Dont Update GamePiece
		return;
	}

	m_piece.Update(m_position);
}

// ===== Grow =====================================================================================
// Method will increase the size of the player based on the size of the player that was just 
// "eaten".
//
// Input: 
//		[IN]	int foodSize		- the size of whatever the player has eaten
//
// Output: none
// ================================================================================================
void Player::Grow(float foodSize)
{
	m_currentDimension += foodSize;

	// Update Player Center
	m_position.x += (foodSize / 2);
	m_position.y += (foodSize / 2);

	m_piece.Update(m_position, m_currentDimension);
	CalculateSpeed();
}

// ===== CalculateSpeed ===========================================================================
// Method will adjust the current speed of this piece based on its' current dimension.
//
// Input: none
// Output: none
// ================================================================================================
void Player::CalculateSpeed(void)
{
	// Get percentage of current size that starting size is
	float percentage = (START_DIMENSION / m_currentDimension);

	// Apply Percentage to base move rate
	m_moveRate = (percentage * BASE_MOVE_RATE);
}

// ===== GetBoundingRectangle =====================================================================
// Returns the bounding rectangle of the players m_piece.
//
// Input: none
// Output: 
//	[OUT] sf::Rect<float>	-	the contained GamePiece's bounding rectangle
// ================================================================================================
sf::Rect<float> Player::GetBoundingRectangle(void)
{
	return m_piece.getGlobalBounds();
}

// ===== CollidesWith(player) =====================================================================
// Method will check if the calling player's bounding rectangle intersects with the bounds of the
// passed player. Method will return false if any error is detected while checking collision.
//
// Input:
//	[IN]	const Player &other
//
// Output:
//	[OUT]	bool intersects		- true if the rectangles intersect, false otherwise or if error
// ================================================================================================
bool Player::CollidesWith(const Player& other) const
{
	return m_piece.getGlobalBounds().intersects(other.m_piece.getGlobalBounds());
}

// ===== CollidesWith(pellet) =====================================================================
// Method will check if the calling player's bounding rectangle intersects with the bounds of the
// passed pellet. Method will return false if any error is detected while checking collision.
//
// Input:
//	[IN]	const Pellet &pellet
//
// Output:
//	[OUT]	bool intersects		- true if the rectangles intersect, false otherwise or if error
// ================================================================================================
bool Player::CollidesWith(const Pellet& pellet) const
{
	return m_piece.getGlobalBounds().intersects(pellet.getGlobalBounds());
}

// ===== OutOfBounds ==============================================================================
// Method will compare passed players bounding rectangle with the bounds of the game board. If the
// players rectangle has intersected with or left the bounding rectangle of the board, this method
// will return true. This method will return false if there are errors.
//
// Input:
//	[IN]	const Player &player
//
// Output:
//	[OUT]	bool intersects		- true if out of bounds, false otherwise or if error
// ================================================================================================
bool Player::OutOfBounds(void) const
{
	if (((m_position.x + (m_currentDimension / 2)) >= GameData::BOARD_WIDTH) ||
		((m_position.x - (m_currentDimension / 2)) <= 0)	||
		((m_position.y + (m_currentDimension / 2)) >= GameData::BOARD_HEIGHT) || 
		((m_position.y - (m_currentDimension / 2) <= 0)))
	{
		return true;
	}
	else
	{
		return false;
	}
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
			<< data.GetPosition().x
			<< data.GetPosition().y
			<< data.GetDirection()
			<< data.IsActive()
			<< data.IsDead()
			<< data.GetMoveRate()
			<< data.GetDimension();

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
	float xCord;
	float yCord;
	int dir;
	bool state;
	bool dead;
	float moveRate;
	float dimension;

	packet	>> username 
			>> playerNum
			>> xCord
			>> yCord
			>> dir
			>> state
			>> dead
			>> moveRate
			>> dimension;

	data.SetUsername(username);
	data.SetPlayerNumber(playerNum);
	data.SetPosition(xCord, yCord);
	data.SetDirection((Player::Direction)dir);
	data.SetActive(state);
	data.SetDead(dead);
	data.SetMoveRate(moveRate);
	data.SetDimension(dimension);

	return packet;
}