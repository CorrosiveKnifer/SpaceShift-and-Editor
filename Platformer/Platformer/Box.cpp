//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Box.cpp
// Description   	: Pickup and Droppable object.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "Box.h" 

//Constructor
Box::Box()
	: m_width(0), m_height(0), m_scale(0.85f)
	, m_isGrounded(false)

{

}

//Destructor
Box::~Box()
{

}

// Initialise( BackBuffer& ,  std::string)
//
// Description:	Sets up the Boxes' values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	BackBuffer&		Backbuffer to create the sprite from.
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
//
// @return	boolean		Status of the initialisation.
//
bool Box::Initialise(BackBuffer& buffer, std::string spawnCode)
{
	//Destruct the spawn code:
	int pos0 = spawnCode.find_first_of('x');
	int pos1 = spawnCode.find_first_of('y');
	std::string x = spawnCode.substr(0, pos0);
	std::string y = spawnCode.substr(pos0 + 1, pos1 - pos0 - 1);
	m_position.x = static_cast<float>(std::atoi(x.c_str()));
	m_position.y = static_cast<float>(std::atoi(y.c_str()));
	
	//Create sprite:
	sf::Sprite* source = buffer.CreateSprite("assets/box.png");
	source->setScale(m_scale, m_scale);

	m_width = static_cast<int>(source->getTexture()->getSize().x * m_scale);
	m_height = static_cast<int>(source->getTexture()->getSize().y * m_scale);
	
	return Entity::Initialise(source);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the box to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Box::Deinitialise()
{
	return Entity::Deinitialise();
}

// Draw( BackBuffer& )
//
// Description:	Draws the box to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the box to.
//
// @return	n/a
//
void Box::Draw(BackBuffer& buffer)
{
	buffer.DrawAt(*m_pDrawable, m_position);
}

// Process( float )
//
// Description:	Process the current state of the box since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Box::Process(float dT)
{
	if (!m_isGrounded)
	{
		m_position.y += m_velocity.y * dT;
	}
	
}

// ApplyGravity( float )
//
// Description:	Adjusts the velocity of the box based on the gravity provided.
//				
// @param	float	Gravity being applied to the box.
//
// @return	n/a
void Box::ApplyGravity(float _grav)
{
	if (m_isGrounded)
	{
		m_velocity.y = 0;
	}
	else
	{
		m_velocity.y += _grav;
	}
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this box.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool Box::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	bool cond1 = m_position.x + m_width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > m_position.x;
	bool cond3 = m_position.y + m_height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > m_position.y;

	return cond1 && cond2 && cond3 && cond4;
}

// IsBelowOf( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping an invisible hitbox just above this box.
//				Note: No interaction with this box, so it just calculates overlap.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool Box::IsBelowOf(const sf::IntRect& otherDiment) const
{
	return IsCollidingWith(otherDiment);
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this box		
//
// @return	sf::IntRect		Hitbox of this box.
const sf::IntRect Box::GetHitBox() const
{
	return sf::IntRect(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_width, m_height);
}

// GetSize( )
//
// Description:	Returns the width and height of this box		
//
// @return	sf::Vector2f	Size of this box.
const sf::Vector2f Box::GetSize() const
{
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}

// SetGrounded( bool)
//
// Description:	Updates the grounded state of the box.		
//
// @param	boolean		New grounded state of the box.
//
// @return	n/a.
void Box::SetGrounded(bool _isGrounded)
{
	m_isGrounded = _isGrounded;
}