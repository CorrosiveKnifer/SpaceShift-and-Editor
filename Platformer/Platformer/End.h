#ifndef _END_H_
#define _END_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: End.h
// Description   	: End location which is the goal of the player.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Includes
#include "Entity.h"

//Forward Declaration
class SpriteMap;

class End : public Entity
{
	//Member functions
public:
	End();
	virtual ~End();

	bool Initialise(BackBuffer& buffer, std::string spawnCode);
	bool Deinitialise();

	//Inherited Functions:
	virtual void Draw(BackBuffer& buffer);
	virtual void Process(float dT);
	virtual void ApplyGravity(float _grav);
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;
	virtual const sf::IntRect GetHitBox() const;
	virtual const sf::Vector2f GetSize() const;

protected:

private:

	//Member Data
public:

protected:
	SpriteMap* m_pAnim;
	int m_width, m_height;
	sf::IntRect m_hitbox;
private:
};
#endif // !_END_H_
