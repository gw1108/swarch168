// ================================================================================================
// Filename: "CNetworkController.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: April 04, 2014
// ================================================================================================
// This is the class implementation file for the CNetworkController class. For a class description
// see the header file "CNetworkController.h"
// ================================================================================================

#include <iostream>
#include <SFML\Network\Packet.hpp>
#include <Windows.h>
#include "CNetworkController.h"

const int CNetworkController::MAX_QUEUE = 1;

// ===== Constructor ==============================================================================
// The constructor will attempt to establish a connection to the passed IP and port number. If
// successful, it will start the listening thread and set m_connected to true. If the connection
// fails, it will not start the thread and will set m_connected to false.
//  ===============================================================================================
CNetworkController::CNetworkController(std::string ipAddress, int portNumber) 
{
}

// ===== Destructor ===============================================================================
// The destructor will ensure all dynamically allocated memory is released.
// ================================================================================================
CNetworkController::~CNetworkController()
{
}

// ===== StopListeningThread ======================================================================
// Method will terminate the socket listening thread.
//
// Input: none
// Output: none
// ================================================================================================
void CNetworkController::StopListeningThread(void)
{
}


// ===== SocketListening ==========================================================================
// This method will loop while connected to the server. When a packet is received, the method
// will check the command code and handle the data accordingly.
//
// Input: none
// Output: none
// ================================================================================================
void CNetworkController::SocketListening(void)
{
}

// ===== SendPacket ===============================================================================
// This method will send the packet to the server.
//
// Input: 
//	[IN] sf::Packet packet - the packet to be sent
//
// Output: none
// ================================================================================================
void CNetworkController::SendPacket(sf::Packet packet)
{
}



