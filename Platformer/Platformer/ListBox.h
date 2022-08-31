#ifndef _LIST_BOX_H_
#define _LIST_BOX_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : Listbox.h
// Description   : Widget handling a list. 
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Parent Include
#include "Widget.h"

class ListBox : public Widget
{
	//Member functions
public:
	ListBox();
	virtual ~ListBox();

	bool Initialise(BackBuffer& _buffer, sf::Vector2f position);

	void HandleHover(float _mouseX, float _mouseY);

	//Inherited Functions:
	virtual void Draw(BackBuffer& _backBuffer);
	virtual bool HandleMouse(float _mouseX, float _mouseY);
	virtual void Update();

	void AddString(std::string itemData);
	std::string GetStringSelected();
	std::string GetStringAt(unsigned int index);
	int GetLimit();
protected:
	//Member data
public:

private:
	sf::Color m_hover = sf::Color(200, 200, 200);
	sf::Color m_notHover = sf::Color(100, 100, 100);

protected:
	unsigned int m_displayStart, m_displayEnd, m_itemLimit, m_hoverValue;
	std::vector<std::string> m_items;
	sf::Sprite* m_pSpriteDown;
	float m_fontSize, m_buttonSize;
};

#endif // !_LIST_BOX_H_
