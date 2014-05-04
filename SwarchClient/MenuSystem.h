// ================================================================================================
// Filename: "MenuSystem.h"
// ================================================================================================
// Author(s): Travis Smith, George Wang
// Last Modified: May 4, 2014
// ================================================================================================
// Class Description:
// 
// The MenuSystem will display the menuscreens to the user. It will be responsible for collecting
// the user name, password, and the IP address of the server from the user. 
// ================================================================================================ 

#pragma once

#include <SFML\Graphics.hpp>
#include "TextBox.h"
#include "CNetworkController.h"

class MenuSystem
{

public:
	
	// Static Constants
	static const int FONT_SIZE;
	static const int MAX_STRING_LENGTH;
	static const int IP_BYTE_STRING_LENGTH;
	static const float LONG_BOX_WIDTH;
	static const float SHORT_BOX_WIDTH;
	static const float BOX_HEIGHT;

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	MenuSystem(sf::RenderWindow& mainWindow, const sf::Font& gameFont, CNetworkController& nwcPtr);

	// Method Prototypes
	void Run(void);

	// Inlined Methods
	std::string GetNameString(void) { return m_nameBox.GetDataString(); }

private:

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Method Prototypes
	void Render(void);
	void NextBox(void);
	void AttemptConnection(void);
	void InitializePositions(void);

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Rendering Members
	sf::RenderWindow &m_mainWindow;
	const sf::Font &m_gameFont;

	sf::Text m_nameLabel;
	sf::Text m_pwLabel;
	sf::Text m_ipLabel;
	sf::Text m_lhLabel;
	sf::Text m_optLabel;
	sf::Text m_msgField;

	TextBox m_nameBox;
	TextBox m_pwBox;
	TextBox m_ipBoxByteOne;
	TextBox m_ipBoxByteTwo;
	TextBox m_ipBoxByteThree;
	TextBox m_ipBoxByteFour;
	TextBox *m_selectedBox;

	// Engine Members
	CNetworkController &m_nwcRef;
	bool m_running;
};

