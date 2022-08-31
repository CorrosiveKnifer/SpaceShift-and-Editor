//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Widget.cpp
// Description   	: Widget parent class to buttons, scale bars and check boxes.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self include
#include "Widget.h"

//Constructor
Widget::Widget()
	: m_sprite(0), m_scale(1, 1)
	, m_isVisible(true)
	, m_x(0), m_y(0)
	, m_width(0), m_height(0)
	, m_value(0)
{

}

//Destructor
Widget::~Widget()
{
	if (m_sprite != 0)
	{
		delete m_sprite;
		m_sprite = 0;
	}
}

// Initialise( sf::Sprite* _sprite )
//
// Description:	Sets up the widget's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	sf::Sprite*	Sprite used with this widget
//
// @return	boolean		Status of the initialisation.
//
bool Widget::Initialise(sf::Sprite* _sprite)
{
	m_sprite = _sprite;
	
	if (m_sprite != 0)
	{
		m_x = m_sprite->getPosition().x;
		m_y = m_sprite->getPosition().y;
		m_scale = m_sprite->getScale();
		m_width = static_cast<float>(m_sprite->getTexture()->getSize().x);
		m_height = static_cast<float>(m_sprite->getTexture()->getSize().y);
	}
	
	return (m_sprite != 0);
}

// SetValue( float _val )
//
// Description:	Updates the widget's value to the float provided.
//
// @param	float	new value for this widget.
//
// @return	n/a
//
void Widget::SetValue(float _val)
{
	m_value = _val;
	Update();
}

// SetValue( bool _val )
//
// Description:	Updates the widget's value to either 1.0f or 0.0f provided.
//
// @param	boolean		new value as solid 1.0f(true) or 0.0f.
//
// @return	n/a
//
void Widget::SetValue(bool _val)
{
	if (_val)
	{
		m_value = 1.0f;
	}
	else
	{
		m_value = 0.0f;
	}
	Update();
}

// SetVisible( bool _val )
//
// Description:	Updates the visiblity of the widget.
//
// @param	boolean		new visiblity of the widget.
//
// @return	n/a
//
void Widget::SetVisible(bool _visible)
{
	m_isVisible = _visible;
}

// GetValueAsBoolean( )
//
// Description:	returns the widget's value as a boolean.
//
// @param	n/a
//
// @return	bool	true if value is equal to 1.0 (or more).
//
bool Widget::GetValueAsBoolean()
{
	return (m_value >= 1.0f);
}

// GetValueAsFloat( )
//
// Description:	returns the widget's value as a float.
//
// @param	n/a
//
// @return	float	the value of the widget.
//
float Widget::GetValueAsFloat()
{
	return m_value;
}

// SetWorldPosition( sf::Vector2f _pos )
//
// Description:	Updates the position of the widget and it's sprite.
//
// @param	sf::Vector2f	new position of the widget.
//
// @return	n/a
//
void Widget::SetWorldPosition(sf::Vector2f _pos)
{
	m_x = _pos.x;
	m_y = _pos.y;
	m_sprite->setPosition(_pos);
}

// SetScale( sf::Vector2f _scale )
//
// Description:	Updates the scale of the widget and it's sprite.
//
// @param	sf::Vector2f	new scale of the widget.
//
// @return	n/a
//
void Widget::SetScale(sf::Vector2f _scale)
{
	m_scale = _scale;
	m_sprite->setScale(_scale);
	Update();
}

// SetTargetWidth( float _width )
//
// Description:	Updates the scale so that the size of the widget
//				grow to the desired width.
//
// @param	width	new desired width of the widget.
//
// @return	n/a
//
void Widget::SetTargetWidth(float _width)
{
	SetScale(sf::Vector2f(_width / m_sprite->getTexture()->getSize().x, m_scale.y));
}

// GetScale( )
//
// Description:	Returns the scale of this widget.
//
// @param	n/a
//
// @return	sf::Vector2f
//
sf::Vector2f Widget::GetScale()
{
	return m_scale;
}