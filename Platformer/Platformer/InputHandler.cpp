//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: InputHandler.cpp
// Description   	: Handles the input from the keyboard and mouse on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "InputHandler.h"

//Local Includes
#include "BackBuffer.h"
#include "Game.h"
#include "Window.h"
#include "IniParser.h"

//Constructor
InputHandler::InputHandler()
	:	m_pBackBuffer(0) 
	,	m_KeyA(false), m_KeyD(false), m_KeySpace(false)
	,	m_KeyTilde(false), m_KeyEsc(false)
{

}

//Destructor
InputHandler::~InputHandler()
{
	//Doesn't handle BackBuffer destruction. 
	m_pBackBuffer = 0;
}

// Initialise( BackBuffer* buffer )
//
// Description:	Sets up the handler's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer*		Pointer to this game's back buffer.
//
// @return	boolean		Status of the initialisation.
//
bool InputHandler::Initialise(const BackBuffer& buffer)
{
	m_pBackBuffer = &buffer;
	return true;
}

// HandleGameInput( Game& _Game )
//
// Description:	Handles specific input for main game application.
//
// @param	Game&	reference to the game class.
//
// @return	n/a
//
void InputHandler::HandleGameInput(Game& _Game)
{
	sf::Event event;
	sf::RenderWindow* _window = m_pBackBuffer->GetWindow();
	int dir = 0; //Player direction

	//Poll All events and update key states
	while (_window->pollEvent(event))
	{
		//Key Pressed Events:
		if (event.type == sf::Event::KeyPressed)
		{	
			if (event.key.code == sf::Keyboard::A)
			{
				m_KeyA = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				m_KeyD = true;
			}
			if (event.key.code == sf::Keyboard::Q)
			{
				m_KeyQ = true;
			}
			if (event.key.code == sf::Keyboard::E)
			{
				m_KeyE = true;
			}
			if (event.key.code == sf::Keyboard::Q)
			{
				m_KeyQ = true;
			}
			if (event.key.code == sf::Keyboard::LShift)
			{
				m_KeyShift = true;
			}
			if (event.key.code == sf::Keyboard::Space)
			{
				m_KeySpace = true;
			}
			if (event.key.code == sf::Keyboard::Tilde)
			{
				m_KeyTilde = true;
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_KeyEsc = true;
			}
			if (event.key.code == sf::Keyboard::R)
			{
				m_KeyR = true;
			}
		}//Key Released Events:
		else if(event.type == sf::Event::KeyReleased)
		{
			if ((event.key.code == sf::Keyboard::A))
			{
				m_KeyA = false;
			}
			if ((event.key.code == sf::Keyboard::D))
			{
				m_KeyD = false;
			}
			if (event.key.code == sf::Keyboard::Q)
			{
				m_KeyQ = false;
			}
			if (event.key.code == sf::Keyboard::E)
			{
				m_KeyE = false;
			}
			if (event.key.code == sf::Keyboard::LShift)
			{
				m_KeyShift = false;
			}
			if (event.key.code == sf::Keyboard::Space)
			{
				m_KeySpace = false;
			}
			if (event.key.code == sf::Keyboard::Tilde)
			{
				m_KeyTilde = false;
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_KeyEsc = false;
			}
			if (event.key.code == sf::Keyboard::R)
			{
				m_KeyR = false;
			}
		}//Other Events:
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			_Game.MousePressed(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			_Game.MouseReleased(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			_Game.MouseMoved(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::Closed)
		{
			_Game.Quit();
		}
	}

	//Handle current state of input:
	if (m_KeyA)
	{
		dir -= 1;
	}
	if (m_KeyD)
	{
		dir += 1;
	}
	if (m_KeyQ)
	{
		_Game.DropItem();
	}
	if (m_KeyE)
	{
		_Game.PickUpItem();
	}
	if (m_KeyTilde)
	{
		_Game.ToggleDebug();
		m_KeyTilde = false;
		return;
	}
	if (m_KeyEsc)
	{
		_Game.TogglePause();
		m_KeyEsc = false;
		return;
	}

	//Constants:
	_Game.PlayerWalking(m_KeyShift);
	_Game.MovePlayer(dir);	//No keypress equates to 0.
	_Game.JumpPlayer(m_KeySpace);

	if (m_KeyR)
	{
		_Game.RestartLevel();
		m_KeyR = false;
	}
}

// HandleClickEvents( Window& _window )
//
// Description:	Handles only quit and mouse related events.
//
// @param	Window&	reference to the Window class.
//
// @return	n/a
//
void InputHandler::HandleClickEvents(Window& _window)
{
	sf::Event event;
	sf::RenderWindow* window = m_pBackBuffer->GetWindow();
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			_window.MousePressed(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			_window.MouseReleased(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			_window.MouseMoved(event.mouseMove.x, event.mouseMove.y);
		}
		else if (event.type == sf::Event::Closed)
		{
			_window.Quit(); //Force Quit
		}
	}
}

// HandleCinematicEvents( Window& _window )
//
// Description:	Handles only events related to cinematics.
//
// @param	Window&	reference to the Window class.
//
// @return	n/a
//
void InputHandler::HandleCinematicEvents(Window& _window)
{
	sf::Event event;
	sf::RenderWindow* window = m_pBackBuffer->GetWindow();
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::KeyReleased)
		{
			_window.SetRunning(false);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			_window.SetRunning(false);
		}
		else if (event.type == sf::Event::Closed)
		{
			_window.Quit(); //Force Quit
		}
	}
}