// ================================================================================================
// Filename: "Player.h"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 20, 2014
// ================================================================================================
// Class Description:
// 
// The Player class will hold the data for an individual player currently in the game.
// ================================================================================================

#pragma once

#include <string>
#include <SFML\Network\Packet.hpp>
#include "Position.h"
#include "GamePiece.h"

class Player
{
public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	Player(void);
	Player(std::string userName, int playerNum, Position position, bool active);

	// Prototypes
	void CopyFrom(const Player &other);

	// Inlined Methods
	std::string GetUsername(void) { return m_username; } 
	int GetAssignedNumber(void) { return m_playerNum; } 
	Position GetPosition(void) { return m_position; }
	GamePiece::Direction GetDirection(void) { return m_direction; }
	bool IsActive(void) {  return m_active; }
	bool IsDead(void) { return m_dead; }
	void SetUsername(std::string username) { m_username = username; }
	void SetPlayerNumber(int playerNum) { m_playerNum = playerNum; }
	void SetPosition(Position newPosition) { m_position = newPosition; }
	void SetDirection(GamePiece::Direction direction) { m_direction = direction; }
	void SetPosition(int xCord, int yCord) { m_position.m_xCoordinate = xCord; m_position.m_yCoordinate = yCord; }
	void SetActive(bool state) { m_active = state; }
	void SetDead(bool state) { m_dead = state; }

private:

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	std::string m_username;
	int m_playerNum;
	Position m_position;
	GamePiece::Direction m_direction;
	bool m_active;
	bool m_dead;

};

// Packet Overloads for LogInData
sf::Packet& operator<<(sf::Packet& packet, Player& data);
sf::Packet& operator>>(sf::Packet& packet, Player& data);