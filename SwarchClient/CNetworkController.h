// ================================================================================================
// Filename: "CNetworkController.h"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 13, 2014
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
// All network related data, including timestamps, will be handled by the CNetworkController. Classes
// that wish to send data, will pass the data and the CNetworkController will handle the packaging and
// sending.
//
// Packets being sent to and from the server will contain a command code and the appropriate data
// for that command. Command codes are defined in the ClientHandler class in the SwarchServer project.
// ================================================================================================

#ifndef CNETWORKCONTROLLER_H
#define CNETWORKCONTROLLER_H

#include <thread>
#include <mutex>
#include <string>
#include <list>
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Clock.hpp>
#include "GameData.h"
#include "GamePiece.h"
#include "Player.h"

class CNetworkController
{

public:

	// ============================================================================================
	// Methods
	// ============================================================================================

	static const int MAX_QUEUE; 

	// Constructor/Destructor
	CNetworkController(const sf::Clock& gameClock);
	~CNetworkController(void);

	// Method Prototypes
	bool Connect(std::string ipAddress, int portNumber);
	void Disconnect(void);
	void StopListeningThread(void);
	bool GetNextData(GameData &dataRef);
	void SendLogIn(std::string name, std::string pw);
	void SendDirectionChange(GamePiece::Direction direction);

	// Inlined Methods
	bool IsConnected(void){ return m_connected; }
	int GetPlayerNumber(void){ return m_playerNum; }
	bool ServerResponded(void) { return m_responded; }
	int GetServerResponse(void) { return m_serverResponse; }

private:

	// ============================================================================================
	// Data Members
	// ============================================================================================

	// Connection Data
	std::thread *m_listeningThread;
	sf::TcpSocket m_serverConnection;
	bool m_connected;

	// GameData Queue
	std::list<GameData> m_dataQueue;
	std::mutex m_dataLock;

	// Client Data
	int m_playerNum;
	const sf::Clock  &m_gameClock;

	// Login Data
	bool m_responded;
	int m_serverResponse;

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Prototypes
	void SocketListening(void);
};

#endif