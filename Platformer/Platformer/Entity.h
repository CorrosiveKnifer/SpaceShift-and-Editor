#ifndef _ENTITY_H_
#define _ENTITY_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Entity.h
// Description   	: Parent class for playable, non-playable or objects within 
//					  the game world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Library Includes
#include <SFML/Graphics.hpp>
#include <cstdlib>

//Local Includes
#include "BackBuffer.h"

//Implementation
class Entity
{
	//Member Functions
public:

	//Platform Collision:
	enum ColliderType { NONE, NORTH, EAST, WEST, SOUTH};

	Entity();
	virtual ~Entity();

	bool Initialise(sf::Drawable* _drawable);
	bool Deinitialise();

	//Inheritance functions:
	virtual void Draw(BackBuffer& buffer) = 0;
	virtual void Process(float dT) = 0;
	virtual void ApplyGravity(float _grav) = 0;
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const = 0;
	virtual ColliderType GetCollider(const sf::IntRect& otherDiment);
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const = 0;
	virtual const sf::IntRect GetHitBox() const = 0;
	virtual const sf::Vector2f GetSize() const = 0;

	const sf::Vector2f GetWorldPosition() const;
	const sf::Vector2f GetVelocity() const;
	
	void SetWorldPosition(float _x, float _y);
	void SetWorldPosition(sf::Vector2f _pos);
	void SetVelocity(sf::Vector2f _vel);

private:

protected:

	//Member Data
public:

private:

protected:
	sf::Drawable* m_pDrawable;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
};
#endif // _ENTITY_H_