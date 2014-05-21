#include "SNetworkController.h"
#include "LogInData.h"
#include <iostream>			//network controller debug
#include <vector>
#include <ctime>

using namespace std;

SNetworkController::SNetworkController()
	:logInHandler(new LogIn()),
	m_networkThread(nullptr),
	serverRunning(false),
	listener(),
	clients(),
	m_data(),
	m_datalock(),
	currentPlayerID(1),
	availableSpots()
{
	listener.setBlocking(false);
	listener.listen(GameData::SERVER_PORT);

	for(int i = 0; i < GameData::MAX_PLAYERS; ++i)
	{
		availableSpots.push_back(true);
	}
}


SNetworkController::~SNetworkController(void)
{
	for(auto client : clients)
	{
		client.first->disconnect();
		delete client.first;
	}
	delete logInHandler;
	stopNetwork();
}

void SNetworkController::run(void)
{
	std::cout << "Start waiting for connections"<< std::endl;

	sf::TcpSocket* player = new sf::TcpSocket();
	player->setBlocking(false);
	while(serverRunning)
	{
		//accept new connections
		newConnections(player);

		//update old connections
		updateConnections();

		std::this_thread::sleep_for(std::chrono::milliseconds(GameData::S_UPDATE_SPEED));
	}
	delete player;
	std::cout << "stopped waiting for connections" << std::endl;
}

void SNetworkController::newConnections(sf::TcpSocket*& player)
{
	if(clients.size() < GameData::MAX_PLAYERS)//only listen for new connections if the server has space
	{
		sf::Socket::Status listenerStatus = listener.accept(*player);
		if(listenerStatus == sf::Socket::Done)
		{
			clients.push_back(std::make_pair(std::ref(player), SPlayer()));
			std::cout << "New client accepted" << std::endl;
			player = new sf::TcpSocket();
			player->setBlocking(false);
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
}

void SNetworkController::updateConnections(void)
{
	auto it = clients.begin();
	while(it != clients.end())
	{
		sf::TcpSocket& client = *(*it).first;
		sf::Packet packet;
		sf::Socket::Status socketStatus = client.receive(packet);

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
				sf::Uint8 clientNumber = (sf::Uint8)  getNewClientNumber();
				packet << newCode;
				packet << serverResponseCode;
				packet << clientNumber;		//todo next number
				cout << "Sending packet new player number is " << (int)clientNumber << endl;
				std::cout << client.send(packet) << std::endl;

				if(responseCode == GameData::LoginResponse::ACCEPTED)
				{
					//Player newPlayer(logInData.GetUsername(), (int)clientNumber);
					//sendNewPlayer(newPlayer);
				}

				(*it).second.m_loggedIn = ((responseCode == GameData::LoginResponse::ACCEPTED) ||
										   (responseCode == GameData::LoginResponse::NEW_ACCOUNT));
				(*it).second.m_playerNumber = (int) clientNumber;
				
				//add new player to game data
				if((*it).second.m_loggedIn)
				{
					ServerData data((*it).second.m_playerNumber, ServerData::LOG_IN);
					data.m_playerName = logInData.GetUsername();
					pushServerData(data);
				}
			}
			else if(commandCode == GameData::CommandCode::PLAYER_UPDATE)	//update the m_data queue
			{
				sf::Int32 timestamp;
				sf::Uint8 sfDirection;

				packet >> timestamp;
				packet >> sfDirection;

				ServerData serverData((*it).second.m_playerNumber, ServerData::PLAYER_UPDATE);
				serverData.direction = sfDirection;
				pushServerData(serverData);
			}
			else
			{
				std::cout << "command is code is : " << commandCode << std::endl;
			}
			it++;		//go to next client
		}
		else if(socketStatus == sf::TcpSocket::Disconnected)
		{
			//remove
			std::cout <<" remove the socket and player plz" << std::endl;
			if((*it).second.m_loggedIn)
			{
				ServerData playerDisconnect((*it).second.m_playerNumber, ServerData::ServerCommands::LOG_OUT);
				pushServerData(playerDisconnect);
			}
			client.disconnect();
			delete (*it).first;
			freeClientNumber((*it).second.m_playerNumber);
			it = clients.erase(it);
			it = clients.end();
		}
		else
		{
			it++;
		}
	}
//	std::cout << "Iterated through all the clients." << std::endl;
}

ServerData SNetworkController::getNextGameData(void)
{
	m_datalock.lock();

	ServerData data = m_data.back();
	m_data.pop_back();

	m_datalock.unlock();
	return data;
}

void SNetworkController::pushServerData(ServerData data)
{
	m_datalock.lock();

	m_data.push_back(data);

	m_datalock.unlock();
}

bool SNetworkController::isGameDataAvailible(void)
{
	return m_data.size() != 0;
}

//sends data to all players
void SNetworkController::sendGameUpdate(GameData data)
{
	for(auto it = clients.begin(); it != clients.end(); ++it)
	{
		if((*it).second.m_loggedIn)
		{
			sf::Packet packet;

			sf::Uint8 code = GameData::GAME_UPDATE;
			packet << code << data;
			(*it).first->send(packet);
		}
	}
}

void SNetworkController::startNetwork(void)
{
	serverRunning = true;
	if(m_networkThread == nullptr)
	{
		m_networkThread = new std::thread(&SNetworkController::run, this);
	}
}

void SNetworkController::stopNetwork(void)
{
	if(m_networkThread != nullptr)
	{
		serverRunning = false;
		m_networkThread->join();
		delete m_networkThread;
		m_networkThread = nullptr;
	}
}

int SNetworkController::getNewClientNumber()
{
	for(int i = 0; i < availableSpots.size(); i++)
	{
		if(availableSpots[i])
		{
			availableSpots[i] = false;
			return i;
		}
	}
	return 0;
}

void SNetworkController::freeClientNumber(int clientNumber)
{
	if(clientNumber > GameData::MAX_PLAYERS)
	{
		cout << "Cannot free a player with number : " << clientNumber << endl;
	}
	else
	{
		availableSpots[clientNumber] = true;
	}
}

void SNetworkController::sendNewPlayer(Player newPlayer)
{
	for(auto it = clients.begin(); it != clients.end(); ++it)
	{
		if((*it).second.m_loggedIn)
		{
			sf::Packet packet;

			sf::Uint8 commandCode = GameData::NEW_PLAYER;

			packet << newPlayer;

			(*it).first->send(packet);
		}
	}
}