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
				//Check Position
				Assert::AreEqual(otherData.m_players[i].GetPosition().x, data.m_players[i].GetPosition().x);
				Assert::AreEqual(otherData.m_players[i].GetPosition().y, data.m_players[i].GetPosition().y);

				//check direction
				Assert::AreEqual((int)otherData.m_players[i].GetDirection(), (int)data.m_players[i].GetDirection());

				Assert::AreEqual(otherData.m_players[i].GetMoveRate() , data.m_players[i].GetMoveRate());

				Assert::AreEqual(otherData.m_players[i].IsActive(), data.m_players[i].IsActive());

				Assert::AreEqual(otherData.m_players[i].IsDead(), data.m_players[i].IsDead());

				Assert::AreEqual(otherData.m_players[i].GetDimension(), data.m_players[i].GetDimension());

				Assert::AreEqual(otherData.m_players[i].GetUsername(), data.m_players[i].GetUsername());

				//check assigned number
				Assert::AreEqual(otherData.m_players[i].GetAssignedNumber(), data.m_players[i].GetAssignedNumber());

				//Check bounding rectangles
				Assert::AreEqual(otherData.m_players[i].GetBoundingRectangle().left, data.m_players[i].GetBoundingRectangle().left);
				Assert::AreEqual(otherData.m_players[i].GetBoundingRectangle().top, data.m_players[i].GetBoundingRectangle().top);
				Assert::AreEqual(otherData.m_players[i].GetBoundingRectangle().height, data.m_players[i].GetBoundingRectangle().height);
				Assert::AreEqual(otherData.m_players[i].GetBoundingRectangle().width, data.m_players[i].GetBoundingRectangle().width);
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

			//assert pellets are same
			for(int i = 0; i < GameData::MAX_PELLETS; ++i)
			{
				Assert::AreEqual(otherGameData.m_pellets[i].getPosition().x, data.m_pellets[i].getPosition().x);
				Assert::AreEqual(otherGameData.m_pellets[i].getPosition().y, data.m_pellets[i].getPosition().y);
			}
		}

		TEST_METHOD(TestPlayerDrawing)
		{
			Player player;
			player = Player();

			Assert::AreEqual(player.GetBoundingRectangle().left, player.GetPiece().getGlobalBounds().left);
			Assert::AreEqual(player.GetBoundingRectangle().top, player.GetPiece().getGlobalBounds().top);
			Assert::AreEqual(player.GetBoundingRectangle().height, player.GetPiece().getGlobalBounds().height);
			Assert::AreEqual(player.GetBoundingRectangle().width, player.GetPiece().getGlobalBounds().width);
		}
	};
}