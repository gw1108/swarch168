// ================================================================================================
// Filename: "CNetworkController.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 13, 2014
// ================================================================================================
// This is the class implementation file for the CNetworkController class. For a class description
// see the header file "CNetworkController.h"
// ================================================================================================

#include <SFML\Network\Packet.hpp>
#include <Windows.h>
#include "CNetworkController.h"
#include "LogInData.h"

const int CNetworkController::MAX_QUEUE = 1;

// ===== Constructor ==============================================================================
// Constructor will perform basic initialization of the class. 
//  ===============================================================================================
CNetworkController::CNetworkController(const sf::Clock& gameClock) :
	m_gameClock(gameClock),
	m_listeningThread(nullptr),
	m_serverConnection(),
	m_connected(false),
	m_dataQueue(),
	m_dataLock(),
	m_playerNum(-1),
	m_responded(false),
	m_serverResponse(-1)
{}

// ===== Destructor ===============================================================================
// The destructor will ensure all dynamically allocated memory is released.
// ================================================================================================
CNetworkController::~CNetworkController()
{
	StopListeningThread();
}

// ===== Connect ==================================================================================
// Attempts to establish a connection to the server using the passed IP address and port number.
// The method will catch exceptions and return a boolean indicating if the connection was
// successful. If connection to the server is established, the method will start the listening
// thread.
//
// Input:
//	[IN] sf::ipAddress ipAddress	-	the IP address of the server
//	[IN] int portNumber				-	the port number that the server is listening to
//
// Output: 
//	[OUT] bool						- true if connection established, false otherwise
// ================================================================================================
bool CNetworkController::Connect(std::string ipAddress, int portNumber)
{
	// Attempt to establish connection
	sf::Time timeOut = sf::seconds(5); // Wait time for server connection
	sf::Socket::Status returnCode;
	returnCode = m_serverConnection.connect(ipAddress, portNumber, timeOut);

	if(returnCode == sf::TcpSocket::Done)
	{
		// Connection established; Start thread
		m_connected = true;
		m_listeningThread = new std::thread(&CNetworkController::SocketListening, this);

		return true;
	}
	else
	{
		// Error Starting Connection
		return false;
	}
}

// ===== Disconnect ===============================================================================
// Terminates the current connection to the server. This method will also reset the members used
// in setting up the initial connection. 
//
// Input: none
// Output: none
// ================================================================================================
void CNetworkController::Disconnect(void)
{
	m_serverConnection.disconnect();
	m_connected = false;
	m_responded = false;
	m_serverResponse = (-1);
}

// ===== StopListeningThread ======================================================================
// Method will terminate the socket listening thread.
//
// Input: none 
// Output: none
// ================================================================================================
void CNetworkController::StopListeningThread(void)
{
	if (m_listeningThread != nullptr)
	{
		m_connected = false;		// Set Thread-Loop conditional to false
		m_listeningThread->join();	// Wait for thread to end
		delete m_listeningThread;
		m_listeningThread = nullptr;
	}
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
	sf::TcpSocket::Status receiveStatus;
	sf::Packet receivedPacket;
	sf::Uint8 cmdCode;

	while(m_connected)
	{
		receiveStatus = m_serverConnection.receive(receivedPacket);

		if(receiveStatus == sf::TcpSocket::Done)
		{
			receivedPacket >> cmdCode;

			if(cmdCode == GameData::INITIALIZE)
			{
				// Get assigned player number from packet
				sf::Uint8 serverResponse;
				sf::Uint8 playerNum;

				receivedPacket >> serverResponse >> playerNum;

				m_responded = true;
				m_serverResponse = serverResponse;
				m_playerNum = playerNum;
			}
			else if(cmdCode == GameData::GAME_UPDATE)
			{
				GameData newData;
				receivedPacket >> newData;

				m_dataLock.lock();		// Lock Data

				if(m_dataQueue.size() > MAX_QUEUE)
				{
					m_dataQueue.pop_front();	// Remove the oldest packet from queue
				}

				m_dataQueue.push_back(newData);

				m_dataLock.unlock();	// Unlock Data
			}
		}
		else if(receiveStatus == sf::TcpSocket::Disconnected)
		{
			Disconnect();
		}
	}
}

// ===== GetNextData ==============================================================================
// The method will update the passed GameData reference with the most recently sent data from the
// server. If no new data exists from the server, the method will not change the passed reference
// and return false.
// 
// Input:
//	[IN/OUT] GameData &dataRef	-	a reference to the clients GameData object
//
// Output:
//	[OUT] bool					-	true if updated, false otherwise
// ================================================================================================
bool CNetworkController::GetNextData(GameData &dataRef)
{
	if(!m_dataQueue.empty())
	{
		m_dataLock.lock();		// Lock Data

		GameData data = m_dataQueue.front();
		dataRef.CopyFrom(data);
		m_dataQueue.pop_front();

		m_dataLock.unlock();	// Unlock Data

		return true;
	}
	else
	{
		return false;
	}
}

// ===== SendLogIn ================================================================================
// This method will take the clients entered username and password and send them to the server.
//
// Input: 
//	[IN] std::string name	- username
//	[IN] std::string pw		- encrypted pw
//
// Output: none
// ================================================================================================
void CNetworkController::SendLogIn(std::string name, std::string pw)
{
	sf::Uint8 commandCode = GameData::LOG_IN;
	sf::Int32 timeStamp = m_gameClock.getElapsedTime().asMilliseconds();

	LogInData data(name, pw);

	sf::Packet dataPacket;
	dataPacket << commandCode << data << timeStamp;

	m_serverConnection.send(dataPacket);
}

// ===== SendDirectionChange =========================================================================
// This method will take the clients paddle information and package it along with a timestamp. Once
// packaged, the method will send the packet through the m_serverConnection.
//
// Input: 
//	[IN] double yLocation	- the current Y-Coordinate of the top of the paddle
//	[IN] int direction		- an int representing the direction the paddle is currently moving
//
// Output: none
// ================================================================================================
void CNetworkController::SendDirectionChange(Player::Direction direction)
{
	sf::Uint8 commandCode = GameData::PLAYER_UPDATE;
	sf::Int32 timeStamp = m_gameClock.getElapsedTime().asMilliseconds();
	sf::Uint8 sfDirection = direction;

	sf::Packet dataPacket;
	dataPacket << commandCode << timeStamp << sfDirection;

	m_serverConnection.send(dataPacket);
}