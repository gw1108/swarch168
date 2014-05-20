#pragma once
#include "GameData.h"

class ServerData
{
public:
	enum ServerCommands {LOG_IN, LOG_OUT, PLAYER_UPDATE};

	ServerData(int playerNumber, ServerCommands command);
	~ServerData(void);
	

	int m_playerNumber;
	ServerCommands m_command;
	int direction;

	//login means new player

	//logout means remove player

	//player update means player has changed directions
};