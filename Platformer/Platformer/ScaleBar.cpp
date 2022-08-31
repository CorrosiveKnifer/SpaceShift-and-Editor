//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: ScaleBar.cpp
// Description   	: A Scale bar that allows the user to select between a range.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "ScaleBar.h"

//Local Includes
#include "BackBuffer.h"

//Constructor
ScaleBar::ScaleBar()
	: m_fMin(0), m_fMax(1)
	, m_fInc(1)
	, m_pCursor(0)
	, CursorY(0)
{

}

//Destructor
ScaleBar::~ScaleBar()
{
	if (m_pCursor != 0)
	{
		delete m_pCursor;
		m_pCursor = 0;
	}
}

//Copy Constructor (for the lols)
ScaleBar::ScaleBar(const ScaleBar& _other)
{
	this->m_fMin = _other.m_fMin;
	this->m_fMax = _other.m_fMax;
	this->m_fInc = _other.m_fInc;
	this->m_isVisible = _other.m_isVisible;
	this->m_x = _other.m_x;
	this->m_y = _other.m_y;
	this->m_width = _other.m_width;
	this->m_height = _other.m_height;
	this->m_value = _other.m_value;
	//Other contents tied to initialise.
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
bool ScaleBar::Initialise(BackBuffer& _backBuffer, float _x, float _y)
{
	sf::Sprite* sp = _backBuffer.CreateSprite("widget/ScaleBar.png");
	m_pCursor = _backBuffer.CreateSprite("widget/ScaleCursor.png");
	sp->setPosition(_x, _y + (m_pCursor->getTexture()->getSize().y/4));
	Widget::Initialise(sp);
	Widget::SetScale(sf::Vector2f(1.5f, 0.8f)); //DEFAULT
	m_pCursor->setScale(m_scale.y, 1);
	CursorY = _y;
	Update();
	return true;
}

// SetBounds( float _min, float _max )
//
// Description:	Update the minimum and maximum values of the scale bar.
//
// @param	float	minimum float value.
// @param	float	maximum float value.
//
// @return	n/a
//
void ScaleBar::SetBounds(float _min, float _max)
{
	if (_min < _max)
	{
		m_fMin = _min;
		m_fMax = _max;
	}
}

// SetIncrement( float _delta )
//
// Description:	updating the value for each stepping incrememt.
//
// @param	float	increment float value.
//
// @return	n/a
//
void ScaleBar::SetIncrement(float _delta)
{
	if(m_fInc > 0)
		m_fInc = _delta;
}

// Draw( BackBuffer& _backBuffer )
//
// Description:	Draws the widget to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the widget to.
//
// @return	n/a
//
void ScaleBar::Draw(BackBuffer& _backBuffer)
{
	if (m_isVisible)
	{
		sf::Vector2f offsetLoc;

		//Screen space inbetween each increment:
		float space = (m_width * m_scale.x) / ((m_fMax + m_fMin) / m_fInc);
		float dashH = m_y / 2;
		int j = 0;
		for (float i = m_fMin; i <= m_fMax; i += m_fInc)
		{
			_backBuffer.SetColour(sf::Color::Black);
			_backBuffer.DrawRectangle(m_x + j * space, m_y + 5, 2, 8);
			j++;
		}
		_backBuffer.Draw(*m_pCursor);
	}
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
bool ScaleBar::HandleMouse(float _mouseX, float _mouseY)
{
	if (m_isVisible)
	{
		float _x = _mouseX - m_x;
		float _y = _mouseY - m_y;
		float space = (m_sprite->getTexture()->getSize().x * m_scale.x) / ((m_fMax + m_fMin) / m_fInc);
		if (_mouseX >= m_x && _mouseX < m_x + (m_width * m_scale.x) && _mouseY >= m_y && _mouseY < m_y + (m_height * m_scale.y))
		{
			int index = static_cast<int>(std::roundf(_x / space));
			float dis = space * index;

			m_value = index * m_fInc;
			Update();
			return true;
		}
	}
	return false;
}

// Update( )
//
// Description:	Update the visual status of the widget.
//
// @param	n/a
//
// @return	n/a
//
void ScaleBar::Update()
{
	sf::Vector2f m_curPos;
	float space = (m_width * m_scale.x) / ((m_fMax + m_fMin) / m_fInc);
	m_curPos.x = m_x + (m_value / m_fInc) * space;
	m_curPos.y = CursorY;
	m_pCursor->setPosition(m_curPos);
}

// SetWorldPosition( sf::Vector2f _pos )
//
// Description:	Updates the position of the widget and it's sprite 
//				(including the cursor).
//
// @param	sf::Vector2f	new position of the widget.
//
// @return	n/a
//
void ScaleBar::SetWorldPosition(sf::Vector2f _pos)
{
	CursorY = _pos.y;
	Widget::SetWorldPosition(_pos);
	Update();
}