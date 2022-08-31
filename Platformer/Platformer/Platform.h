#ifndef _PLATFORM_H_
#define _PLATFORM_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Playform.h
// Description   	: A physical platform for the player to stand on.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Includes
#include "Entity.h"

//Library Includes
#include <string>

//Implementation
class Platform : public Entity
{
	//Member functions
public:
	Platform();
	virtual ~Platform();

	bool Initialise(std::string platformCode);
	bool Initialise(sf::IntRect _info);

	bool Deinitialise();

	virtual void Draw(BackBuffer& buffer);
	void DrawHitboxes(BackBuffer& buffer);
	virtual void Process(float dT);
	virtual void ApplyGravity(float _grav);
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual ColliderType GetCollider(const sf::IntRect& otherDiment);
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;
	virtual const sf::IntRect GetHitBox() const;
	virtual const sf::Vector2f GetSize() const ;

	sf::Vector2f GetRandomPointAbove();
	sf::Vector2f GetPointAboveFrom(sf::Vector2f pos, float minDist);
	sf::Vector2f GetProjectedPoint(sf::Vector2f pos);
	sf::Vector2f GetStartPoint();
	sf::Vector2f GetEndPoint();
protected:

private:

	//Member Data
public:

protected:
	int m_width, m_height;
	sf::RectangleShape* m_render; //owned by parent
private:

};
#endif // !_PLATFORM_H_

