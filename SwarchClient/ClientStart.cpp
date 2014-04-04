// ================================================================================================
// Filename: "ClientStart.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: April 04, 2014
// ================================================================================================
// This file is where client execution will start. It will get the server IP from the user, attempt
// a connection and if successful, will prepare the client for play.
// ================================================================================================

#include <iostream>
#include <string>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Clock.hpp>

// Game Constants
const int SERVER_PORT = 8585;
const std::string SERVER_LOCAL_HOST = "127.0.0.1"; 

// Function Prototypes
std::string GetIP(void);

int main(int argc, char* argv[])
{
	// Introduction Output
	std::cout << "Swarch Client\nClass: ICS 168\n";

	// Connection Data
	std::string ipString;
	sf::IpAddress serverIP;
	sf::Clock gameClock;

	ipString = GetIP();

	// Attempt Connection to Server
	// Failure, Close program
	// Success, Start Engine
}

// ===== GetIP ====================================================================================
// Will get a string from the user and check if it is a valid IP address or if the user specified
// local host. If the string is valid, or LH, it will be returned. While the string is not valid,
// the method will continue to loop unless the exit command is given or a valid string is entered.
//
// Input: none
//
// Output: 
//	[OUT] ipAddress - a std::string representing the ip address of the server
// ================================================================================================
std::string GetIP(void)
{

}
