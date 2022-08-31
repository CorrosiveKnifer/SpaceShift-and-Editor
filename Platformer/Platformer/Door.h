#ifndef _DOOR_H_
#define _DOOR_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Door.h
// Description   	: Door within the world which blocks the player.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Platform.h"

class Door : public Platform
{
	//Member Functions
public:
	Door();
	virtual ~Door();

	bool Initialise(std::string platformCode);
	bool Deinitialise();

	//Parent Includes
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;

	void SetTrigger(bool _trigger);
	bool GetTrigger();

	void SetOpen(bool _isOpen);
	bool IsOpen();
private:

protected:

	//Member Data
public:

private:
	bool m_isTriggered;
	bool m_baseState;
	bool m_currState;

protected:
};
#endif // !_DOOR_H_
