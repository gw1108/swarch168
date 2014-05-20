// ================================================================================================
// Filename: "Position.h"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 20, 2014
// ================================================================================================
// Class Description:
// 
// The Position class will be used to represent a point on the game board. It will contain public
// member and is intended to be used as a private member of another class.
// ================================================================================================

#pragma once

#include <string>

class Position
{
public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Inlines
	Position(void) : m_xCoordinate(0), m_yCoordinate(0){}
	Position(int xCord, int yCord) : m_xCoordinate(xCord), m_yCoordinate(yCord){}

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	int m_xCoordinate;
	int m_yCoordinate;

};

