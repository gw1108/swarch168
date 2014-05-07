// ================================================================================================
// Filename: "CNetworkController.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 04, 2014
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
	m_listeningThread(NULL),
	m_serverConnection(),
	m_connected(false),
	m_dataQueue(),
	m_dataLock(),
	m_playerNum(-1),
	m_startGame(false),
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

	if(m_serverConnection.connect(ipAddress, portNumber, timeOut) == sf::TcpSocket::Done)
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
// Terminates the current connection to the server, if it exsists.
//
// Input: none
// Output: none
// ================================================================================================
void CNetworkController::Disconnect(void)
{
	StopListeningThread();
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
	if (m_listeningThread != NULL)
	{
		m_connected = false;		// Set Thread-Loop conditional to false
		m_listeningThread->join();	// Wait for thread to end
		delete m_listeningThread;
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
	m_serverConnection.setBlocking(false);

	while(m_connected)
	{
		receiveStatus = m_serverConnection.receive(receivedPacket);	// Blocking

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

				if(!m_startGame)
				{
					m_startGame = newData.startGame;	// Check for GameStart Command
				}

				m_dataLock.lock();		// Lock Data

				if(m_dataQueue.size() > MAX_QUEUE)
				{
					m_dataQueue.pop_front();
				}

				m_dataQueue.push_back(newData);

				m_dataLock.unlock();	// Unlock Data
			}
		}
		else if(receiveStatus == sf::TcpSocket::Disconnected)
		{
			m_connected = false;
		}
	}
}

// ===== GetNextData ==============================================================================
// The method will return the most recent GameData update received by the server. It will be 
// controlled by a mutex lock to ensure is doesn't grab data while the listening thread is writing
// new data.
// 
// Input: none
//
// Output:
//	[OUT] GameData m_newData	-	the latest GameData update from the server
// ================================================================================================
GameData CNetworkController::GetNextData(void)
{
	m_dataLock.lock();		// Lock Data

	GameData data = m_dataQueue.front();
	m_dataQueue.pop_front();

	m_dataLock.unlock();	// Unlock Data

	return data;
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

	SendPacket(dataPacket);
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
void CNetworkController::SendDirectionChange(GamePiece::Direction direction)
{
	sf::Uint8 commandCode = GameData::PLAYER_UPDATE;
	sf::Int32 timeStamp = m_gameClock.getElapsedTime().asMilliseconds();
	sf::Uint8 sfDirection = direction;

	sf::Packet dataPacket;
	dataPacket << commandCode << timeStamp << sfDirection;

	SendPacket(dataPacket);
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
	m_serverConnection.send(packet);
}



