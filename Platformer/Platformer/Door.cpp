//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Door.cpp
// Description   	: Door within the world which blocks the player.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "Door.h"

//Local Includes
#include "NoiseSystem.h"

//Constructor
Door::Door()
	: m_isTriggered(false) , m_baseState(false), m_currState(false)
{

}

//Destructor
Door::~Door()
{

}

// Initialise( BackBuffer& ,  std::string)
//
// Description:	Sets up the Boxes' values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
//
// @return	boolean		Status of the initialisation.
//
bool Door::Initialise(std::string platformCode)
{
	bool cond = Platform::Initialise(platformCode);
	m_render->setFillColor(sf::Color(139, 69, 19, 255));
	return cond;
}

// Deinitialise( )
//
// Description:	Clears the memory used by the entity to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Door::Deinitialise()
{
	return Platform::Deinitialise();
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this platform.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool Door::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	if (m_isTriggered)
	{
		return false;
	}

	return Platform::IsCollidingWith(otherDiment);
}

// IsBelowOf( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping an invisible hitbox just above this platform.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool Door::IsBelowOf(const sf::IntRect& otherDiment) const
{
	if (m_isTriggered)
	{
		return false;
	}
	
	return Platform::IsBelowOf(otherDiment);
}

// SetTrigger( boolean )
//
// Description:	updates the state of the door, creating a sound in the world.
//				
// @param	boolean		new state of the door.
//
// @return	n/a
void Door::SetTrigger(bool _trigger)
{
	if (_trigger) //Door open
	{
		m_currState = !m_baseState;
		if (m_isTriggered != _trigger)  //Different state
		{
			NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(m_position.x + static_cast<float>(m_width / 2.0f), m_position.y + static_cast<float>(m_height / 2.0f)), 1, 0.9f, "assets/Door.wav", SourceType::WORLD);
		}
	}
	else //Door Closed
	{
		m_currState = m_baseState;
		if (m_isTriggered != _trigger) //Different state
		{
			NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(m_position.x + static_cast<float>(m_width / 2.0f), m_position.y + static_cast<float>(m_height / 2.0f)), 0.7f, 1, "assets/Door.wav", SourceType::WORLD);
		}
	}

	//New color based on state: 
	if (m_currState)
	{
		m_render->setFillColor(sf::Color(139, 69, 19, 128));
	}
	else
	{
		m_render->setFillColor(sf::Color(139, 69, 19, 255));
	}

	m_isTriggered = _trigger;
}

// GetTrigger( )
//
// Description:	Get the current state of the door.
//
// @return	boolean		the state of the door.
bool Door::GetTrigger()
{
	return m_isTriggered;
}

// SetOpen( )
//
// Description:	Update the base state of the door.
//
// @param	boolean		the new base state of the door.
//
// @return n/a
void Door::SetOpen(bool _isOpen)
{
	m_baseState = _isOpen;
}

// IsOpen( )
//
// Description:	returns the current state of the door.
//
// @return	boolean		the new base state of the door.
bool Door::IsOpen()
{
	return m_currState;
}