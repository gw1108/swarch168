// ================================================================================================
// Filename: "IpGetter.h"
// ================================================================================================
// Author(s): Travis Smith, George Wang
// Last Modified: April 16, 2014
// ================================================================================================
// Class Description:
// 
// The IpGetter is an encapsulation of the command line interface used when the client first starts
// the game to get the IP address of the server. 
//
// It is responsible for displaying the appropriate messages to the user and getting the user 
// input. It will check the validity of the input IpAddress and handle input errors so that the
// ClientNetworkController recieves a valid ip format.
//
// The class will utilize a static interface and should not be instantiated.
// ================================================================================================ 

#ifndef IPGETTER_H
#define IPGETTER_H

#include <iostream>
#include <string>

class IpGetter
{

public:

	// Constants
	static const std::string SERVER_LOCAL_HOST;
	static const std::string EXIT_COMMAND;

	// Prototypes
	static std::string GetIP(void);

private:

	// Constructor Prototypes, Prevent Instantiation
	IpGetter(void);
	IpGetter(const IpGetter& other);

	// Prototypes
	static std::string InputIP(void);
	static bool CheckIP(std::string input);
	static unsigned GetNumberOfDigits(unsigned i);
};

#endif