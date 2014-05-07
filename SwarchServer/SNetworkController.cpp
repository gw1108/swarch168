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

void SNetworkController::run(void)
{
	std::cout << "Start waiting for connections"<< std::endl;

	sf::TcpSocket* newClient = new sf::TcpSocket();
	newClient->setBlocking(false);
	while(serverRunning)
	{
		//accept new connections
		newConnections(newClient);

		//update old connections
		updateConnections();
	}
	std::cout << "stopped waiting for connections" << std::endl;
}

void SNetworkController::newConnections(sf::TcpSocket* socket)
{
	sf::Socket::Status listenerStatus = listener.accept(*socket);
	if(listenerStatus == sf::Socket::Done)
	{
		clients.push_back(socket);
		std::cout << "New client accepted" << std::endl;
		socket = new sf::TcpSocket();
		socket->setBlocking(false);
	}
	else if(listenerStatus == sf::Socket::NotReady)
	{
	}
	else if(listenerStatus == sf::Socket::Error)
	{
		std::cout << "Error" << std::endl;
	}
	else if(listenerStatus == sf::Socket::Disconnected)
	{
		std::cout << "Listener disconnected" << std::endl;
	}
}

void SNetworkController::updateConnections(void)
{
	auto it = clients.begin();
	while(it != clients.end())
	{
		sf::TcpSocket& client = **it;
		sf::Packet packet;
		std::cout << "receiving packet " << std::endl;
		sf::Socket::Status socketStatus = client.receive(packet);

		std::cout << "The socket status is : " + socketStatus << std::endl;

		if(socketStatus == sf::TcpSocket::Done)
		{
			std::cout << "Socket done accepting." << std::endl;
			//if packet is login check login
			sf::Uint8 commandCode;
			packet >> commandCode;
			if(commandCode == GameData::LOG_IN)
			{
				std::cout << "command code is login" << std::endl;
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

				it++;		//go to next client
			}
			else
			{
				std::cout << "command is not log in" << std::endl;
				it++;
			}
		}
		else if(socketStatus == sf::TcpSocket::Disconnected)
		{
			//remove
			std::cout <<" remove the socket plz" << std::endl;
			it = clients.erase(it);
		}
	}
	std::cout << "Iterated through all the clients." << std::endl;
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
		m_networkThread = new std::thread(&SNetworkController::run, this);
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