// ================================================================================================
// Filename: "IpGetter.cpp"
// ================================================================================================
// Author(s): Travis Smith, George Wang
// Last Modified: April 16, 2014
// ================================================================================================
// This is the implementation file for the IpGetter class. For class description see the header 
// file "IpGetter.h"
// ================================================================================================

#include "IpGetter.h"

// Define Constants
const std::string IpGetter::SERVER_LOCAL_HOST = "127.0.0.1"; 
const std::string IpGetter::EXIT_COMMAND = "exit";

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
std::string IpGetter::GetIP(void)
{
	std::string ipString;
	do
	{
		ipString = InputIP();
		
		if(_stricmp(ipString.c_str(), "LH") == 0)
		{
			std::cout << "Local Host Specified\n";
			return IpGetter::SERVER_LOCAL_HOST;
		}
	}while(!CheckIP(ipString));
	return ipString;
}

std::string IpGetter::InputIP(void)
{
	char tempIP[256];

	fgets(tempIP, 256, stdin);	// Get the next characters off input stream

	std::string command = std::string(tempIP);
	
	//remove \n
	command.pop_back();
	
	return command;
}

bool IpGetter::CheckIP(std::string input)
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

unsigned IpGetter::GetNumberOfDigits (unsigned i)
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}