//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Bullet.cpp
// Description   	: Bullet created by guards.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
#include "Bullet.h"

//Constructor
Bullet::Bullet()
	: m_speed(800)
	, m_dirVector(0, 0), m_origin(0, 0)
{

}

//Destructor
Bullet::~Bullet()
{

}

// Initialise( sf::Vector2f ,  sf::Vector2f)
//
// Description:	Sets up the Bullet's values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	sf::Vector2f	Directional vector of the bullet.
// @param	sf::Vector2f	Original point of the bullet. 
//
// @return	boolean		Status of the initialisation.
//
bool Bullet::Initialise(sf::Vector2f dirVect, sf::Vector2f origin)
{
	m_dirVector = dirVect;
	m_origin = origin;
	m_bullet.setPosition(origin);
	m_bullet.setSize(sf::Vector2f(5, 2));
	m_streak.setPosition(origin);
	m_streak.setSize(sf::Vector2f(20, 2));

	m_bullet.setFillColor(sf::Color(198, 144, 25));
	m_streak.setFillColor(sf::Color(238, 232, 170, 105));

	m_bullet.setRotation(std::atan(dirVect.y / dirVect.x));
	m_streak.setRotation(std::atan(dirVect.y / dirVect.x));
	return true;
}

// Deinitialise( )
//
// Description:	Clears the memory used by the entity to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Bullet::Deinitialise()
{
	return true;
}

// Draw( BackBuffer& )
//
// Description:	Draws the widget to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the widget to.
//
// @return	n/a
//
void Bullet::Draw(BackBuffer& buffer)
{
	buffer.Draw(m_streak);
	buffer.Draw(m_bullet);
}

// Process( float )
//
// Description:	Process the current state of the box since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void Bullet::Process(float dT)
{
	sf::Vector2f pos = m_bullet.getPosition();
	pos.x += m_speed * m_dirVector.x * dT;
	pos.y += m_speed * m_dirVector.y * dT;
	m_bullet.setPosition(pos);
	m_streak.setPosition(pos);
	sf::Vector2f size = m_streak.getSize();

	if (std::abs(size.x) < 600.0f)
		size.x += m_speed * m_dirVector.x * dT;
	if (std::abs(size.y) < 600.0f)
		size.y += m_speed * m_dirVector.y * dT;
	
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this bullet.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool Bullet::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	sf::Vector2f pos = m_bullet.getPosition();
	sf::Vector2f size = m_bullet.getSize();

	bool cond1 = pos.x + size.x > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > pos.x;
	bool cond3 = pos.y + size.y > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > pos.y;

	return cond1 && cond2 && cond3 && cond4;
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this bullet		
//
// @return	sf::IntRect		Hitbox of this bullet.
//
sf::IntRect Bullet::GetHitBox()
{
	sf::Vector2f pos = m_bullet.getPosition();
	sf::Vector2f size = m_bullet.getSize();
	return sf::IntRect(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y));
}