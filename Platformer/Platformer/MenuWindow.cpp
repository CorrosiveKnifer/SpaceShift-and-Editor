//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: MenuWindow.cpp
// Description   	: A class for the Main menu, processed before the game is played.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//This include
#include "MenuWindow.h"

//Local includes
#include "Button.h"
#include "Game.h"
#include "MiniWindow.h"
#include "resource.h"
#include "ListBox.h"
#include "Iniparser.h"
#include "LogoWindow.h"

//Library includes
#include <windows.h>

//Constructor
MenuWindow::MenuWindow()
	: m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pBackground(0)
	, m_pPlayBtn(0), m_pSelectBtn(0), m_pSettingBtn(0), m_pQuitBtn(0)
{	 
	 
}	

//Destructor
MenuWindow::~MenuWindow()
{
	if (m_mousePressPos != 0)
	{
		delete m_mousePressPos;
		m_mousePressPos = 0;
	}

	if (m_pPlayBtn != 0)
	{
		delete m_pPlayBtn;
		m_pPlayBtn = 0;
	}

	if (m_pSelectBtn != 0)
	{
		delete m_pSelectBtn;
		m_pSelectBtn = 0;
	}

	if (m_pSettingBtn != 0)
	{
		delete m_pSettingBtn;
		m_pSettingBtn = 0;
	}

	if (m_pQuitBtn != 0)
	{
		delete m_pQuitBtn;
		m_pQuitBtn = 0;
	}

	if (m_levelList != 0)
	{
		delete m_levelList;
		m_levelList = 0;
	}

	if (m_pBackground != 0)
	{
		delete m_pBackground;
		m_pBackground = 0;
	}

	Window::Deinitialise();
}

// Initialise( BackBuffer* _backBuffer , SoundBuffer* _soundBuffer )
//
// Description:	Sets up the window's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer*		Pointer to this game's back buffer.
// @param	SoundBuffer*	Pointer to this game's sound buffer.
//
// @return	boolean		Status of the initialisation.
//
bool MenuWindow::Initialise(BackBuffer* _backBuffer, SoundBuffer* _soundBuffer)
{
	//Background image:
	m_pBackground = _backBuffer->CreateSprite("assets/MenuBack.png");
	m_pPlayBtn = new Button();
	m_pSelectBtn = new Button();
	m_pSettingBtn = new Button();
	m_pQuitBtn = new Button();

	//Play button:
	sf::Sprite* temp = _backBuffer->CreateSprite("widget/playButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(_backBuffer->GetWindowSize().y * 3/ 7));
	m_pPlayBtn->Initialise(temp);

	//Select button:
	temp = _backBuffer->CreateSprite("widget/selectButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(_backBuffer->GetWindowSize().y * 4 / 7));
	m_pSelectBtn->Initialise(temp);

	//Setting button:
	temp = _backBuffer->CreateSprite("widget/settingButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(_backBuffer->GetWindowSize().y * 5 / 7));
	m_pSettingBtn->Initialise(temp);
	
	//Quit button:
	temp = _backBuffer->CreateSprite("widget/quitButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(_backBuffer->GetWindowSize().y * 6 / 7));
	m_pQuitBtn->Initialise(temp);

	//Level list:
	m_levelList = new ListBox();
	m_levelList->Initialise(*_backBuffer, sf::Vector2f(350, 430));

	IniParser::GetInstance().LoadIniFile("assets/Levels/levels.ini");
	if (IniParser::GetInstance().ContainsSection("Level"))
	{
		int count = IniParser::GetInstance().GetValueAsInt("Level", "lCount");
		for (int i = 0; i < count; i++)
		{
			std::string temp = IniParser::GetInstance().GetValueAsString("Level", "l"+std::to_string(i));
			m_levelList->AddString(temp);
		}
	}

	//Background music:
	_soundBuffer->LoadBackgroundMusic("assets/MenuMusic.wav");
	_soundBuffer->SetBGLooping(true);
	return Window::Initialise(*_backBuffer, *_soundBuffer);
}

// DrawContents( BackBuffer& buffer )
//
// Description:	Draws all objects to the screen using the backbuffer provided.
//				
// @param	BackBuffer&		The back buffer to draw all of the objects to.
//
// @return	n/a
//
void MenuWindow::DrawContents(BackBuffer& buffer)
{
	//Background Image
	buffer.Draw(*m_pBackground);
	
	//Buttons:
	m_pPlayBtn->Draw(buffer);
	m_pSelectBtn->Draw(buffer);
	m_pSettingBtn->Draw(buffer);
	m_pQuitBtn->Draw(buffer);
	m_levelList->Draw(buffer);

	//"Level Select"
	buffer.SetFontSize(20);
	buffer.SetColour(sf::Color::Black);
	buffer.DrawRectangle(355, 400, 152, 22);
	buffer.SetColour(sf::Color::White);

	//Level List:
	buffer.DrawTextToWorld("Level Select:", 355, 400);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void MenuWindow::Process(float dT)
{
	if (m_hasMouseClicked && m_mousePressPos != 0)
	{
		
		if (m_pPlayBtn->HandleMouse(m_mousePressPos->x, m_mousePressPos->y))
		{//Play Button:
			if (m_pPlayBtn->HandleMouse(m_mousePos.x, m_mousePos.y))
			{
				Play(0);
			}
		}
		else if (m_pSelectBtn->HandleMouse(m_mousePressPos->x, m_mousePressPos->y))
		{//Select Button:
			if (m_pSelectBtn->HandleMouse(m_mousePos.x, m_mousePos.y))
			{
				Play(static_cast<int>(m_levelList->GetValueAsFloat()));
			}
		}
		else if (m_pSettingBtn->HandleMouse(m_mousePressPos->x, m_mousePressPos->y))
		{//Setting Button:
			if (m_pSettingBtn->HandleMouse(m_mousePos.x, m_mousePos.y))
			{
				Settings();
			}
		}
		else if (m_pQuitBtn->HandleMouse(m_mousePressPos->x, m_mousePressPos->y))
		{//Quit Button:
			if (m_pQuitBtn->HandleMouse(m_mousePos.x, m_mousePos.y))
			{
				Quit();
			}
		}
		else if (m_levelList->HandleMouse(m_mousePressPos->x, m_mousePressPos->y))
		{//Level list:
			//Do nothing
		}

		//Remove mouse location
		delete m_mousePressPos;
		m_mousePressPos = 0;
		m_hasMouseClicked = false;
	}
}

// DoWindowLoop( )
//
// Description:	The main while loop that takes input, processes then draws the frame.
//				
// @param	n/a
//
// @return	bool	if the loop was stopped due to force close.
//
bool MenuWindow::DoWindowLoop()
{
	m_timePassed = 0;
	m_pSoundBuffer->PlayBackgroundMusic();
	m_Clock.restart();

	do {

		float deltaTime = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();
		
		m_pInputHandler->HandleClickEvents(*this);
		m_lag += deltaTime;
		if (m_lag >= sm_frameTime) //Fixing to 60 frames
		{
			Process(sm_frameTime);
			m_lag -= sm_frameTime;
			Draw(*m_pBackBuffer);
		}
		
		m_timePassed += deltaTime;
		m_IsRunning  = !m_HasQuit;
	} while (m_IsRunning);
	return m_HasQuit;
}

// MouseMoved( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseMoved event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
// @Param	const int&	Reference to the position of the mouse on the y-axis.
//
// @Return	n/a
//
void MenuWindow::MouseMoved(const int& _mouseX, const int& _mouseY)
{
	m_mousePos.x = static_cast<float>(_mouseX);
	m_mousePos.y = static_cast<float>(_mouseY);

	m_pPlayBtn->HandleMouse(m_mousePos.x, m_mousePos.y);
	m_pSelectBtn->HandleMouse(m_mousePos.x, m_mousePos.y);
	m_pSettingBtn->HandleMouse(m_mousePos.x, m_mousePos.y);
	m_pQuitBtn->HandleMouse(m_mousePos.x, m_mousePos.y);
	m_levelList->HandleHover(m_mousePos.x, m_mousePos.y);
}

// MousePressed( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseClickPressed event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
// @Param	const int&	Reference to the position of the mouse on the y-axis.
//
// @Return	n/a
//
void MenuWindow::MousePressed(const int& _mouseX, const int& _mouseY)
{
	if (m_mousePressPos == 0)
	{
		m_mousePressPos = new sf::Vector2f();
	}
	m_mousePressPos->x = static_cast<float>(_mouseX);
	m_mousePressPos->y = static_cast<float>(_mouseY);
}

// MouseReleased( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseClickReleased event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
//
// @Return	n/a
//
void MenuWindow::MouseReleased(const int& _mouseX, const int& _mouseY)
{
	if (m_mousePressPos != 0)
	{
		m_hasMouseClicked = true;
		m_mousePos.x = static_cast<float>(_mouseX);
		m_mousePos.y = static_cast<float>(_mouseY);
	}
}

// Play( _level )
//
// Description:	Creates the game after the play button is pressed.
//				
// @Param	int		Level to start at.
//
// @Return	n/a	
//
void MenuWindow::Play(int _level)
{
	Game* _game = &Game::GetInstance(); //Get Game instance
	int ghostCount = 0; //Levels ghosted.
	LogoWindow loadScreen; //Loading screen
	loadScreen.Initialise(m_pBackBuffer, m_pSoundBuffer);
	loadScreen.SetInputEnabled(false);
	
	if (_game->Initialise(*m_pBackBuffer, *m_pSoundBuffer))
	{
		bool isPlayingGame = true;

		do
		{
			//Load current level:
			std::string levelName = m_levelList->GetStringAt(_level);
			m_pBackBuffer->GetWindow()->setTitle(levelName);
			_game->LoadLevel("assets/Levels/" + levelName + ".ini");
			
			//Play level:
			if (_game->DoWindowLoop())
			{
				//Game was force quit...
				Quit(); 
				_game->Deinitialise();
				loadScreen.Deinitialise();
				return;
			}

			//Was the level completed
			if (_game->HasFinished()) 
			{
				_level++; //Next level
				bool ghost = !_game->HasBeenDetected();

				//Unload level
				_game->ClearLevel();

				//If there is another level
				if (!(_level >= m_levelList->GetLimit()))
				{
					//Ghost status text:
					if (ghost)
					{
						loadScreen.AddText("Ghost: YES", sf::Vector2f(50, 50));
						ghostCount++;
					}
					else
					{
						loadScreen.AddText("Ghost: NO", sf::Vector2f(50, 50));
					}

					loadScreen.SetLogo(m_pBackBuffer->CreateSprite("assets/Loading.png"));
					loadScreen.AddText("Loading the next level...", sf::Vector2f(50, 75));
					loadScreen.SetTimeLength(1.0f);
				}
				else //Show end screen
				{
					if (ghost)
						ghostCount++;

					loadScreen.AddText("Ghosts: " + std::to_string(ghostCount) + " / " + std::to_string(m_levelList->GetLimit()), sf::Vector2f(50, 50));
					loadScreen.SetLogo(m_pBackBuffer->CreateSprite("assets/Loading.png"));
					loadScreen.AddText("You Win!!!", sf::Vector2f(50, 75));
					loadScreen.SetTimeLength(2.0f);
				}

				m_pBackBuffer->ResetView();

				//Play loading screen:
				if (loadScreen.DoWindowLoop())
				{
					Quit();
					_game->Deinitialise();
					loadScreen.Deinitialise();
					return;
				}

				loadScreen.ClearTexts();

				if (_level >= m_levelList->GetLimit())
				{
					//return to main menu
					isPlayingGame = false;
				}

			}
			else //return to main menu
			{
				isPlayingGame = false;
			}
		}while (isPlayingGame);
	}

	//Close and restore settings back to main menu:
	_game->Deinitialise();
	m_pSoundBuffer->LoadHeldBackgroundMusic();
	m_pSoundBuffer->StopBackgroundMusic();
	m_pSoundBuffer->PlayBackgroundMusic();
	m_pBackBuffer->GetWindow()->setTitle("Space Shift");
	m_Clock.restart();
}

// Settings( )
//
// Description:	Displays the settings after the controls button is pressed.
//				
// @Param	n/a
//
// @Return	n/a	
//
void MenuWindow::Settings()
{
	MiniWindow popUp;
	sf::Vector2f Pos(m_pBackBuffer->GetWindowSize());
	if (popUp.Initialise(this, sf::Vector2f((Pos.x - 500) / 2, (Pos.y - 600) / 2), Type::SETTING))
	{
		if(popUp.DoWindowLoop())
		{
			Quit(); //Force quit
		}
	}
	m_Clock.restart();
}
