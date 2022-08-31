//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : TextObject.cpp
// Description   : Define the text object within the world.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Self Include
#include "TextObject.h"

//Constructor
TextObject::TextObject()
	: m_text(""), m_size(0)
{

}

//Destructor
TextObject::~TextObject()
{

}

// Initialise( std::string , std::string , int )
//
// Description:	Sets up the Texts' values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	std::string		String of characters for the text.
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
// @param	int		Font size of the text.
//
// @return	boolean		Status of the initialisation.
//
bool TextObject::Initialise(std::string _text, std::string _loc, int size)
{
	//Extract Text
	if (_text.at(0) == '\"')
	{
		m_text = _text.substr(1, _text.length() - 2);
	}
	else
	{
		m_text = _text;
	}

	//Deconstruct spawn location
	int pos0 = _loc.find_first_of('x');
	int pos1 = _loc.find_first_of('y');
	std::string x = _loc.substr(0, pos0);
	std::string y = _loc.substr(pos0 + 1, pos1 - pos0 - 1);
	m_position.x = static_cast<float>(std::atoi(x.c_str()));
	m_position.y = static_cast<float>(std::atoi(y.c_str()));

	m_size = size;

	return Entity::Initialise(new sf::RectangleShape());
}

// Initialise( std::string , std::string , int )
//
// Description:	Sets up the Texts' values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	std::string		String of characters for the text.
// @param	sf::Vector2f	The starting position.
// @param	int		Font size of the text.
//
// @return	boolean		Status of the initialisation.
//
bool TextObject::Initialise(std::string _text, sf::Vector2f _loc, int size)
{
	//Extract Text
	if (_text.at(0) == '\"')
	{
		m_text = _text.substr(1, _text.length() - 2);
	}
	else
	{
		m_text = _text;
	}

	m_position.x = _loc.x;
	m_position.y = _loc.y;

	m_size = size;

	return Entity::Initialise(new sf::RectangleShape());
}

// Deinitialise( )
//
// Description:	Clears the memory used by the text to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool TextObject::Deinitialise()
{
	return Entity::Deinitialise();
}

// Draw( BackBuffer& )
//
// Description:	Draws the text to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the text to.
//
// @return	n/a
//
void TextObject::Draw(BackBuffer& buffer)
{
	buffer.SetFontAlign(Align::Left);
	buffer.SetColour(sf::Color::White);
	buffer.SetFontSize(m_size);
	buffer.DrawTextToWorld(m_text, m_position.x, m_position.y);
}

// Process( float )
//
// Description:	Process the current state of the text since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void TextObject::Process(float dT)
{
	//Do nothing
}

// ApplyGravity( float _grav )
//
// Description:	Adjusts the velocity of the text based on the gravity provided.
//				
// @param	float	Gravity being applied to the text.
//
// @return	n/a
//
void TextObject::ApplyGravity(float _grav)
{
	//Do nothing
}

// IsCollidingWith(const sf::IntRect&)
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this text.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool TextObject::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	return false; //No collision
}

// IsBelowOf( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping an invisible hitbox just above this text.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool TextObject::IsBelowOf(const sf::IntRect& otherDiment) const
{
	return false; //No collision
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this box		
//
// @return	sf::IntRect		Hitbox of this box.
//
const sf::IntRect TextObject::GetHitBox() const
{
	return sf::IntRect(0, 0, 0, 0);
}

// GetSize( )
//
// Description:	Returns the width and height of this box		
//
// @return	sf::Vector2f	Size of this box.
//
const sf::Vector2f TextObject::GetSize() const
{
	return sf::Vector2f(0, 0);
}