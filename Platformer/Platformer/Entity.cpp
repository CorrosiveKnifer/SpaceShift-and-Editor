//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Entity.cpp
// Description   	: Parent class for playable, non-playable or objects within 
//					  the game world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8823@mediadesign.com
//
//Self Include
#include "Entity.h"

//Constructor
Entity::Entity()
	: m_pDrawable(0)
	, m_position(0, 0)
	, m_velocity(0, 0)
{

}

//Destructor
Entity::~Entity()
{
	
}

// Initialise( sf::Drawable* )
//
// Description:	Sets up the Entity's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	sf::Drawable*	main body of the entity which is drawable.
//
// @return	boolean		Status of the initialisation.
//
bool Entity::Initialise(sf::Drawable* _drawable)
{
	m_pDrawable = _drawable;

	return (_drawable != 0);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the entity to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Entity::Deinitialise()
{
	if (m_pDrawable != 0)
	{
		delete m_pDrawable;
		m_pDrawable = 0;
	}
	return true;
}

// GetWorldPosition( )
//
// Description:	Returns the world position of the entity.
//
// @param	n/a
//
// @return	sf::vector2f	world position of the entity.
//
const sf::Vector2f Entity::GetWorldPosition() const
{
	return m_position;
}

// GetVelocity( )
//
// Description:	Returns the velocity of the entity in world space.
//
// @param	n/a
//
// @return	sf::vector2f	velocity of the entity.
//
const sf::Vector2f Entity::GetVelocity() const
{
	return m_velocity;
}

// SetWorldPosition( sf::Vector2f )
//
// Description:	Update the entity and it's sprite's world location.
//
// @param	sf::vector2f	new world position of the entity.
//
// @return	n/a
//
void Entity::SetWorldPosition(sf::Vector2f _pos)
{
	m_position.x = _pos.x;
	m_position.y = _pos.y;
}

// SetWorldPosition( float, float )
//
// Description:	Update the entity and it's sprite's world location.
//
// @param	float	new Position of the entity on the x-axis.
// @param	float	new Position of the entity on the y-axis.
//
// @return	n/a
//
void Entity::SetWorldPosition(float _x, float _y)
{
	SetWorldPosition(sf::Vector2f(_x, _y));
}

// SetVelocity( sf::Vector2f )
//
// Description:	Update the entity's velocity.
//
// @param	 sf::Vector2f	New velocity of the entity.
//
// @return	n/a
//
void Entity::SetVelocity(sf::Vector2f _vel)
{
	m_velocity.x = _vel.x;
	m_velocity.y = _vel.y;
}

// GetCollider( const sf::IntRect& )
//
// Description:	Gets the collision type using Rect vs. Rect collision.
//				Entity version returns exclusivly "NONE".
//
// @param	 sf::IntRect&	Hitbox of the other.
//
// @return	 ColliderType	type of collision.
//
Entity::ColliderType Entity::GetCollider(const sf::IntRect& otherDiment)
{
	return ColliderType::NONE;
}