// ================================================================================================
// Filename: "ClientStart.cpp"
// ================================================================================================
// Author(s): Travis Smith, George Wang
// Last Modified: May 04, 2014
// ================================================================================================
// This file is where client execution will start. It will get the server IP from the user, attempt
// a connection and if successful, will prepare the client for play.
// ================================================================================================

#include <Windows.h>
#include <string>
#include <ctime>
#include <random>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Clock.hpp>
#include "CNetworkController.h"
#include "ClientEngine.h"
#include "MenuSystem.h"

int CALLBACK WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow)
{
	// Initialize main window and game font
	sf::RenderWindow mainWindow(sf::VideoMode(GameData::BOARD_WIDTH, GameData::BOARD_HEIGHT), "Swarch Client", sf::Style::Close | sf::Style::Titlebar);
	sf::Font gameFont;
	sf::Clock gameClock;
	CNetworkController networkControl(gameClock);

	if(!gameFont.loadFromFile("arial.ttf"))
	{ 
		mainWindow.close();
		return 0;
	}

	// Start Menu System
	MenuSystem menu(mainWindow, gameFont, networkControl);
	menu.Run();
	
	// Menu Exited, Check for active connection
	if(networkControl.IsConnected())
	{
		srand((unsigned int)(time(0)));
//		ClientEngine engine(mainWindow, gameFont, networkControl, menu.GetPlayerNumber(), menu.GetNameString());
		ClientEngine engine(mainWindow, gameFont, 1, menu.GetNameString());
		engine.Run();
	}

	mainWindow.close();
	return 0;
}
