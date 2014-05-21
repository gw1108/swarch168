// ================================================================================================
// Filename: "Player.cpp"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 20, 2014
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
	m_direction(GamePiece::UP),
	m_active(false),
	m_dead(false),
	m_currentDimension(START_DIMENSION),
	m_moveRate(BASE_MOVE_RATE)
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
	m_direction(GamePiece::UP),
	m_active(active),
	m_dead(false),
	m_currentDimension(START_DIMENSION),
	m_moveRate(BASE_MOVE_RATE)
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
	m_active = other.m_active;
	m_dead = other.m_dead;
	m_moveRate = other.m_moveRate;
	m_currentDimension = other.m_currentDimension;
}

// ===== ReSpawn ==================================================================================
// Will respawn the player in a randomly generated position. This method should be called after
// death.
//
// Input:
//	[IN]	sf::Vector2f destination	- a vector that represents the position that the piece 
//										  will be moved to
//
// Output: none
// ================================================================================================
void Player::ReSpawn(void)
{
	ResetSize();

	float xCoord = 0;
	float yCoord = 0;

	xCoord = (float)((rand() % (GameData::BOARD_WIDTH)));	

	if(xCoord <= 10)
	{
		xCoord = 10;
	}
	else if(xCoord >= (GameData::BOARD_WIDTH - 10))
	{
		xCoord = (GameData::BOARD_WIDTH - 10.f);
	}

	yCoord = (float)((rand() % (GameData::BOARD_HEIGHT)));	

	if(yCoord <= 10)
	{
		yCoord = 10;
	}
	else if(yCoord >= (GameData::BOARD_HEIGHT - 10))
	{
		yCoord = (GameData::BOARD_HEIGHT - 10.f);
	}

	sf::Vector2f position(xCoord, yCoord);

	m_position = position;
	m_dead = false;
}

// ===== TakeTurn =================================================================================
// The TakeTurn method will adjust the position by the m_moveRate in the direction passed. This
// should be called on every game loop.
//
// Input:
//	[IN]	int direction	- an int that specifies the direction of movement
//
// Output: none
// ================================================================================================
void Player::TakeTurn(GamePiece::Direction direction)
{
	m_direction = direction;

	if(direction == GamePiece::UP)
	{
		m_position.y -= m_moveRate; 
	}
	else if(direction == GamePiece::DOWN)
	{
		m_position.y += m_moveRate; 
	}
	else if(direction == GamePiece::RIGHT)
	{
		m_position.x += m_moveRate;
	}
	else if(direction == GamePiece::LEFT)
	{
		m_position.x -= m_moveRate;  
	}
	else
	{
		// Invalid direction passed
	}
}

void Player::TakeTurn(void)
{
	TakeTurn(m_direction);
}

// ===== Grow(pellet)==============================================================================
// The Grow method will adjust the size of the GamePiece by the PELLET_GROW_SIZE specified in 
// GameData.
//
// Input: none
// Output: none
// ================================================================================================
void Player::Grow(void)
{
	m_currentDimension += GameData::PELLET_GROW_SIZE;

	CalculateSpeed();
}

// ===== Grow(player) =============================================================================
// This overloaded version of Grow will take the size of the opponent that was just eaten and use
// it to calculate the new size of this piece.
//
// Input: 
//		[IN]	int opponentSize	- the m_currentDimension of the eaten GamePiece
//
// Output: none
// ================================================================================================
void Player::Grow(int opponentSize)
{
	m_currentDimension += opponentSize;

	CalculateSpeed();
}

// ===== ResetSize ================================================================================
// Will reset the m_currentDimension to the START_DIMENSION. This should be called whenever a 
// GamePiece is killed or when starting a new game.
//
// Input: none
// Output: none
// ================================================================================================
void Player::ResetSize(void)
{
	m_currentDimension = START_DIMENSION;

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

// ===== GetRectangle =============================================================================
// TODO
//
// Input: none
// Output: none
// ================================================================================================
sf::Rect<float> Player::GetRectangle(void)
{
	return sf::Rect<float>(m_position.x, m_position.y, m_currentDimension, m_currentDimension);
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
	data.SetDirection((GamePiece::Direction)dir);
	data.SetActive(state);
	data.SetDead(dead);
	data.SetMoveRate(moveRate);
	data.SetDimension(dimension);

	return packet;
}