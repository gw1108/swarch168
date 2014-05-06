#include "SNetworkController.h"
#include "LogInData.h"
#include <iostream>			//network controller debug

const float SNetworkController::CHECK_FREQUENCY = 2;

SNetworkController::SNetworkController(void)
	:m_networkThread(NULL),
	serverRunning(false)
{
	logInHandler = new LogIn();
	listener.setBlocking(false);
	listener.listen(GameData::SERVER_PORT);
	selector.add(listener);
}


SNetworkController::~SNetworkController(void)
{
	for(sf::TcpSocket* socket : clients)
	{
		delete socket;
	}
	delete logInHandler;
	stopNetwork();
}

void SNetworkController::waitForUsers(void)
{
	sf::Time idleTime = sf::seconds(CHECK_FREQUENCY);

	std::cout << "Start waiting for connections"<< std::endl;

	//wait forever until game start
	while(serverRunning)
	{
		// Make the selector wait for data on any socket
		if (selector.wait(idleTime))
		{
			std::cout << "a socket is ready" << std::endl;

			// Test the listener
			if (selector.isReady(listener))
			{
				std::cout << "new player detected" << std::endl;				
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					clients.push_back(client);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*client);
				}
				else
				{
					std::cout << "Error : no new player after all." << std::endl;
					// Error, we won't get a new connection, delete the socket
					delete client;
				}
			}
			// The listener socket is not ready, test all other sockets (the clients)
			m_datalock.lock();
			for (auto it = clients.begin(); it != clients.end(); ++it)
			{
				sf::TcpSocket& client = **it;
				if (selector.isReady(client))
				{
					std::cout << "client was ready" << std::endl;
					// The client has sent some data, we can receive it
					sf::Packet packet;
					sf::Socket::Status clientStatus = client.receive(packet);
					if (clientStatus == sf::Socket::Done)
					{
						//if packet is login check login
						sf::Uint8 commandCode;
						packet >> commandCode;
						if(commandCode == GameData::LOG_IN)
						{
							//if(logInHandler.login(username, password))
							//then mark the client as ready
							LogInData logInData;
							GameData::LoginResponse responseCode;
							packet >> logInData;
							responseCode = logInHandler->loginUser(logInData.GetUsername(), logInData.GetPassword());
							std::cout << "response code to log in " << responseCode << std::endl;
							packet.clear();

							//send back packet
							sf::Uint8 newCode = GameData::INITIALIZE;
							sf::Uint8 serverResponseCode = responseCode;
							sf::Uint8 clientNumber = 1;
							packet << newCode;
							packet << serverResponseCode;
							packet << clientNumber;		//todo next number
							std::cout << client.send(packet) << std::endl;

							//TODO : send initialize packet somewhere
						}
					}
					else if(clientStatus == sf::Socket::Disconnected)
					{
						//TODO delete this client
						client.disconnect();
						std::cout << "client disconnected" << std::endl;
						clientsToRemove.push_back(&client);
					}
				}
			}
			m_datalock.unlock();

			//removeDisconnectedClients();
		}
	}
}

GameData SNetworkController::getNextGameData(void)
{
	GameData data;
	m_datalock.lock();

	data = m_data.back();
	m_data.pop_back();

	m_datalock.unlock();
	return data;
}

bool SNetworkController::isGameDataAvailible(void)
{
	return m_data.size() != 0;
}

void SNetworkController::startNetwork(void)
{
	serverRunning = true;
	if(m_networkThread == NULL)
	{
		m_networkThread = new std::thread(&SNetworkController::waitForUsers, this);
	}
}

void SNetworkController::stopNetwork(void)
{
	if(m_networkThread != NULL)
	{
		serverRunning = false;
		m_networkThread->join();
		delete m_networkThread;
	}
}

void SNetworkController::removeDisconnectedClients(void)
{
	m_datalock.lock();

	for(auto it = clientsToRemove.begin(); it != clientsToRemove.end(); it++)
	{
		auto dcClient = std::find(clients.begin(), clients.end(), *it);
		if(dcClient != std::end(clients))
		{
			clients.erase(dcClient);
			if(*dcClient != NULL)
			{
				delete *dcClient;
			}
		}
	}
	clientsToRemove.clear();
	m_datalock.unlock();
}