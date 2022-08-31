//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : SpriteMap.cpp
// Description   : Functionality for a grid of sprites to be converted 
//				   into animation. Note: the logic of the animation is up to
//				   the class who has a sprite map.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Self Include
#include "SpriteMap.h"

//Constructor
SpriteMap::SpriteMap()
	:	m_Source(0), m_frameX(0), m_frameY(0), m_indexX(0), m_indexY(0)
	,   m_MaxIndexX(0),	m_MaxIndexY(0),	m_width(0),	m_height(0)
	,	m_isLooping(true),	m_delayX(1), m_delayY(1), m_timeX(0), m_timeY(0)
{

}

//Destructor
SpriteMap::~SpriteMap()
{
	//Sprite controlled by Entity/Widget
	if (m_frameX != 0)
	{
		delete[] m_frameX;
		m_frameX = 0;
	}
	if (m_frameY != 0)
	{
		delete[] m_frameY;
		m_frameY = 0;
	}
}

// Initialise( sf::Sprite* _Source, int frameWidth, int frameHeight )
//
// Description:	Sets up the Sprite map for use. This function MUST
//				be called before using any of the other functions.
//
// @param	sf::Sprite*	Sprite used with this widget
// @param	int		amount of columns within each frame.
// @param	int		amount of rows within each frame.
//
// @return	boolean		Status of the initialisation.
//
bool SpriteMap::Initialise(sf::Sprite* _Source, int frameWidth, int frameHeight)
{
	if (_Source != 0)
	{
		m_Source = _Source;
		m_MaxIndexX = frameWidth;
		m_MaxIndexY = frameHeight;
		m_width = _Source->getTexture()->getSize().x / m_MaxIndexX;
		m_height = _Source->getTexture()->getSize().y / m_MaxIndexY;
		m_frameX = new int[m_MaxIndexX];
		m_frameY = new int[m_MaxIndexY];
		m_indexX = 0;
		m_indexY = 0;
		m_loopS = 0;
		m_loopE = m_MaxIndexX;

		m_frameX[0] = 0;
		for (unsigned int x = 1; x < m_MaxIndexX; ++x)
		{
			m_frameX[x] = m_frameX[x - 1] + m_width;
		}

		m_frameY[0] = 0;
		for (unsigned int y = 1; y < m_MaxIndexY; ++y)
		{
			m_frameY[y] = m_frameY[y - 1] + m_width;
		}
		m_Source->setTextureRect(sf::IntRect(m_frameX[m_indexX], m_frameY[m_indexY], m_width, m_height));
		return true;
	}
	return false;
}

// SetDelay( float x, float y )
//
// Description:	Update the time delay between updating between frames.
//
// @param	float	delay between columns.
// @param	float	delay between rows.
//
// @return	n/a
//
void SpriteMap::SetDelay(float x, float y)
{
	if (x >= 0 && y >= 0)
	{
		m_delayX = x;
		m_delayY = y;
	}
}

// ProcessX( float dT )
//
// Description:	Updates the position sprite render depending on the delay.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void SpriteMap::ProcessX(float dT)
{
	if (m_indexX < m_loopE)
	{
		if (m_delayX != 0)
		{//Only ticks up if delay exists
			m_timeX += dT;
			if (m_timeX > m_delayX)
			{
				SetIndexX(m_indexX + 1);
				m_timeX -= m_delayX;
			}
		}
		else
		{
			SetIndexX(m_indexX + 1);
		}

		if (m_indexX >= m_loopE)
		{
			SetIndexX(m_isLooping ? m_loopS : m_loopE); //true = 0, false = clamp
		}
	}
}

// ProcessY( float dT )
//
// Description:	Updates the position sprite render depending on the delay.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void SpriteMap::ProcessY(float dT)
{
	if (m_delayY != 0)
	{//Only ticks up if delay exists
		m_timeY += dT; 
		if (m_timeY > m_delayY)
		{
			SetIndexY(m_indexY + 1);
			m_timeY -= m_delayY;
		}
	}
	else
	{
		SetIndexY(m_indexY + 1);
	}

	if (m_indexY >= m_MaxIndexY)
	{
		SetIndexY(m_isLooping ? 0 : m_MaxIndexY - 1); //true = 0, false = clamp
	}
}

// IsAnimationFinished( )
//
// Description:	returns the curreny Status of the animation strip.
//				
// @return	boolean		Status of the current animation strip.
//
bool SpriteMap::IsAnimationFinished()
{
	return m_indexX >= m_loopE;
}

// IsLooping( )
//
// Description:	returns if the curreny animation strip is set to loop.
//				
// @return	boolean		Status of if it is set to loop.
//
bool SpriteMap::IsLooping()
{
	return m_isLooping;
}

// SetIsLooping( float dT )
//
// Description:	Update the state of if to loop the sprite.
//				
// @param	bool	new state of if the sprite loops.
//
// @return	n/a
//
void SpriteMap::SetIsLooping(bool _isLooping)
{
	m_isLooping = _isLooping;
}

// SetLoopingBoundsX( unsigned int start, unsigned int end )
//
// Description:	Update the bounds at which the animation loops between.
//				Note: if the current frame is NOT within these bounds, it 
//				will NOT skip upto the start.
//				
// @param	unsigned int	Starting Frame.
// @param	unsigned int	Ending Frame.
//
// @return	n/a
//
void SpriteMap::SetLoopingBoundsX(unsigned int start, unsigned int end)
{
	m_loopS = start;
	m_loopE = end;
}
// GetIndexX( )
//
// Description:	returns the current index of which column the animation is at.
//				
// @param	n/a
//
// @return	int		The current index of which column the animation is at.
//
int SpriteMap::GetIndexX() const
{
	return m_indexX;
}

// GetIndexY( )
//
// Description:	returns the current index of which column the animation is at.
//				
// @param	n/a
//
// @return	int		The current index of which row the animation is at.
//
int SpriteMap::GetIndexY() const
{
	return m_indexY;
}

// SetIndexX( )
//
// Description:	Force the animation to update to a specific column index.
//				
// @param	int		new column index.
//
// @return	n/a
//
void SpriteMap::SetIndexX(int _x)
{
	m_indexX = _x;
	m_Source->setTextureRect(sf::IntRect(m_frameX[m_indexX], m_frameY[m_indexY], m_width, m_height));
}

// SetIndexY( )
//
// Description:	Force the animation to update to a specific row index.
//				
// @param	int		new row index.
//
// @return	n/a
//
void SpriteMap::SetIndexY(int _y)
{
	m_indexY = _y;
	m_Source->setTextureRect(sf::IntRect(m_frameX[m_indexX], m_frameY[m_indexY], m_width, m_height));
}