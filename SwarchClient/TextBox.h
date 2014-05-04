// ================================================================================================
// Filename: "TextBox.h"
// ================================================================================================
// Author(s): Travis Smith, George Wang
// Last Modified: May 4, 2014
// ================================================================================================
// Class Description:
// 
// The TextBox will be used to get data from the user. It will limit the total input and can be
// customized to only accept letters or numbers or both. 
// ================================================================================================ 

#pragma once

#include <SFML\Graphics.hpp>

class TextBox
{

public:

	enum InputType {ALPHABET_ONLY, NUMERIC_ONLY, ALPHA_NUMERIC};

	// ============================================================================================
	// Methods
	// ============================================================================================

	// Constructor Prototype
	TextBox(const sf::Font& gameFont, float width, float height, int maxLength, bool hideDisplay, InputType type);

	// Method Prototypes
	void SetPosition(float xCord, float yCord);
	void SetSelected(bool state);
	bool AddCharacter(char newChar);
	bool DeleteLast(void);
	void Render(sf::RenderWindow& drawWindow);

	// Inlined Methods
	std::string GetDataString(void) { return m_userData; }
	sf::FloatRect GetGlobalBounds(void) { return m_backGround.getGlobalBounds(); }

private:

	// ============================================================================================
	// Class Data Members
	// ============================================================================================

	// Static Constants
	static const float OUTLINE_THICKNESS;

	// Rendering Members
	const sf::Font &m_gameFont;
	float m_width;
	float m_height;
	bool m_hidingData;
	InputType m_inputType;

	sf::RectangleShape m_backGround;
	sf::Text m_textDisplay;
	bool m_isSelected;

	// User Data Members
	std::string m_userData;
	int m_maxLength;
};

