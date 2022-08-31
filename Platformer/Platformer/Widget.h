#ifndef _WIDGET_H_
#define _WIDGET_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Widget.h
// Description   	: Widget parent class to buttons, scale bars and check boxes.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Library Includes
#include <SFML/Graphics.hpp>

//Forward Declaration
class BackBuffer;

//Implementation
class Widget
{
	//Member Function
public:
	Widget();
	virtual ~Widget();

	bool Initialise(sf::Sprite* _sprite);

	virtual void Draw(BackBuffer& _backBuffer) = 0;
	virtual bool HandleMouse(float _mouseX, float _mouseY) = 0;
	virtual void Update() = 0;

	void SetValue(float _val);
	void SetValue(bool _val);
	void SetVisible(bool _visible);

	bool GetValueAsBoolean();
	float GetValueAsFloat();
	void SetWorldPosition(sf::Vector2f _pos);
	void SetScale(sf::Vector2f _scale);
	void SetTargetWidth(float _width);
	sf::Vector2f GetScale();
private:

protected:

	//Member Data
public:

private:

protected:
	sf::Sprite* m_sprite;
	sf::Vector2f m_scale;
	bool m_isVisible;
	float m_x;
	float m_y;
	float m_width;
	float m_height;

	float m_value;
};
#endif // _WIDGET_H_