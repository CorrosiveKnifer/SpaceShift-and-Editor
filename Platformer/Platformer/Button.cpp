//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Button.cpp
// Description   	: A button that temporarily toggles when clicked.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "Button.h"

//Local Includes
#include "SpriteMap.h"
#include "BackBuffer.h"
#include "Window.h"

//Constructor
Button::Button()
	: m_Anim(0)
{

}

//Destructor
Button::~Button()
{
	if (m_Anim != 0)
	{
		delete m_Anim;
		m_Anim = 0;
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
bool Button::Initialise(sf::Sprite* _sprite)
{
	m_Anim = new SpriteMap();
	m_Anim->Initialise(_sprite, 2, 1);
	return Widget::Initialise(_sprite);
}

// HandleMouse( float _x, float _y )
//
// Description:	Handles when a mouse is over this widget.
//
// @param	float	position of the mouse on the x-axis.
// @param	float	position of the mouse on the y-axis.
//
// @return	boolean		Status of the initialisation.
//
bool Button::HandleMouse(float _x, float _y)
{
	if (m_isVisible)
	{
		if (_x >= m_x && _x < m_x + ((m_width * m_scale.x )/ 2) && _y >= m_y && _y < m_y + ((m_height * m_scale.y) /1))
		{
			m_value = 1.0f;
		}
		else
		{
			m_value = 0.0f;
		}
		Update();
	}
	return (m_value == 1);
}

// Update( )
//
// Description:	Update the visual status of the widget.
//
// @param	n/a
//
// @return	n/a
//
void Button::Update()
{
	m_Anim->SetIndexX(static_cast<int>(m_value));
}

// Draw( BackBuffer& _backBuffer )
//
// Description:	Draws the widget to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the widget to.
//
// @return	n/a
//
void Button::Draw(BackBuffer& _backBuffer)
{
	if (m_isVisible)
		_backBuffer.Draw(*m_sprite);
}