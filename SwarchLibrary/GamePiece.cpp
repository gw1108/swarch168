// ================================================================================================
// Filename: "GamePiece.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 16, 2014
// ================================================================================================
// This is the class implementation file for the GamePiece class. For a class description see the 
// header file "GamePiece.h"
// ================================================================================================

#include "GamePiece.h"
#include "GameData.h"

// Initialize Static Constants
const float GamePiece::START_DIMENSION = 20;
const float GamePiece::BASE_MOVE_RATE = 3;
const sf::Color GamePiece::PLAYER0_COLOR = sf::Color::Blue;
const sf::Color GamePiece::PLAYER1_COLOR = sf::Color::Red;
const sf::Color GamePiece::PLAYER2_COLOR = sf::Color::Green;
const sf::Color GamePiece::PLAYER3_COLOR = sf::Color::Yellow;

// ===== Constructor ==============================================================================
// The constructor will use class constants to define the size of the sf::RectangleShape parent.
// It will abstract away most of the set-up required for an sf::RectangleShape. The color and 
// starting position of the GamePiece will be dictated by which player is passed to the constructor.
// 
// Input:
//	[IN] int player	-	player assigned for this GamePiece, 1 is always this clients user
//
// Output: none
//  ===============================================================================================
GamePiece::GamePiece(void) :
	m_currentDimension(START_DIMENSION),
	m_moveRate(BASE_MOVE_RATE), 
	m_direction(Direction::DOWN), 
	m_playerID(0)
{
	setSize(sf::Vector2f(START_DIMENSION, START_DIMENSION));
	setOrigin(sf::Vector2f((START_DIMENSION / 2), (START_DIMENSION / 2)));
	setFillColor(sf::Color::White);
}

// ===== Constructor ==============================================================================
// The constructor will use class constants to define the size of the sf::RectangleShape parent.
// It will abstract away most of the set-up required for an sf::RectangleShape. The color and 
// starting position of the GamePiece will be dictated by which player is passed to the constructor.
// 
// Input:
//	[IN] int player	-	player assigned for this GamePiece, 1 is always this clients user
//
// Output: none
//  ===============================================================================================
GamePiece::GamePiece(int player) : 
	m_currentDimension(START_DIMENSION),
	m_moveRate(BASE_MOVE_RATE), 
	m_direction(Direction::DOWN), 
	m_playerID(player)
{
	setSize(sf::Vector2f(START_DIMENSION, START_DIMENSION));
	setOrigin(sf::Vector2f((START_DIMENSION / 2), (START_DIMENSION / 2)));
	setFillColor(sf::Color::White);
}

// ===== SetPlayerNumber ==========================================================================
// Sets the assigned players number and color to this piece. Should be called after initialized.
//
// Input:
//	[IN]	int playerNum	- assigned player
//
// Output: none
// ================================================================================================
void GamePiece::SetPlayerNumber(int playerNum)
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
void GamePiece::ReSpawn(void)
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

	setPosition(position);
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
void GamePiece::TakeTurn(Direction direction)
{
	m_direction = direction;
	if(direction == UP)
	{
		move(0, ((-1) * m_moveRate)); 
	}
	else if(direction == DOWN)
	{
		move(0, m_moveRate); 
	}
	else if(direction == RIGHT)
	{
		move(m_moveRate, 0); 
	}
	else if(direction == LEFT)
	{
		move(((-1) * m_moveRate), 0); 
	}
	else
	{
		// Invalid direction passed
	}
}

void GamePiece::TakeTurn(void)
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
void GamePiece::Grow(void)
{
	m_currentDimension += GameData::PELLET_GROW_SIZE;

	setSize(sf::Vector2f(m_currentDimension, m_currentDimension));
	setOrigin(sf::Vector2f((m_currentDimension / 2), (m_currentDimension / 2)));

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
void GamePiece::Grow(int opponentSize)
{
	m_currentDimension += opponentSize;

	setSize(sf::Vector2f(m_currentDimension, m_currentDimension));
	setOrigin(sf::Vector2f((m_currentDimension / 2), (m_currentDimension / 2)));

	CalculateSpeed();
}

// ===== ResetSize ================================================================================
// Will reset the m_currentDimension to the START_DIMENSION. This should be called whenever a 
// GamePiece is killed or when starting a new game.
//
// Input: none
// Output: none
// ================================================================================================
void GamePiece::ResetSize(void)
{
	m_currentDimension = START_DIMENSION;

	setSize(sf::Vector2f(m_currentDimension, m_currentDimension));
	setOrigin(sf::Vector2f((m_currentDimension / 2), (m_currentDimension / 2)));

	CalculateSpeed();
}

// ===== CalculateSpeed ===========================================================================
// Method will adjust the current speed of this piece based on its' current dimension.
//
// Input: none
// Output: none
// ================================================================================================
void GamePiece::CalculateSpeed(void)
{
	// Get percentage of current size that starting size is
	float percentage = (START_DIMENSION / m_currentDimension);

	// Apply Percentage to base move rate
	m_moveRate = (percentage * BASE_MOVE_RATE);
}