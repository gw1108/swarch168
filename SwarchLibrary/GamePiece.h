// ================================================================================================
// Filename: "GamePiece.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 16, 2014
// ================================================================================================
// Class Description:
// 
// The GamePiece class will extend an sf::RectangleShape and provide the functionality needed for a 
// rectangle to operate as a users "whale" during gameplay. The class will contain the methods that
// govern movement and changing size.
// ================================================================================================

#ifndef GAMEPIECE_H
#define GAMEPIECE_H

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Color.hpp>

class GamePiece: public sf::RectangleShape
{

public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	GamePiece(int player);

	// Method Prototypes
	void ReSpawn(void);
	void TakeTurn(int direction);
	void Grow(void);
	void Grow(int opponentSize);
	void ResetSize(void);
	
	// Inlined Methods
	int GetDimension(void){ return m_currentDimension; }
	int GetMoveRate(void){ return m_moveRate; }

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	enum Direction {UP, DOWN, LEFT, RIGHT};

	// Constants
	static const float START_DIMENSION;
	static const float BASE_MOVE_RATE;
	static const sf::Color PLAYER1_COLOR;
	static const sf::Color PLAYER2_COLOR;
	static const sf::Color PLAYER3_COLOR;

private:

	// Prototypes
	void CalculateSpeed(void);

	// Members
	float m_currentDimension;	
	float m_moveRate;
};

#endif