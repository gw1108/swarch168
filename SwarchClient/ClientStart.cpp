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
#include "CNetworkController.h"

// Game Constants
const int SERVER_PORT = 8585;
const std::string SERVER_LOCAL_HOST = "127.0.0.1"; 
const std::string EXIT_COMMAND = "exit";

// Function Prototypes
std::string GetIP(void);
unsigned GetNumerOfDigits(unsigned i);
void DelayInput(void);

int main(int argc, char* argv[])
{
	// Introduction Output
	std::cout << "Swarch Client\nClass: ICS 168\n";

	// Connection Data
	std::string ipString;
	sf::IpAddress serverIP;
	sf::Clock gameClock;

	ipString = GetIP();
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
	return 0;
}

void DelayInput(void)
{
	std::cout << "Press any key to continue...";
	getchar();
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
	std::string ipString;
	do
	{
		ipString = InputIP();
		
		if(_stricmp(ipString.c_str(), "LH") == 0)
		{
			std::cout << "Local Host Specified\n";
			return SERVER_LOCAL_HOST;
		}
	}while(!CheckIP(ipString));
	return ipString;
}

std::string InputIP(void)
{
	char tempIP[256];

	fgets(tempIP, 256, stdin);	// Get the next characters off input stream

	std::string command = std::string(tempIP);
	
	//remove \n
	command.pop_back();
	
	return command;
}

bool CheckIP(std::string input)
{
	std::string delimiter = ".";
	
	//check if it's now empty
	if(input.empty())
	{
		std::cout << "Error Parsing : empty string detected";
		return false;
	}

	if(_stricmp(input.c_str(), "exit") == 0)
	{
		return true;
	}

	//check if everything is a number or the delimeter
	if(input.find_first_not_of("0123456789"+delimiter) != std::string::npos)
	{
		std::cout << "Error Parsing : invalid character " + input.find_first_not_of("0123456789"+delimiter);
		return false;
	}

	size_t pos;
	int value;
	int numberOfDelimeters = 0;
	while(input.length() != 0 && numberOfDelimeters < 4){
		if((pos = input.find(delimiter)) == std::string::npos)
		{
			pos = input.length();
		}
		//check if s.substr(0,pos); is a int
		value = atoi(input.substr(0,pos).c_str());
		if(GetNumberOfDigits(value) > 3)
		{
			std::cout << "Error Parsing : 4 or more numbers detected between '.'";
			return false;
		}
		input.erase(0, pos + delimiter.length());
		numberOfDelimeters++;
	}
	if(numberOfDelimeters > 3)
	{
		std::cout << "Error Parsing : 4 or more " + delimiter + " detected";
		return false;
	}

	std::cout << "IP is Valid : " << input << std::endl;
	return true;
}

unsigned GetNumberOfDigits (unsigned i)
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}