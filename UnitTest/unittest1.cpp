#include "stdafx.h"
#include "CppUnitTest.h"
#include "SNetworkController.h"
#include <codecvt>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestSNetworkControllerEmptyData)
		{
			// TODO: Your test code here
			SNetworkController controller = SNetworkController();
			Assert::AreEqual(false, controller.isGameDataAvailible());
		}

		TEST_METHOD(TestGameDataResetPacket)
		{
			GameData data = GameData();

			srand(time(NULL));

			data.reset = rand() % 2;

			sf::Packet packet;
			GameData otherGameData = data;

			packet << data;
			packet >> data;
			Assert::AreEqual(otherGameData.reset, data.reset);
		}

		TEST_METHOD(TestGamePacketPlayers)
		{
			Player player;
			GameData data = GameData();
			srand(time(NULL));
			
			for(int i = 0; i < GameData::MAX_PLAYERS; i++)
			{
				player.SetPosition(rand() % 100, rand() % 100);
				player.SetActive(rand() % 2);
				player.SetDead(rand() % 2);
				player.SetDimension(rand() % 100);
				player.SetDirection(static_cast<Player::Direction>(rand() % 4));
				player.SetMoveRate(rand() % 100);
				player.SetPlayerNumber(rand() % 4);
				
				std::stringstream ss;
				ss << rand() % 50;

				player.SetUsername(ss.str());
				data.m_players[i] = player;
			}

			GameData otherData = data;
			sf::Packet packet;
			packet << data;
			packet >> data;

			for(int i = 0; i < GameData::MAX_PLAYERS; i++)
			{
				Assert::AreEqual(otherData.m_players[i].GetPosition().x, data.m_players[i].GetPosition().x);
			}
		}

		TEST_METHOD(TestPacketPellets)
		{
			Pellet pellet;
			GameData data = GameData();
			srand(time(NULL));

			for(int i = 0; i < GameData::MAX_PELLETS; i++)
			{
				pellet.setPosition(rand() % 100, rand() % 100);
				data.m_pellets[i] = pellet;
			}

			GameData otherGameData = data;
			sf::Packet packet;
			packet << data;
			packet >> data;
		}
	};
}