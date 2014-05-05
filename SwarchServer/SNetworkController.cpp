#include "SNetworkController.h"


SNetworkController::SNetworkController(void)
{
	listener.setBlocking(false);
	selector.add(listener);
}


SNetworkController::~SNetworkController(void)
{
	for(sf::TcpSocket* socket : clients)
	{
		delete socket;
	}
}

void SNetworkController::waitForUsers(void)
{
	sf::Time idleTime = sf::seconds(0.5f);

	//wait forever until game start
	while(true)
	{
		// Make the selector wait for data on any socket
		if (selector.wait(idleTime))
		{
			// Test the listener
			if (selector.isReady(listener))
			{
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
					// Error, we won't get a new connection, delete the socket
					delete client;
				}
			}
			// The listener socket is not ready, test all other sockets (the clients)
			for (auto it = clients.begin(); it != clients.end(); ++it)
			{
				sf::TcpSocket& client = **it;
				if (selector.isReady(client))
				{
					// The client has sent some data, we can receive it
					sf::Packet packet;
					if (client.receive(packet) == sf::Socket::Done)
					{
						//if packet is login check login
						//if(logInHandler.login(username, password))
						//then mark the client as ready
					}
				}
			}	
		}
	}
}

void SNetworkController::listenUsers()
{

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