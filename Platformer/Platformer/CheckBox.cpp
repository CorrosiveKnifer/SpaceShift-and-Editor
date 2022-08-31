//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: CheckBox.cpp
// Description   	: A checkbox that toggles between true and false
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Includes
#include "Checkbox.h"

//Local Includes
#include "BackBuffer.h"
#include "SpriteMap.h"

//Constructor
CheckBox::CheckBox()
	: m_Anim(0)
{

}

//Destructor
CheckBox::~CheckBox()
{
	if (m_Anim != 0)
	{
		delete m_Anim;
		m_Anim = 0;
	}
}

//Copy Constructor
CheckBox::CheckBox(const CheckBox& _other)
{
	this->m_scale = _other.m_scale;
	this->m_isVisible = _other.m_isVisible;
	this->m_x = _other.m_x;
	this->m_y = _other.m_y;
	this->m_width = _other.m_width;
	this->m_height = _other.m_height;
	this->m_value = _other.m_value;
}

// Initialise( BackBuffer& _backBuffer, float _x, float _y )
//
// Description:	Sets up the widget's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer&		Back buffer to create sprite.
// @param	float	Position of the widget on the x-axis.
// @param	float	Position of the widget on the y-axis.
//
// @return	boolean		Status of the initialisation.
//
bool CheckBox::Initialise(BackBuffer& _backBuffer, float _x, float _y)
{
	sf::Sprite* sp = _backBuffer.CreateSprite("widget/checkbox.png");
	sp->setPosition(_x, _y);
	sp->setScale(0.5, 0.5);
	m_scale.x = 0.5;
	m_scale.y = 0.5;
	m_Anim = new SpriteMap();
	m_Anim->Initialise(sp, 2, 1);
	m_Anim->SetIsLooping(true);
	bool cond = Widget::Initialise(sp);
	if (cond)
	{
		m_width /= 2;
	}
	return cond;
}

// Draw( BackBuffer& _backBuffer )
//
// Description:	Draws the widget to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the widget to.
//
// @return	n/a
//
void CheckBox::Draw(BackBuffer& _backBuffer)
{
	if(m_isVisible)
		_backBuffer.Draw(*m_sprite);
}

// Update( )
//
// Description:	Update the visual status of the widget.
//
// @param	n/a
//
// @return	n/a
//
void CheckBox::Update()
{
	m_Anim->SetIndexX(static_cast<int>(m_value));
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
bool CheckBox::HandleMouse(float _mouseX, float _mouseY)
{
	if (m_isVisible)
	{
		if (_mouseX >= m_x && _mouseX < m_x + (m_width * m_scale.x) && _mouseY >= m_y && _mouseY < m_y + (m_height*m_scale.y))
		{
			if (m_value == 0.0f)
			{
				m_value = 1.0f;
			}
			else
			{
				m_value = 0.0f;
			}
			Update();
			return true;
		}
	}
	return false;
}