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
#include "GamePiece.h"

class Player
{
public:

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
	void TakeTurn(GamePiece::Direction direction);
	void Grow(void);
	void Grow(float opponentSize);
	void ResetSize(void);
	sf::Rect<float> GetRectangle(void);

	// Inlined Methods
	std::string GetUsername(void) { return m_username; } 
	int GetAssignedNumber(void) { return m_playerNum; } 
	sf::Vector2f GetPosition(void) { return m_position; }
	GamePiece::Direction GetDirection(void) { return m_direction; }
	bool IsActive(void) {  return m_active; }
	bool IsDead(void) { return m_dead; }
	float GetMoveRate(void) { return m_moveRate; }
	float GetDimension(void) { return m_currentDimension; }
	void SetUsername(std::string username) { m_username = username; }
	void SetPlayerNumber(int playerNum) { m_playerNum = playerNum; }
	void SetPosition(sf::Vector2f newPosition) { m_position = newPosition; }
	void SetPosition(float xCord, float yCord) { m_position.x = xCord; m_position.y = yCord; }
	void SetDirection(GamePiece::Direction direction) { m_direction = direction; }
	void SetActive(bool state) { m_active = state; }
	void SetDead(bool state) { m_dead = state; }
	void SetMoveRate(float rate) { m_moveRate = rate; }
	void SetDimension(int dime) { m_currentDimension = dime; }

private:

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Prototypes
	void CalculateSpeed(void);
	
	// Constants
	static const float START_DIMENSION;
	static const float BASE_MOVE_RATE;

	std::string m_username;
	int m_playerNum;
	sf::Vector2f m_position;
	GamePiece::Direction m_direction;
	bool m_active;
	bool m_dead;
	float m_currentDimension;	
	float m_moveRate;

};

// Packet Overloads for LogInData
sf::Packet& operator<<(sf::Packet& packet, Player& data);
sf::Packet& operator>>(sf::Packet& packet, Player& data);