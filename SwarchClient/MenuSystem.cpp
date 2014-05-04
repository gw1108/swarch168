// ================================================================================================
// Filename: "MenuSystem.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 4, 2014
// ================================================================================================
// This is the class implementation file for the MenuSystem class. For a class description see the 
// header file "MenuSystem.h"
// ================================================================================================

#include "MenuSystem.h"

// Initialize Static Constants
const int MenuSystem::FONT_SIZE = 18;
const int MenuSystem::MAX_STRING_LENGTH = 30;
const int MenuSystem::IP_BYTE_STRING_LENGTH = 3;
const float MenuSystem::LONG_BOX_WIDTH = 550;
const float MenuSystem::SHORT_BOX_WIDTH = 40;
const float MenuSystem::BOX_HEIGHT = 20;

// ===== Constructor ==============================================================================
// The constructor will initialize all data members and prepare the menu system to receive user
// input.
//
// Input:
//	[IN] const sf::RenderWindwow& mainWindow	- the window that the menu will be displayed in
//	[IN] const sf::Font &gameFont				- a preloaded Font used to draw character strings
//
// Output: none
//  ===============================================================================================
MenuSystem::MenuSystem(sf::RenderWindow& mainWindow, const sf::Font& gameFont, CNetworkController& nwcRef) :
	m_mainWindow(mainWindow),
	m_gameFont(gameFont),
	m_nwcRef(nwcRef),
	m_nameLabel("User Name:", m_gameFont, FONT_SIZE),
	m_pwLabel("Password:", m_gameFont, FONT_SIZE),
	m_ipLabel("Server IP:", m_gameFont, FONT_SIZE),
	m_lhLabel("Local Host is: 127.0.0.1", m_gameFont, FONT_SIZE),
	m_optLabel("Press \"Return\" to attempt connection or \"ESC\" to quit.", m_gameFont, FONT_SIZE),
	m_msgField("", m_gameFont, FONT_SIZE),
	m_nameBox(gameFont, LONG_BOX_WIDTH, BOX_HEIGHT, MAX_STRING_LENGTH, false, TextBox::ALPHA_NUMERIC),
	m_pwBox(gameFont, LONG_BOX_WIDTH, BOX_HEIGHT, MAX_STRING_LENGTH, true, TextBox::ALPHA_NUMERIC),
	m_ipBoxByteOne(gameFont, SHORT_BOX_WIDTH, BOX_HEIGHT, IP_BYTE_STRING_LENGTH, false, TextBox::NUMERIC_ONLY),
	m_ipBoxByteTwo(gameFont, SHORT_BOX_WIDTH, BOX_HEIGHT, IP_BYTE_STRING_LENGTH, false, TextBox::NUMERIC_ONLY),
	m_ipBoxByteThree(gameFont, SHORT_BOX_WIDTH, BOX_HEIGHT, IP_BYTE_STRING_LENGTH, false, TextBox::NUMERIC_ONLY),
	m_ipBoxByteFour(gameFont, SHORT_BOX_WIDTH, BOX_HEIGHT, IP_BYTE_STRING_LENGTH, false, TextBox::NUMERIC_ONLY),
	m_selectedBox(&m_nameBox),
	m_running(false)
{
	InitializePositions();
	m_msgField.setColor(sf::Color::Red);
	m_selectedBox->SetSelected(true);
}

// ===== Run ======================================================================================
// Will display the main menu to the user until the user submits acceptable input and is able to
// connect to the server.
//
// Input: none
// Output: none
// ================================================================================================
void MenuSystem::Run(void)
{
	m_running = true;

	while(m_running)
	{
			// Loop through events and get character input
			sf::Event event;
			while(m_mainWindow.pollEvent(event))
			{
				if((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
				{
					m_running = false;
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return))
				{
					AttemptConnection();

					// Clear Any input that was entered during connection attempt
					while(m_mainWindow.pollEvent(event)){}
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Tab))
				{
					NextBox();
				}
				else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::BackSpace))
				{
					m_selectedBox->DeleteLast();
				}
				else if(event.type == sf::Event::TextEntered)
				{
					char input = event.text.unicode;
					m_selectedBox->AddCharacter(input);
				}
			} 
			
			Render();
		}
	}

// ===== Render ===================================================================================
// Method will render the main menu window.
//
// Input: none
// Output: none
// ================================================================================================
void MenuSystem::Render(void)
{
	// Clear Screen
	m_mainWindow.clear(sf::Color(0, 0, 75));

	// Draw Name
	m_mainWindow.draw(m_nameLabel);
	m_nameBox.Render(m_mainWindow);

	// Draw PW
	m_mainWindow.draw(m_pwLabel);
	m_pwBox.Render(m_mainWindow);

	// Draw IP
	m_mainWindow.draw(m_ipLabel);
	m_ipBoxByteOne.Render(m_mainWindow);
	m_ipBoxByteTwo.Render(m_mainWindow);
	m_ipBoxByteThree.Render(m_mainWindow);
	m_ipBoxByteFour.Render(m_mainWindow);
	m_mainWindow.draw(m_lhLabel);
	m_mainWindow.draw(m_optLabel);

	// Center Message Field
	float msgXCoord = ((m_mainWindow.getSize().x / 2) - (m_msgField.getGlobalBounds().width / 2));
	float msgYCoord = ((m_mainWindow.getSize().y / 2) - (m_msgField.getGlobalBounds().height / 2));
	m_msgField.setPosition(msgXCoord, msgYCoord);
	m_mainWindow.draw(m_msgField);

	// Display Screen
	m_mainWindow.display();
}

// ===== NextBox ==================================================================================
// Will adjust the selected box point to point to the "next" TextBox. It will first deselect the 
// current box and then select the next. 
//
// Input: none
// Output: none
// ================================================================================================
void MenuSystem::NextBox(void)
{
	m_selectedBox->SetSelected(false);

	if(m_selectedBox == &m_nameBox)
	{
		m_selectedBox = &m_pwBox;
	}
	else if(m_selectedBox == &m_pwBox)
	{
		m_selectedBox = &m_ipBoxByteOne;
	}
	else if(m_selectedBox == &m_ipBoxByteOne)
	{
		m_selectedBox = &m_ipBoxByteTwo;
	}
	else if(m_selectedBox == &m_ipBoxByteTwo)
	{
		m_selectedBox = &m_ipBoxByteThree;
	}
	else if(m_selectedBox == &m_ipBoxByteThree)
	{
		m_selectedBox = &m_ipBoxByteFour;
	}
	else if(m_selectedBox == &m_ipBoxByteFour)
	{
		m_selectedBox = &m_nameBox;
	}

	m_selectedBox->SetSelected(true);
}

// ===== AttemptConnection ========================================================================
// This method will have two parts. First it will attempt to establish a TCP connection with the
// server using the IP address supplied by the user. If the connection is successful, it will send
// the username and password to the server. 
//
// Input: none
// Output: none
// ================================================================================================
void MenuSystem::AttemptConnection(void)
{
	// Get Name and PW
	std::string userName = m_nameBox.GetDataString();
	std::string psswrd = m_pwBox.GetDataString();

	if(userName.length() == 0)
	{
		m_msgField.setString("Please Enter User Name");
		return;
	}

	if(psswrd.length() == 0)
	{
		m_msgField.setString("Please Enter Password");
		return;
	}

	// Get Byte Strings
	std::string firstByte = m_ipBoxByteOne.GetDataString();
	std::string secondByte = m_ipBoxByteTwo.GetDataString();
	std::string thirdByte = m_ipBoxByteThree.GetDataString();
	std::string fourthByte = m_ipBoxByteFour.GetDataString();

	// Check if IP is valid
	int firstB = 0;
	int secondB = 0;
	int thirdB = 0;
	int fourthB = 0;

	sscanf_s(firstByte.c_str(), "%d", &firstB, 3);
	sscanf_s(secondByte.c_str(), "%d", &secondB, 3);
	sscanf_s(thirdByte.c_str(), "%d", &thirdB, 3);
	sscanf_s(fourthByte.c_str(), "%d", &fourthB, 3);

	if((firstByte.length() == 0) || ((firstB < 0) || (firstB > 255)))
	{
		m_msgField.setString("First Byte out of Range");
		return;
	}

	if((secondByte.length() == 0) || ((secondB < 0) || (secondB > 255)))
	{
		m_msgField.setString("Second Byte out of Range");
		return;
	}

	if((thirdByte.length() == 0) || ((thirdB < 0) || (thirdB > 255)))
	{
		m_msgField.setString("Third Byte out of Range");
		return;
	}

	if((fourthByte.length() == 0) || ((fourthB < 0) || (fourthB > 255)))
	{
		m_msgField.setString("Fourth Byte out of Range");
		return;
	}

	std::string ipString = (firstByte + '.' + secondByte + '.' + thirdByte + '.' + fourthByte);

	m_msgField.setString("Data Accepted, Attempting Connection @ " + ipString);
	Render();

	// Attempt Connection
	if(m_nwcRef.Connect(ipString, CNetworkController::SERVER_PORT))
	{
		m_running = false;
	}
	else
	{
		m_msgField.setString("Connection Failed");
	}
}

// ===== InitializePositions ======================================================================
// Sets the positions of all drawable components within the main window. These positions will be
// fixed and hardcoded within this method, any layout changes should be made here.
//
// Input: none
// Output: none
// ================================================================================================
void MenuSystem::InitializePositions(void)
{
	float horizBuffer = 10;
	float vertBuffer = 30;
	float textBoxXStart = 120;

	// Name
	m_nameLabel.setPosition(horizBuffer, vertBuffer);
	m_nameBox.SetPosition(textBoxXStart, vertBuffer);
	// Password
	float pwYCord = (m_nameLabel.getGlobalBounds().top + m_nameLabel.getGlobalBounds().height + vertBuffer);

	m_pwLabel.setPosition(horizBuffer, pwYCord);
	m_pwBox.SetPosition(textBoxXStart, pwYCord);

	// IP Address
	float ipYCoord = (m_pwLabel.getGlobalBounds().top + m_pwLabel.getGlobalBounds().height + vertBuffer);

	m_ipLabel.setPosition(horizBuffer, ipYCoord);
	m_ipBoxByteOne.SetPosition(textBoxXStart, ipYCoord);
	m_ipBoxByteTwo.SetPosition((m_ipBoxByteOne.GetGlobalBounds().left + m_ipBoxByteOne.GetGlobalBounds().width + horizBuffer), ipYCoord);
	m_ipBoxByteThree.SetPosition((m_ipBoxByteTwo.GetGlobalBounds().left + m_ipBoxByteOne.GetGlobalBounds().width + horizBuffer), ipYCoord);
	m_ipBoxByteFour.SetPosition((m_ipBoxByteThree.GetGlobalBounds().left + m_ipBoxByteOne.GetGlobalBounds().width + horizBuffer), ipYCoord);
	m_lhLabel.setPosition((m_ipBoxByteFour.GetGlobalBounds().left + m_ipBoxByteFour.GetGlobalBounds().width + horizBuffer), ipYCoord);

	// Menu Options
	float optionYCoord = (m_ipLabel.getGlobalBounds().top + m_ipLabel.getGlobalBounds().height + vertBuffer);
	m_optLabel.setPosition(horizBuffer, optionYCoord);
}

