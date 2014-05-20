#include "ServerData.h"


ServerData::ServerData(int playerNumber, ServerCommands command)
	:m_playerNumber(playerNumber),
	m_command(command)
{
}


ServerData::~ServerData(void)
{
}
