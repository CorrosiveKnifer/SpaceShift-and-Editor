#ifndef _BUTTON_OBJ_H_
#define _BUTTON_OBJ_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: ButtonObject.h
// Description   	: Togglable button within the world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Entity.h"

//Forward Declaration
class Door;

class ButtonObject : public Entity
{
	//Member Functions
public:
	ButtonObject();
	virtual ~ButtonObject();

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

	void SetTriggered(bool triggered);
	void AddLink(Door* _newLink);
private:

protected:

	//Member Data
public:

private:
	int m_width, m_height;
	sf::Sprite* m_pButtonTop;
	bool m_isTriggered;
	std::vector<Door*> m_links; //Owned by Door
protected:
};
#endif // _BUTTON_OBJ_H_