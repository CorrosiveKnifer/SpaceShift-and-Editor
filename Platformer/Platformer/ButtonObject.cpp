//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: ButtonObject.h
// Description   	: Togglable button within the world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "ButtonObject.h"

//Local Includes
#include "Door.h"
#include "NoiseSystem.h"

//Constructor
ButtonObject::ButtonObject()
	: m_width(0), m_height(0)
	, m_pButtonTop(0)
	, m_isTriggered(false)
{

}

//Destructor
ButtonObject::~ButtonObject() 
{

}

// Initialise( BackBuffer& ,  std::string)
//
// Description:	Sets up the Button' values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	BackBuffer&		Backbuffer to create the sprite from.
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
//
// @return	boolean		Status of the initialisation.
//
bool ButtonObject::Initialise(BackBuffer& buffer, std::string spawnCode)
{
	//Destruct the spawn code:
	int pos0 = spawnCode.find_first_of('p');
	int pos1 = spawnCode.find_first_of('x');
	int pos2 = spawnCode.find_first_of('y');
	std::string x = spawnCode.substr(pos0 + 1, pos1);
	std::string y = spawnCode.substr(pos1 + 1, pos2 - pos1 - 1);
	m_position.x = static_cast<float>(std::atoi(x.c_str()));
	m_position.y = static_cast<float>(std::atoi(y.c_str()) - 20);

	//Create sprites:
	sf::Sprite* source = buffer.CreateSprite("assets/buttonBase.png");
	m_pButtonTop = buffer.CreateSprite("assets/buttonTop.png");
	source->setScale(0.8f, 1);
	m_pButtonTop->setScale(0.8f, 1);

	m_width = static_cast<int>(source->getTexture()->getSize().x * 0.8f);
	m_height = static_cast<int>(source->getTexture()->getSize().y);
	return Entity::Initialise(source);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the button to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool ButtonObject::Deinitialise()
{
	if (m_pButtonTop != 0)
	{
		delete m_pButtonTop;
		m_pButtonTop = 0;
	}
	m_links.clear();
	return Entity::Deinitialise();
}

// Draw( BackBuffer& _backBuffer )
//
// Description:	Draws the button to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the button to.
//
// @return	n/a
//
void ButtonObject::Draw(BackBuffer& buffer)
{
	if (!m_isTriggered) //Up
	{
		buffer.DrawAt(*m_pDrawable, m_position);
		buffer.DrawAt(*m_pButtonTop, m_position);
	}
	else //Down
	{
		sf::Vector2f newPos = m_position;
		newPos.y += (3.0f / 5.0f) * m_height;
		buffer.DrawAt(*m_pButtonTop, newPos);
		buffer.DrawAt(*m_pDrawable, m_position);
	}
}

// Process( float dT )
//
// Description:	Process the current state of the button since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void ButtonObject::Process(float dT)
{
	//Do nothing
}

// ApplyGravity( float _grav )
//
// Description:	Adjusts the velocity of the button based on the gravity provided.
//				
// @param	float	Gravity being applied to the button.
//
// @return	n/a
void ButtonObject::ApplyGravity(float _grav)
{
	//Do nothing
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this button.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool ButtonObject::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	bool cond1 = m_position.x + m_width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > m_position.x;
	bool cond3 = m_position.y + m_height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > m_position.y;

	return cond1 && cond2 && cond3 && cond4;
}

// IsBelowOf(const sf::IntRect&)
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping an invisible hitbox just above this button.
//				Note: No interaction with this button, so it just calculates overlap.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool ButtonObject::IsBelowOf(const sf::IntRect& otherDiment) const
{
	return IsCollidingWith(otherDiment);
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this button.		
//
// @return	sf::IntRect		Hitbox of this box.
const sf::IntRect ButtonObject::GetHitBox() const
{
	return sf::IntRect(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_width, m_height);
}

// GetSize( )
//
// Description:	Returns the width and height of this button.		
//
// @return	sf::Vector2f	Size of this box.
const sf::Vector2f ButtonObject::GetSize() const
{
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}

// AddLink( Door* )
//
// Description:	Add a link from this button to a door, so that
//				once the button is triggered the door is inverted.		
//
// @param	Door*	Door to link to.
//
// @return	n/a.
void ButtonObject::AddLink(Door* _newLink)
{
	m_links.push_back(_newLink);
}

// SetTriggered( bool )
//
// Description:	Sets the current state of the button and the connected doors.
//				Note: A different state creates a sound in the world.
//
// @param	bool	New state of the button.
//
// @return	n/a.
void ButtonObject::SetTriggered(bool _trigger)
{
	if (_trigger) //Button on
	{
		m_pButtonTop->setTextureRect(sf::IntRect(0, 0, static_cast<int>(m_width / 0.8f), static_cast<int>(m_height/3.0f)));
		if (m_isTriggered != _trigger) //different state:
		{
			NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(m_position.x + static_cast<float>(m_width / 2.0f), m_position.y + static_cast<float>(m_height / 2.0f)), 0.7f, 1, "assets/Button.wav", SourceType::WORLD);
		}
	}
	else //Button down
	{
		m_pButtonTop->setTextureRect(sf::IntRect(0, 0, static_cast<int>(m_width / 0.8f), static_cast<int>(m_height)));
		if (m_isTriggered != _trigger) //different state:
		{
			NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(m_position.x + static_cast<float>(m_width / 2.0f), m_position.y + static_cast<float>(m_height / 2.0f)), 0.7f, 1, "assets/Button.wav", SourceType::WORLD);
		}
	}

	m_isTriggered = _trigger;

	//Update all doors:
	for (unsigned int i = 0; i < m_links.size(); i++)
	{
		m_links.at(i)->SetTrigger(_trigger);
	}
}