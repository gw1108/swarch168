// ================================================================================================
// Filename: "TextBox.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: May 4, 2014
// ================================================================================================
// This is the class implementation file for the TextBox class. For a class description see the 
// header file "TextBox.h"
// ================================================================================================

#include "TextBox.h"
#include "MenuSystem.h"

// Initialize Static Constants
const float TextBox::OUTLINE_THICKNESS = 3;

// ===== Constructor ==============================================================================
// The constructor will set-up the TextBox and prepare it to receive data. TextBoxes will be
// built in accordance with their passed parameters.
//
// Input:
//	[IN] const sf::Font &gameFont	- a preloaded Font used to draw character strings
//	[IN] float width				- width of the box
//	[IN] float height				- height of the box
//	[IN] int maxLength				- the maximum characters allowed to be entered into this box
//	[IN] bool hideDisplay			- if true, this box will not display user input 
//	[IN] TextType type				- defines the type of characters that this box will accept
//
// Output: none
//  ===============================================================================================
TextBox::TextBox(const sf::Font& gameFont, float width, float height, int maxLength, bool hideDisplay,
				 InputType inputType) :
	m_gameFont(gameFont),
	m_width(width),
	m_height(height),
	m_maxLength(maxLength),
	m_hidingData(hideDisplay),
	m_inputType(inputType),
	m_backGround(sf::Vector2f(m_width, m_height)),
	m_textDisplay("", m_gameFont, MenuSystem::FONT_SIZE),
	m_isSelected(false),
	m_userData()
{
	// Initialize Background
	m_backGround.setFillColor(sf::Color::Black);
	m_backGround.setOutlineThickness(OUTLINE_THICKNESS);
	m_backGround.setOutlineColor(sf::Color::White);
}

// ===== SetPosition ==============================================================================
// Sets the position of this text box and its' components
//
// Input:
//	[IN] float xCord	- the x-coordinate of this box relative to the window
//	[IN] float yCord	- the y-coordinate of this box relative to the window
//
// Output: none
// ================================================================================================
void TextBox::SetPosition(float xCord, float yCord)
{
	m_backGround.setPosition(sf::Vector2f(xCord, yCord));
	m_textDisplay.setPosition(sf::Vector2f((xCord + 3), (yCord - 2)));
}

// ===== SetSelected ==============================================================================
// Sets the selected boolean and changes the outline color of the box.
//
// Input:
//	[IN] bool state - whether or not the box is now selected
//
// Output: none
// ================================================================================================
void TextBox::SetSelected(bool state)
{
	m_isSelected = state;

	if(m_isSelected)
	{
		m_backGround.setOutlineColor(sf::Color::Yellow);
	}
	else
	{
		m_backGround.setOutlineColor(sf::Color::White);
	}
}

// ===== AddCharacter ==============================================================================
// Method attempts to add the passed char to m_userData. The method will check that the character is
// of the appropriate type allowed in this box and that there is room remaining in the string.
//
// Input:
//	[IN] char state - the char to append to the string
//
// Output:
//	[OUT] bool		- returns false if improper type or string is full; otherwise, true
// ================================================================================================
bool TextBox::AddCharacter(char newChar)
{
	// Check for appropriate type
	if(m_inputType == ALPHABET_ONLY)
	{
		if(!((newChar >= 'a') && (newChar <= 'z')) && 
		   !((newChar >= 'A') && (newChar <= 'Z')))
		{
			// Not Alphabet
			return false;
		}
	}
	else if(m_inputType == NUMERIC_ONLY)
	{
		if(!((newChar >= '0') && (newChar <= '9')))
		{
			// Not Number
			return false;
		}
	}
	else if(m_inputType == ALPHA_NUMERIC)
	{
		if(!((newChar >= 'a') && (newChar <= 'z')) && 
		   !((newChar >= 'A') && (newChar <= 'Z')) &&
		   !((newChar >= '0') && (newChar <= '9')))
		{
			// Not Alpha-Numeric
			return false;
		}
	}

	// Check for full string
	if(m_userData.size() >= (unsigned)m_maxLength)
	{
		return false;
	}

	// Add Character to Data
	m_userData.push_back(newChar);
	return true;
}

// ===== DeleteLast ==============================================================================
// Method attempts to remove the last character from the m_userData string. 
//
// Input: none
//
// Output:
//	[OUT] bool		- returns false if the string is empty; otherwise, true
// ================================================================================================
bool TextBox::DeleteLast(void)
{
	if(!m_userData.empty())
	{
		m_userData.pop_back();
		return true;
	}
	else
	{
		return false;
	}
}

// ===== Render ===================================================================================
// Will render the TextBox's components onto the passed RenderWindow at the specified position.
//
// Input:
//	[IN] const sf::RenderWindow &drawWindow - the rendering window that this box will be drawn on
//
// Output: none
// ================================================================================================
void TextBox::Render(sf::RenderWindow& drawWindow)
{
	if(m_hidingData)
	{
		m_textDisplay.setString(std::string(m_userData.size(), '*'));
	}
	else
	{
		m_textDisplay.setString(m_userData);
	}

	drawWindow.draw(m_backGround);
	drawWindow.draw(m_textDisplay);
}
