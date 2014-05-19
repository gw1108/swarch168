#pragma once
#include "SFML\Network.hpp"
#include <memory>

class SPlayer
{
public:
	SPlayer(void);
	SPlayer(sf::TcpSocket& socket);
	~SPlayer(void);

	int m_playerNumber;
	bool m_loggedIn;
};