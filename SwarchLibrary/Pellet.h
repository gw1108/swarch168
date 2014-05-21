// ================================================================================================
// Filename: "Pellet.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: Apr 16, 2014
// ================================================================================================
// Class Description:
// 
// The Pellet class will extend an sf::RectangleShape and provide the functionality needed for a 
// rectangle to operate as a food pellet during gameplay.
// ================================================================================================

#ifndef PELLET_H
#define PELLET_H

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Network\Packet.hpp>

class Pellet: public sf::RectangleShape
{

public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	Pellet();

	// Method Prototypes
	void Spawn(void);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Constants
	static const float DIMENSION;
	static const sf::Color COLOR;
};

// Packet Overloads for GameData
sf::Packet& operator<<(sf::Packet& packet, Pellet& pellet);
sf::Packet& operator>>(sf::Packet& packet, Pellet& pellet);

#endif