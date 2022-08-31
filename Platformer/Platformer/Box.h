#ifndef _BOX_H_
#define _BOX_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Box.h
// Description   	: Pickup and Droppable object.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Includes
#include "Entity.h"

class Box : public Entity
{
	//Member Functions
public:
	Box();
	virtual ~Box();

	bool Initialise(BackBuffer& buffer, std::string spawnCode);
	bool Deinitialise();

	//Inherited functions:
	virtual void Draw(BackBuffer& buffer);
	virtual void Process(float dT);
	virtual void ApplyGravity(float _grav);
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;
	virtual const sf::IntRect GetHitBox() const;
	virtual const sf::Vector2f GetSize() const;

	void SetGrounded(bool _isGrounded);
private:

protected:

	//Member Data
public:

private:
	int m_width, m_height;
	float m_scale;

protected:
	bool m_isGrounded;
};

#endif // _BOX_H_