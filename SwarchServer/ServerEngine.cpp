#include "ServerEngine.h"

using namespace std;

const unsigned short ServerEngine::DEFAULT_PORT = 53000;

ServerEngine::ServerEngine(void)
{
	db = new Database("Database.sqlite");
	currentPort = DEFAULT_PORT;
	listener.setBlocking(false);
	selector.add(listener);
}

ServerEngine::~ServerEngine(void)
{
	db->close();
	delete db;
	for(sf::TcpSocket* socket : clients)
	{
		delete socket;
	}
}

void ServerEngine::run(void)
{
	serverIsRunning = true;
	
	while(serverIsRunning)
	{
		//listen for people

		//update people
	}
}

void ServerEngine::waitForUsers(void)
{
	//wait forever until game start
	while(true)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
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
			else
			{
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
							//if packet is login mark the client as ready
							//ignore if the client is already logged in
						}
					}
				}
			}
		}
	}
}