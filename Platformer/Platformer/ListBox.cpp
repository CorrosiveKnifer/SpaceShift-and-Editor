//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : Listbox.cpp
// Description   : Widget handling a list.  
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Self Include
#include "ListBox.h"

//Local Includes
#include "BackBuffer.h"

//Constructor
ListBox::ListBox()
	: m_displayStart(0), m_displayEnd(0), m_itemLimit(0), m_hoverValue(0)
	, m_pSpriteDown(0)
	, m_fontSize(0), m_buttonSize(0)
{

}

//Destructor
ListBox::~ListBox()
{
	delete m_pSpriteDown;
	m_pSpriteDown = 0;
}

// Initialise( BackBuffer&, sf::Vector2f)
//
// Description:	Sets up the widget's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer&		Back buffer to create sprite.
// @param	sf::Vector2f	Position of the widget.
//
// @return	boolean		Status of the initialisation.
//
bool ListBox::Initialise(BackBuffer& _buffer, sf::Vector2f position)
{
	//Create Sprite
	sf::Sprite* up = _buffer.CreateSprite("widget/UpButton.png");
	sf::Sprite* down = _buffer.CreateSprite("widget/DownButton.png");
	up->setScale(0.25, 0.25);
	down->setScale(0.25, 0.25);
	up->setColor(m_notHover);
	down->setColor(m_notHover);

	bool cond1 = Widget::Initialise(up);
	m_pSpriteDown = down;
	m_buttonSize = down->getTexture()->getSize().y * 0.25f;

	m_x = position.x;
	m_y = position.y;
	m_scale = sf::Vector2f(1, 1);
	m_width = 150;
	m_height = 75;
	m_fontSize = 15;
	m_itemLimit = static_cast<int>(m_height / m_fontSize);
	m_displayStart = 0;
	m_displayEnd = m_itemLimit;

	m_value = 0;

	return cond1;
}

// Draw( BackBuffer& )
//
// Description:	Draws the widget to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the widget to.
//
// @return	n/a
//
void ListBox::Draw(BackBuffer& _backBuffer)
{
	_backBuffer.SetColour(sf::Color::White);
	_backBuffer.DrawRectangle(m_x, m_y, m_width, m_height);
	_backBuffer.SetFontSize(static_cast<int>(m_fontSize));
	_backBuffer.SetFontAlign(Align::Left);
	_backBuffer.SetColour(sf::Color::Black);

	//Draw list:
	for (unsigned int i = m_displayStart; i < m_displayEnd; i++)
	{
		int offset = m_displayEnd - m_itemLimit;
		if (m_value == i)
		{
			_backBuffer.SetColour(m_notHover);
			_backBuffer.DrawRectangle(m_x, m_y + m_fontSize * (i - offset), m_width, m_fontSize);
			_backBuffer.SetColour(sf::Color::Black);
		}
		else if (m_hoverValue == i)
		{
			_backBuffer.SetColour(m_hover);
			_backBuffer.DrawRectangle(m_x, m_y + m_fontSize * (i - offset), m_width, m_fontSize);
			_backBuffer.SetColour(sf::Color::Black);
		}
		if (i < m_items.size())
		{
			_backBuffer.DrawTextToWorld(m_items.at(i), m_x, m_y + m_fontSize * (i - offset));
		}
	}

	//Draw side buttons:
	_backBuffer.SetColour(sf::Color(140, 140, 140));
	_backBuffer.DrawRectangle(m_x + m_width, m_y, m_buttonSize, m_height);
	_backBuffer.DrawAt(*m_sprite, sf::Vector2f(m_x + m_width, m_y));
	_backBuffer.DrawAt(*m_pSpriteDown, sf::Vector2f(m_x + m_width, m_y + m_height - m_buttonSize));
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
bool ListBox::HandleMouse(float _mouseX, float _mouseY)
{
	if (m_isVisible)
	{
		//Location of buttons:
		sf::Vector2f upButton(m_x + m_width, m_y);
		sf::Vector2f downButton(m_x + m_width, m_y + m_height - m_buttonSize);

		if (_mouseX >= m_x && _mouseX < m_x + (m_width) && _mouseY >= m_y && _mouseY < m_y + (m_height))
		{
			//Update selected value: 

			unsigned int tempVal = (m_displayEnd - m_itemLimit) + (int)((_mouseY - m_y) / 15);

			if(tempVal < m_items.size())
				m_value = static_cast<float>(tempVal);
		}
		if (_mouseX >= upButton.x && _mouseX < upButton.x + (m_buttonSize) && _mouseY >= upButton.y && _mouseY < upButton.y + (m_buttonSize))
		{
			//Accend list
			if (m_displayStart > 0)
			{
				m_displayStart--;
				m_displayEnd--;
			}
		}
		if (_mouseX >= downButton.x && _mouseX < downButton.x + (m_buttonSize) && _mouseY >= downButton.y && _mouseY < downButton.y + (m_buttonSize))
		{
			//Decend list
			if (m_displayStart + m_itemLimit < m_items.size())
			{
				m_displayStart++;
				m_displayEnd++;
			}
		}
		Update();
	}
	return false;
}

// HandleHover( float _x, float _y )
//
// Description:	Handles when a mouse hovers over this widget.
//
// @param	float	position of the mouse on the x-axis.
// @param	float	position of the mouse on the y-axis.
//
// @return	boolean		Status of the initialisation.
//
void ListBox::HandleHover(float _mouseX, float _mouseY)
{
	//Location of buttons:
	sf::Vector2f upButton(m_x + m_width, m_y);
	sf::Vector2f downButton(m_x + m_width, m_y + m_height - m_buttonSize);

	//Up button hover
	if (_mouseX >= upButton.x && _mouseX < upButton.x + (m_buttonSize) && _mouseY >= upButton.y && _mouseY < upButton.y + (m_buttonSize))
	{
		m_sprite->setColor(m_hover);
	}
	else
	{
		m_sprite->setColor(m_notHover);
	}

	//Down button hover
	if (_mouseX >= downButton.x && _mouseX < downButton.x + (m_buttonSize) && _mouseY >= downButton.y && _mouseY < downButton.y + (m_buttonSize))
	{
		m_pSpriteDown->setColor(m_hover);
	}
	else
	{
		m_pSpriteDown->setColor(m_notHover);
	}

	//Hover over items
	if (_mouseX >= m_x && _mouseX < m_x + (m_width) && _mouseY >= m_y && _mouseY < m_y + (m_height))
	{
		unsigned int tempVal = (m_displayEnd - m_itemLimit) + (int)((_mouseY - m_y) / 15);

		if (tempVal < m_items.size())
			m_hoverValue = tempVal;
	}
}

// Update( )
//
// Description:	Update the visual status of the widget.
//
// @param	n/a
//
// @return	n/a
//
void ListBox::Update()
{
	//Do Nothing
}

// AddString( std::string )
//
// Description:	Add a string to the list.
//
// @param	std::string		item string to be added.
//
// @return	n/a
//
void ListBox::AddString(std::string itemData)
{
	m_items.push_back(itemData);
}

// GetStringSelected( )
//
// Description:	Returns the value which is selected
//
// @return	std::string		item string which is selected.
//
std::string ListBox::GetStringSelected()
{
	return m_items.at(static_cast<int>(m_value));
}

// GetStringAt( int )
//
// Description:	Returns the value which is selected
//
// @param	int		index on the list.
//
// @return	std::string		item string which has the same index.
//
std::string ListBox::GetStringAt(unsigned int index)
{
	if (index < m_items.size())
	{
		return m_items.at(index);
	}
	else
	{
		return m_items.at(m_items.size() - 1);
	}
}

// GetLimit( )
//
// Description:	Returns the size of the list
//
// @return	int		Size of the list.
//
int ListBox::GetLimit()
{
	return m_items.size();
}