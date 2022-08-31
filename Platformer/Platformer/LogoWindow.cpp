//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: LogoWindow.cpp
// Description   	: Used to display a single logo which fades in and out.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "LogoWindow.h"

//Library Includes

//Local Includes
#include "InputHandler.h"
#include "TextObject.h"

//Constructor
LogoWindow::LogoWindow()
	: m_logo(0)
	, m_isInputEnabled(true)
	, m_screenColor(sf::Color::Black)
{

}

//Destructor
LogoWindow::~LogoWindow()
{
	
}

// Initialise( BackBuffer* _backBuffer , SoundBuffer* _soundBuffer )
//
// Description:	Sets up the window's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer*		Pointer to this game's back buffer.
// @param	SoundBuffer*	Pointer to this game's sound buffer.
//
// @return	bool	Status of the initialisation.
//
bool LogoWindow::Initialise(BackBuffer* _backBuffer, SoundBuffer* _soundBuffer)
{
	return Window::Initialise(*_backBuffer, *_soundBuffer);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the window to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool LogoWindow::Deinitialise()
{
	if (m_logo != 0)
	{
		delete m_logo;
		m_logo = 0;
	}
	ClearTexts();
	return Window::Deinitialise();
}

// DoWindowLoop( )
//
// Description:	The main while loop that takes input, processes then draws the frame.
//				
// @param	n/a
//
// @return	bool	if the loop was stopped due to force close.
//
bool LogoWindow::DoWindowLoop()
{
	m_timePassed = 0;
	m_IsRunning = true;
	m_Clock.restart();
	do {
		float deltaTime = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();

		if (m_isInputEnabled)
		{
			m_pInputHandler->HandleCinematicEvents(*this);
		}

		m_lag += deltaTime;
		if (m_lag >= sm_frameTime) //Fixing to 60 frames
		{
			Process(sm_frameTime);
			m_lag -= sm_frameTime;
			Draw(*m_pBackBuffer);
		}

		m_timePassed += deltaTime;

	} while (m_IsRunning);
	return m_HasQuit;
}

// DrawContents( BackBuffer& buffer )
//
// Description:	Draws all objects to the screen using the backbuffer provided.
//				
// @param	BackBuffer&		The back buffer to draw all of the objects to.
//
// @return	n/a
//
void LogoWindow::DrawContents(BackBuffer& buffer)
{
	if(m_logo != 0)
		buffer.Draw(*m_logo);

	for (TextObject* t : m_texts)
	{
		t->Draw(buffer);
	}
	buffer.SetColour(m_screenColor);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void LogoWindow::Process(float dT)
{
	if (m_logo != 0)
	{
		if (m_timePassed < m_renderTime / 2) //Fade in
		{
			m_logo->setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * (m_timePassed / (m_renderTime / 2)))));
		}
		else if (m_timePassed > m_renderTime / 2) //Fade out
		{
			float alpha = 1 - (m_timePassed - (m_renderTime / 2)) / (m_renderTime / 2);
			m_logo->setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));
		}
		else if (m_timePassed > m_renderTime) //Reset colour
		{
			m_logo->setColor(sf::Color(255, 255, 255, 0));
		}
		else
		{
			m_logo->setColor(sf::Color(255, 255, 255, 255));
		}
	}

	if (m_timePassed >= m_renderTime)
	{
		m_IsRunning = false;
	}
}

// SetLogo( sf::Sprite* sp )
//
// Description:	Updates the logo to a new one, while deleting the old one.
//				
// @param	sf::Sprite*		Pointer to the new logo.
//
// @return	n/a
//
void LogoWindow::SetLogo(sf::Sprite* sp)
{
	if (m_logo != 0)
	{
		delete m_logo;
	}
	m_logo = sp;
	sf::Vector2f pos;
	pos.x = (m_pBackBuffer->GetWindowSize().x - (sp->getTexture()->getSize().x * sp->getScale().x)) / 2;
	pos.y = (m_pBackBuffer->GetWindowSize().y - (sp->getTexture()->getSize().y * sp->getScale().y)) / 2;
	m_logo->setPosition(pos);
}

// SetBackgroundColour( sf::Color col )
//
// Description:	Updates the background colour of the backbuffer.
//				
// @param	sf::Color	Background colour value.
//
// @return	n/a
//
void LogoWindow::SetBackgroundColour(sf::Color col)
{
	m_screenColor = col;
}

// SetTimeLength( float _time )
//
// Description:	Updates the total length of time for the window to live for.
//				
// @param	float	Total time for the logo to be displayed.
//
// @return	n/a
//
void LogoWindow::SetTimeLength(float _time)
{
	m_renderTime = _time;
}

// SetInputEnabled( bool )
//
// Description:	Updates if the user can send input during the logo screen.
//				
// @param	bool	if input is allowed
//
// @return	n/a
//
void LogoWindow::SetInputEnabled(bool _input)
{
	m_isInputEnabled = _input;
}

// SetLogoLoc( sf::Vector2f )
//
// Description:	Sets the location of the logo on the screen.
//				
// @param	sf::Vector2f	new Logo position. 
//
// @return	n/a
//
void LogoWindow::SetLogoLoc(sf::Vector2f logoLoc)
{
	if (m_logo != 0)
	{
		m_logo->setPosition(logoLoc);
	}
}

// AddText( std::string, sf::Vector2f )
//
// Description:	Add text to the screen to be drawn with the logo.
//	
// @param	std::string		new string to the screen. 
// @param	sf::Vector2f	location of the string. 
//
// @return	n/a
//
void LogoWindow::AddText(std::string msg, sf::Vector2f loc)
{
	TextObject* temp = new TextObject();
	temp->Initialise(msg, loc, 25);
	m_texts.push_back(temp);
}

// ClearTexts( )
//
// Description:	Clear, and clean memory used for, all texts within this logo window.
//	
// @return	n/a
//
void LogoWindow::ClearTexts()
{
	std::vector<TextObject*>::iterator iter = m_texts.begin();
	while(iter != m_texts.end())
	{
		(*iter)->Deinitialise();
		delete * iter;
		iter++;
	}
	m_texts.clear();
}