#ifndef _TEXT_OBJECT_H_
#define _TEXT_OBJECT_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : TextObject.cpp
// Description   : Define the text object within the world.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Parent Include
#include "Entity.h"

class TextObject : public Entity
{
	// Member function
public:
	TextObject();
	virtual ~TextObject();

	bool Initialise(std::string _text, std::string _loc, int size);
	bool Initialise(std::string _text, sf::Vector2f _loc, int size);
	bool Deinitialise();

	//Inherited Functions
	virtual void Draw(BackBuffer& buffer);
	virtual void Process(float dT);
	virtual void ApplyGravity(float _grav);
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;
	virtual const sf::IntRect GetHitBox() const;
	virtual const sf::Vector2f GetSize() const;

private:

protected:

	//Member data
public:

private:
	std::string m_text;
	int m_size;

protected:

};
#endif // !_TEXT_OBJECT_H_
