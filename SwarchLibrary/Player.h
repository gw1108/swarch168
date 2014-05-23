// ================================================================================================
// Filename: "Player.h"
// ================================================================================================
// Author: Travis Smith
// Last Modified: May 22, 2014
// ================================================================================================
// Class Description:
// 
// The Player class will hold the data for an individual player currently in the game.
// ================================================================================================

#pragma once

#include <string>
#include <SFML\Network\Packet.hpp>
#include "GamePiece.h"
#include "Pellet.h"

class Player
{
public:

	enum Direction {UP, DOWN, LEFT, RIGHT};

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	Player(void);
	Player(std::string userName, int playerNum, sf::Vector2f position, bool active);

	// Prototypes
	void CopyFrom(const Player &other);
	void ReSpawn(void);
	void TakeTurn(void);	
	void Grow(float opponentSize);
	sf::Rect<float> GetBoundingRectangle(void);
	bool CollidesWith(const Player& other) const;
	bool CollidesWith(const Pellet& pellet) const;
	bool OutOfBounds(void) const;

	// ===== Inlined Methods =====
	void Grow(void) { Grow(4.f); }

	// Getters
	std::string GetUsername(void) { return m_username; } 
	int GetAssignedNumber(void) { return m_playerNum; } 
	sf::Vector2f GetPosition(void) { return m_position; }
	Direction GetDirection(void) { return m_direction; }
	bool IsActive(void) {  return m_active; }
	bool IsDead(void) { return m_dead; }
	float GetMoveRate(void) { return m_moveRate; }
	float GetDimension(void) { return m_currentDimension; }
	GamePiece GetPiece(void) { return m_piece; };

	// Setters
	void SetUsername(std::string username) { m_username = username; }
	void SetPlayerNumber(int playerNum) { m_playerNum = playerNum; m_piece.SetColor(playerNum); }
	void SetPosition(sf::Vector2f newPosition) { m_position = newPosition; }
	void SetPosition(float xCord, float yCord) { m_position.x = xCord; m_position.y = yCord; }
	void SetDirection(Direction direction) { m_direction = direction; }
	void SetActive(bool state) { m_active = state; }
	void SetDead(bool state) { m_dead = state; }
	void SetMoveRate(float rate) { m_moveRate = rate; }
	void SetDimension(float dime) { m_currentDimension = dime; }

private:

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Prototypes
	void CalculateSpeed(void);
	
	// Constants
	static const float START_DIMENSION;
	static const float BASE_MOVE_RATE;

	// Player Data
	std::string m_username;
	int m_playerNum;

	// Movement Data
	sf::Vector2f m_position;
	Direction m_direction;
	float m_moveRate;

	// Draw Data
	float m_currentDimension;
	GamePiece m_piece;

	// Gameplay Data
	bool m_active;
	bool m_dead;
};

// Packet Overloads for LogInData
sf::Packet& operator<<(sf::Packet& packet, Player& data);
sf::Packet& operator>>(sf::Packet& packet, Player& data);