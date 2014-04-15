// ================================================================================================
// Filename: "CNetworkController.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: April 04, 2014
// ================================================================================================
// Class Description:
// 
// The CNetworkController will be responsible for establishing a connection to the the server, 
// creating and controlling the listening thread, and the sending/receiving of data to/from the 
// server.
//
// The class will contain a thread that will be used to monitor the listening socket for data sent
// from the server. The thread will be responsible for extracting the packet from the socket, 
// decoding the packet, and updating the game data. The method that the thread will be utilizing 
// will be contained within this class.
//
// When an instance of CNetworkController is created, it will attempt to establish a connection with
// the passed IP-Address and port number. If the connection fails, the classes "IsConnected()" method
// will return false. This method should be called by the client after the instance is created to 
// ensure the connection is ready.
//
// All network related data, including timestamps, will be handled by the CNetworkController. Classes
// that wish to send data, will pass the data and the CNetworkController will handle the packaging and
// sending.
//
// Packets being sent to and from the server will contain a command code and the appropriate data
// for that command. Command codes are defined in the ClientHandler class in the PongServer project.
// ================================================================================================

#ifndef CNETWORKCONTROLLER_H
#define CNETWORKCONTROLLER_H

#include <thread>
#include <string>
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Clock.hpp>

class CNetworkController
{

public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	static const int MAX_QUEUE; 

	// Constructor/Destructor
	CNetworkController(std::string ipAddress, int portNumber);
	~CNetworkController(void);

	// Method Prototypes
	void StopListeningThread(void);

	// Inlined Methods
	bool IsConnected(){ return m_connected; }
	int GetPlayerNumber(void){ return m_playerNum; }
	bool ShouldStartGame(void){ return m_startGame; }

private:

	// ============================================================================================
	// Data Members
	// ============================================================================================

	// Connection Data
	std::thread *m_listeningThread;
	sf::TcpSocket m_serverConnection;
	bool m_connected;

	// Game Data
	int m_playerNum;
	const sf::Clock *m_gameClock;
	bool m_startGame;

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Prototypes
	void SocketListening(void);
	void SendPacket(sf::Packet packet);
};

#endif