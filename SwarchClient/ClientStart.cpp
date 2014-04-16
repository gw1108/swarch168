// ================================================================================================
// Filename: "ClientStart.cpp"
// ================================================================================================
// Author(s): Travis Smith, George Wang
// Last Modified: April 04, 2014
// ================================================================================================
// This file is where client execution will start. It will get the server IP from the user, attempt
// a connection and if successful, will prepare the client for play.
// ================================================================================================

#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Clock.hpp>
#include "CNetworkController.h"
#include "IpGetter.h"
#include "ClientEngine.h"

// Game Constants
const int SERVER_PORT = 8585;

// Prototypes
void DelayInput(void);

int main(int argc, char* argv[])
{
	// Introduction Output
	std::cout << "Swarch Client\nClass: ICS 168\n";

	// Connection Data
	std::string ipString;
	sf::IpAddress serverIP;
	sf::Clock gameClock;

	/* Server Related code, uncomment once MP version is done.
	IpGetter::GetIP();

	ipString = IpGetter::GetIP();

	if(_stricmp(ipString.c_str(), "exit") == 0)
	{
		std::cout << "Exiting\n";
		DelayInput();
		return 0;
	}

	// Attempt Connection to Server
	CNetworkController networkController(ipString,SERVER_PORT);
	if(networkController.IsConnected())
	{
		// Success, Start Engine
	}
	else
	{
		// Failure, Close program
		std::cout << "Error attempting Server Connection.\n";
		DelayInput();
		return 1;
	}

	DelayInput();
	*/

	srand((unsigned int)(time(0)));

	ClientEngine engine(1);

	engine.Run();

	return 0;
}

void DelayInput(void)
{
	std::cout << "Press any key to continue...";
	getchar();
}