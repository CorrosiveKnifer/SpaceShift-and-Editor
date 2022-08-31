#ifndef _BULLET_H_
#define _BULLET_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Bullet.h
// Description   	: Bullet created by guards.
// Author         	: Michael Jordan
// Mail         	: michael.jor8823@mediadesign.com
//
#include "BackBuffer.h"

class Bullet
{
	//Member Functions
public:
	Bullet();
	~Bullet();

	bool Initialise(sf::Vector2f dirVect, sf::Vector2f origin);
	bool Deinitialise();

	void Draw(BackBuffer& buffer);
	void Process(float dT);

	bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	sf::IntRect GetHitBox();
private:

protected:
	
	//Member Data
public:

private:
	sf::Vector2f m_dirVector, m_origin;
	sf::RectangleShape m_bullet;
	sf::RectangleShape m_streak;
	float m_speed;
protected:
};
#endif // !_BULLET_H_
