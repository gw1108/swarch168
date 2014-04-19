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
#include <Windows.h>
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
void SetStdinEcho(bool enable = true);

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

	// TODO: replace with a real class
	char userName[100];
	char dummyPassword[100];
	std::cout << "Please input your user name." << std::endl;
	std::cin.getline(userName,sizeof(userName));
	std::cout << "Please input your password (it will not be stored, used, or displayed)." << std::endl;
	SetStdinEcho(false);
	std::cin.getline(dummyPassword, sizeof(dummyPassword));
	SetStdinEcho(true);
	ClientEngine engine(1, userName);

	engine.Run();

	std::cout << "Exiting\n";
	DelayInput();

	return 0;
}

void DelayInput(void)
{
	system("pause");
}

void SetStdinEcho(bool enable)
{
	#ifdef WIN32
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
		DWORD mode;
		GetConsoleMode(hStdin, &mode);

		if( !enable )
			mode &= ~ENABLE_ECHO_INPUT;
		else
			mode |= ENABLE_ECHO_INPUT;

		SetConsoleMode(hStdin, mode );

	#else
		struct termios tty;
		tcgetattr(STDIN_FILENO, &tty);
		if( !enable )
			tty.c_lflag &= ~ECHO;
		else
			tty.c_lflag |= ECHO;

		(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	#endif
}