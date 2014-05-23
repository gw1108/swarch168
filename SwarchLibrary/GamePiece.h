// ================================================================================================
// Filename: "GamePiece.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 22, 2014
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
	GamePiece(int player, sf::Vector2f position, float dimension);

	// Method Prototypes
	void Rebuild(int player, sf::Vector2f position, float dimension);
	void Update(sf::Vector2f position, float dimension);
	void Update(sf::Vector2f position);
	void Update(float dimension);
	void SetColor(int playerNum);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Constants
	static const sf::Color PLAYER0_COLOR;
	static const sf::Color PLAYER1_COLOR;
	static const sf::Color PLAYER2_COLOR;
	static const sf::Color PLAYER3_COLOR;

private:

};

#endif