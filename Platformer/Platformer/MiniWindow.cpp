//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: MiniWindow.cpp
// Description   	: Acts as a pop up infront of another window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self include
#include "MiniWindow.h"

//Local includes
#include "ScaleBar.h"
#include "CheckBox.h"
#include "iniParser.h"
#include "Button.h"
#include "Game.h"

//Constructor
MiniWindow::MiniWindow()
	: m_backWindow(0)
	, m_topLeftPos(0, 0), m_size(0, 0), m_mousePress(0, 0)
	, m_hasClicked(false)
	, m_windowType(PAUSE)
	, m_BGBar(0), m_SEBar(0)
	, m_DebugMode(0), m_FullScreenMode(0), m_highscore(0)
	, m_pBtnSettings(0), m_pBtnBack(0), m_pBtnQuit(0)
	, m_GodMode(0), m_SilentShoes(0), m_ShowNoise(0), m_ShowHitboxes(0)
	, m_ShowHearing(0), m_ShowSight(0), m_ShowPatrol(0)
	, isGodMode(false), isSilentShoes(false), isShowingNoise(false), isShowingHitboxes(false)
	, isShowingHearing(false)
{

}

//Destructor
MiniWindow::~MiniWindow()
{
	//Doesn't own m_backWindow
	m_backWindow = 0;

	//Debug:
	if (m_GodMode != 0)
	{
		delete m_GodMode;
		m_GodMode = 0;
	}
	if (m_SilentShoes != 0)
	{
		delete m_SilentShoes;
		m_SilentShoes = 0;
	}
	if (m_ShowNoise != 0)
	{
		delete m_ShowNoise;
		m_ShowNoise = 0;
	}
	if (m_ShowHitboxes != 0)
	{
		delete m_ShowHitboxes;
		m_ShowHitboxes = 0;
	}
	if (m_ShowHearing != 0)
	{
		delete m_ShowHearing;
		m_ShowHearing = 0;
	}
	if (m_ShowSight != 0)
	{
		delete m_ShowSight;
		m_ShowSight = 0;
	}
	if (m_ShowPatrol != 0)
	{
		delete m_ShowPatrol;
		m_ShowPatrol = 0;
	}

	//Settings:
	if (m_BGBar != 0)
	{
		delete m_BGBar;
		m_BGBar = 0;
	}
	if (m_SEBar != 0)
	{
		delete m_SEBar;
		m_SEBar = 0;
	}
	if (m_DebugMode != 0)
	{
		delete m_DebugMode;
		m_DebugMode = 0;
	}
	if (m_FullScreenMode != 0)
	{
		delete m_FullScreenMode;
		m_FullScreenMode = 0;
	}

	//Pause
	if (m_pBtnSettings != 0)
	{
		delete m_pBtnSettings;
		m_pBtnSettings = 0;
	}
	if (m_pBtnBack != 0)
	{
		delete m_pBtnBack;
		m_pBtnBack = 0;
	}
	if (m_pBtnQuit != 0)
	{
		delete m_pBtnQuit;
		m_pBtnQuit = 0;
	}

	IniParser::GetInstance().SaveIniFile();
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
// @return	bool	Status of the initialisation.
//
bool MiniWindow::Initialise(Window* _belowWindow, sf::Vector2f _topLeftPos, Type _type)
{
	m_backWindow = _belowWindow;
	m_topLeftPos = _topLeftPos;
	m_size.x = 500;
	m_size.y = 600;
	IniParser::GetInstance().LoadIniFile("assets/settings.ini");

	/*m_highscore = IniParser::GetInstance().GetValueAsInt("Settings", "HS");*/
	m_windowType = _type;
	switch (m_windowType)
	{
	default:
	case Type::DEBUG:
		InitialiseDebug(*(_belowWindow->GetBackBuffer()));
		break;
	case Type::SETTING:
		InitialiseSettings(*(_belowWindow->GetBackBuffer()), *(_belowWindow->GetSoundBuffer()));
		break;
	case Type::PAUSE:
		InitialisePause(*(_belowWindow->GetBackBuffer()));
		break;
	}
	_belowWindow->GetBackBuffer()->SetFontSize(15);
	return Window::Initialise(*_belowWindow->GetBackBuffer(), *_belowWindow->GetSoundBuffer());
}

// DoWindowLoop( )
//
// Description:	The main while loop that takes input, processes then draws the frame.
//				
// @param	n/a
//
// @return	bool	if the loop was stopped due to force close.
//
bool MiniWindow::DoWindowLoop()
{
	m_timePassed = 0;

	do {
		float deltaTime = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();

		m_pInputHandler->HandleClickEvents(*this);
		m_lag += deltaTime;

		if (m_lag >= sm_frameTime)
		{
			Process(sm_frameTime);
			m_lag -= sm_frameTime;
			Draw(*m_pBackBuffer);
		}

		m_timePassed += deltaTime;
	} while (m_IsRunning);

	m_pSoundBuffer->UpdateVolumeModifier(1.0f); //Return volume back to normal

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
void MiniWindow::DrawContents(BackBuffer& buffer)
{
	m_backWindow->DrawContents(buffer);
	buffer.SetColour(sf::Color(70, 70, 70, static_cast<sf::Uint8>(255 * 0.4f)));
	buffer.DrawRectangle(0, 0, static_cast<float>(m_pBackBuffer->GetWindowSize().x), static_cast<float>(m_pBackBuffer->GetWindowSize().y));
	buffer.SetColour(sf::Color(180, 180, 180, static_cast<sf::Uint8>(255 * 0.8f)));
	buffer.DrawRectangle(m_topLeftPos.x, m_topLeftPos.y, m_size.x, m_size.y);
	buffer.SetColour(sf::Color(70, 70, 70, 255));
	buffer.DrawRectangle(m_topLeftPos.x, m_topLeftPos.y, m_size.x, 24);

	switch (m_windowType)
	{
	case Type::DEBUG:
		DebugDraw(*m_pBackBuffer);
		break;
	case Type::SETTING:
		SettingsDraw(*m_pBackBuffer);
		break;
	case Type::PAUSE:
		PauseDraw(*m_pBackBuffer);
		break;
	default:
		break;
	}
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void MiniWindow::Process(float dT)
{
	if (m_hasClicked)
	{
		bool condX = m_mousePress.x >= m_topLeftPos.x && m_mousePress.x <= m_topLeftPos.x + m_size.x;
		bool condY = m_mousePress.y >= m_topLeftPos.y && m_mousePress.y <= m_topLeftPos.y + m_size.y;
		if (!(condX && condY))
		{
			//Quit the window:
			condX = m_mousePos.x >= m_topLeftPos.x && m_mousePos.x <= m_topLeftPos.x + m_size.x;
			condY = m_mousePos.y >= m_topLeftPos.y && m_mousePos.y <= m_topLeftPos.y + m_size.y;
			if (!(condX && condY))
			{
				m_IsRunning = false;
				if (m_windowType == DEBUG)
				{
					Game* temp = dynamic_cast<Game*>(m_backWindow);
					temp->SetDebugInfo(isGodMode, isSilentShoes, isShowingNoise, isShowingHitboxes, isShowingHearing, isShowingSight, isShowingPatrol);
				}
			}
		}
		else
		{
			//Handle Process:
			switch (m_windowType)
			{
			case DEBUG:
				DebugProcess(dT);
				break;
			case SETTING:
				SettingsProcess(dT);
				break;
			case PAUSE:
				PauseProcess(dT);
				break;
			default:
				break;
			}
		}
		m_hasClicked = false;
	}
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
void MiniWindow::MouseMoved(const int& _mouseX, const int& _mouseY)
{
	if (m_windowType == Type::PAUSE)
	{
		m_pBtnSettings->HandleMouse(static_cast<float>(_mouseX), static_cast<float>(_mouseY));
		m_pBtnBack->HandleMouse(static_cast<float>(_mouseX), static_cast<float>(_mouseY));
		m_pBtnQuit->HandleMouse(static_cast<float>(_mouseX), static_cast<float>(_mouseY));
	}
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
void MiniWindow::MousePressed(const int& _mouseX, const int& _mouseY)
{
	m_mousePress.x = static_cast<float>(_mouseX);
	m_mousePress.y = static_cast<float>(_mouseY);
}

// MouseReleased( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseClickReleased event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
//
// @Return	n/a
//
void MiniWindow::MouseReleased(const int& _mouseX, const int& _mouseY)
{
	m_mousePos.x = static_cast<float>(_mouseX);
	m_mousePos.y = static_cast<float>(_mouseY);

	m_hasClicked = true;
}

// InitialiseSettings( BackBuffer& bBuffer, SoundBuffer& sBuffer )
//
// Description:	Sets up the window's values for use (for the settings type).
//
// @param	BackBuffer&		Pointer to this game's back buffer.
// @param	SoundBuffer*	Pointer to this game's sound buffer.
//
// @return	bool	Status of the initialisation.
//
void MiniWindow::InitialiseSettings(BackBuffer& bBuffer, SoundBuffer& sBuffer)
{
	//Background Volume:
	m_BGBar = new ScaleBar();
	m_BGBar->Initialise(bBuffer, m_topLeftPos.x + 20, m_topLeftPos.y + 55);
	m_BGBar->SetBounds(0, 100);
	m_BGBar->SetIncrement(1);
	m_BGBar->SetValue(static_cast<float>(sBuffer.GetBGVolume()));
	m_BGBar->SetTargetWidth(m_size.x - 40);

	//Sound Effec Volume:
	m_SEBar = new ScaleBar(*m_BGBar);
	m_SEBar->Initialise(bBuffer, m_topLeftPos.x + 20, m_topLeftPos.y + 115);
	m_SEBar->SetValue(static_cast<float>(sBuffer.GetSEVolume()));
	m_SEBar->SetTargetWidth(m_size.x - 40);

	//Debug mode checkbox:
	m_DebugMode = new CheckBox();
	m_DebugMode->Initialise(bBuffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 150);
	m_DebugMode->SetValue(IniParser::GetInstance().GetValueAsBoolean("Settings", "DBMode"));
	
	//FullScreen checkbox:
	m_FullScreenMode = new CheckBox();
	m_FullScreenMode->Initialise(bBuffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 175);
	m_FullScreenMode->SetValue(IniParser::GetInstance().GetValueAsBoolean("Settings", "FSMode"));

}

// InitialisePause( BackBuffer& _buffer)
//
// Description:	Sets up the window's values for use (for the pause type).
//
// @param	BackBuffer&		Pointer to this game's back buffer.
//
// @return	bool	Status of the initialisation.
//
void MiniWindow::InitialisePause(BackBuffer& buffer)
{
	m_backWindow->GetSoundBuffer()->UpdateVolumeModifier(0.3f);

	//Settings button:
	m_pBtnSettings = new Button();
	m_pBtnSettings->Initialise(buffer.CreateSprite("widget/settingButton.png"));
	m_pBtnSettings->SetWorldPosition(sf::Vector2f(m_topLeftPos.x + m_size.x / 2 - 150, m_topLeftPos.y + 50));
	
	//Back button:
	m_pBtnBack = new Button();
	m_pBtnBack->Initialise(buffer.CreateSprite("widget/backButton.png"));
	m_pBtnBack->SetWorldPosition(sf::Vector2f(m_topLeftPos.x + m_size.x / 2 - 150, m_topLeftPos.y + 200));
	
	//Quit button:
	m_pBtnQuit = new Button();
	m_pBtnQuit->Initialise(buffer.CreateSprite("widget/quitButton.png"));
	m_pBtnQuit->SetWorldPosition(sf::Vector2f(m_topLeftPos.x + m_size.x / 2 - 150, m_topLeftPos.y + 350));
}

// SettingsDraw( BackBuffer& buffer )
//
// Description:	Draws all objects to the screen using the backbuffer provided, only for the settings type.
//				
// @param	BackBuffer&		The back buffer to draw all of the objects to.
//
// @return	n/a
//
void MiniWindow::SettingsDraw(BackBuffer& buffer)
{
	buffer.SetFontAlign(Align::Centre);
	buffer.SetColour(sf::Color::Black);
	float x = m_topLeftPos.x + 20;
	float y = m_topLeftPos.y;
	buffer.DrawTextToWorld("Settings", m_topLeftPos.x + m_size.x / 2, m_topLeftPos.y);

	buffer.SetFontAlign(Align::Left);

	buffer.DrawTextToWorld("Background Music: "+ std::to_string(m_pSoundBuffer->GetBGVolume()), x, y + 30);
	m_BGBar->Draw(buffer);

	buffer.DrawTextToWorld("Sound Effects: " + std::to_string(m_pSoundBuffer->GetSEVolume()), x, y + 95);
	m_SEBar->Draw(buffer);

	buffer.DrawTextToWorld("Enable Debug Mode:", x, y + 148);
	m_DebugMode->Draw(buffer);

	buffer.DrawTextToWorld("FullScreen:", x, y + 172);
	m_FullScreenMode->Draw(buffer);

	if(m_DebugMode->GetValueAsBoolean())
		buffer.DrawTextToWorld("Debug menu key: [~]", x, y + 245);
}

// PauseDraw( BackBuffer& buffer )
//
// Description:	Draws all objects to the screen using the backbuffer provided, only for the pause type.
//				
// @param	BackBuffer&		The back buffer to draw all of the objects to.
//
// @return	n/a
//
void MiniWindow::PauseDraw(BackBuffer& buffer)
{
	buffer.SetFontAlign(Align::Centre);
	buffer.SetColour(sf::Color::Black);
	buffer.DrawTextToWorld("Pause", m_topLeftPos.x + m_size.x / 2, m_topLeftPos.y);
	
	//Draw buttons:
	m_pBtnSettings->Draw(buffer);
	m_pBtnBack->Draw(buffer);
	m_pBtnQuit->Draw(buffer);
}

// SettingsProcess( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call (only for Settings type).
//
// @return	n/a
//
void MiniWindow::SettingsProcess(float dT)
{
	//Background Volume
	if (m_BGBar->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		m_pSoundBuffer->UpdateBGVolume(static_cast<unsigned int>(m_BGBar->GetValueAsFloat()));
		IniParser::GetInstance().SetValueAsString("Settings", "BGMusic", std::to_string(static_cast<int>(m_BGBar->GetValueAsFloat())));
	}

	//Sound Effect Volume
	if (m_SEBar->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		m_pSoundBuffer->UpdateSEVolume(static_cast<unsigned int>(m_SEBar->GetValueAsFloat()));
		IniParser::GetInstance().SetValueAsString("Settings", "SEMusic", std::to_string(static_cast<int>(m_SEBar->GetValueAsFloat())));
		m_pSoundBuffer->PlaySoundEffect("assets/gunFire.wav");
	}

	//Debugger mode:
	if (m_DebugMode->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		IniParser::GetInstance().SetValueAsBoolean("Settings", "DBMode", m_DebugMode->GetValueAsBoolean());
	}

	//Fullscreen:
	if (m_FullScreenMode->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		IniParser::GetInstance().SetValueAsBoolean("Settings", "FSMode", m_FullScreenMode->GetValueAsBoolean());
		m_pBackBuffer->SetFullScreen(IniParser::GetInstance().GetValueAsBoolean("Settings", "FSMode"));
	}
}

// PauseProcess( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call (only for Pause type).
//
// @return	n/a
//
void MiniWindow::PauseProcess(float dT)
{
	//Settings button
	if (m_pBtnSettings->HandleMouse(m_mousePress.x, m_mousePress.y))
	{
		if (m_pBtnSettings->HandleMouse(m_mousePos.x, m_mousePos.y))
		{
			MiniWindow popUp;
			sf::Vector2f Pos(m_pBackBuffer->GetWindowSize());
			if (popUp.Initialise(this, sf::Vector2f((Pos.x - 500) / 2, (Pos.y - 600) / 2), Type::SETTING))
			{
				if (popUp.DoWindowLoop())
				{
					Quit();
				}
			}
		}
	}

	//Back button
	if (m_pBtnBack->HandleMouse(m_mousePress.x, m_mousePress.y))
	{
		if (m_pBtnBack->HandleMouse(m_mousePos.x, m_mousePos.y))
		{
			m_backWindow->SetRunning(false);
			m_IsRunning = false;
		}
	}

	//Quit button
	if (m_pBtnQuit->HandleMouse(m_mousePress.x, m_mousePress.y))
	{
		if (m_pBtnQuit->HandleMouse(m_mousePos.x, m_mousePos.y))
		{
			Quit();
		}
	}
}

// InitialiseDebug( BackBuffer& bBuffer, SoundBuffer& sBuffer )
//
// Description:	Sets up the window's values for use (for the debug type).
//
// @param	BackBuffer&		Pointer to this game's back buffer.
//
// @return	bool	Status of the initialisation.
//
void MiniWindow::InitialiseDebug(BackBuffer& buffer)
{
	m_backWindow->GetSoundBuffer()->UpdateVolumeModifier(0.3f);
	Game* temp = dynamic_cast<Game*>(m_backWindow);
	temp->GetDebugInfo(isGodMode, isSilentShoes, isShowingNoise, isShowingHitboxes, isShowingHearing, isShowingSight, isShowingPatrol);
	
	//Toggle God mode checkbox:
	m_GodMode = new CheckBox();
	m_GodMode->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 36);
	m_GodMode->SetValue(isGodMode);

	//Toggle Silent Shoes checkbox:
	m_SilentShoes = new CheckBox();
	m_SilentShoes->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 66);
	m_SilentShoes->SetValue(isSilentShoes);

	//Toggle Show noise checkbox:
	m_ShowNoise = new CheckBox();
	m_ShowNoise->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 96);
	m_ShowNoise->SetValue(isShowingNoise);

	//Toggle Show Hitboxes checkbox:
	m_ShowHitboxes = new CheckBox();
	m_ShowHitboxes->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 126);
	m_ShowHitboxes->SetValue(isShowingHitboxes);

	//Toggle Show hearing checkbox:
	m_ShowHearing = new CheckBox();
	m_ShowHearing->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 156);
	m_ShowHearing->SetValue(isShowingHearing);

	//Toggle Show sight checkbox:
	m_ShowSight = new CheckBox();
	m_ShowSight->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 186);
	m_ShowSight->SetValue(isShowingSight);

	//Toggle Show patrol checkbox:
	m_ShowPatrol = new CheckBox();
	m_ShowPatrol->Initialise(buffer, m_topLeftPos.x + (m_size.x / 2) + 30, m_topLeftPos.y + 216);
	m_ShowPatrol->SetValue(isShowingPatrol);
}

// PauseDraw( BackBuffer& buffer )
//
// Description:	Draws all objects to the screen using the backbuffer provided, only for the pause type.
//				
// @param	BackBuffer&		The back buffer to draw all of the objects to.
//
// @return	n/a
//
void MiniWindow::DebugDraw(BackBuffer& buffer)
{
	buffer.SetFontAlign(Align::Centre);
	buffer.SetColour(sf::Color::Black);
	buffer.DrawTextToWorld("Debug", m_topLeftPos.x + m_size.x / 2, m_topLeftPos.y);

	float x = m_topLeftPos.x + 20;
	float y = m_topLeftPos.y;

	buffer.SetColour(sf::Color::Black);
	buffer.SetFontAlign(Align::Left);

	buffer.DrawTextToWorld("God Mode:", x, y + 30);
	m_GodMode->Draw(buffer);

	buffer.DrawTextToWorld("Silent Shoes:", x, y + 60);
	m_SilentShoes->Draw(buffer);

	buffer.DrawTextToWorld("Show Noise:", x, y + 90);
	m_ShowNoise->Draw(buffer);

	buffer.DrawTextToWorld("Show Hitboxes:", x, y + 120);
	m_ShowHitboxes->Draw(buffer);

	buffer.DrawTextToWorld("Show Hearing:", x, y + 150);
	m_ShowHearing->Draw(buffer);

	buffer.DrawTextToWorld("Show Sight:", x, y + 180);
	m_ShowSight->Draw(buffer);

	buffer.DrawTextToWorld("Show Patrol:", x, y + 210);
	m_ShowPatrol->Draw(buffer);
}

// DebugProcess( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call (only for Debug type).
//
// @return	n/a
//
void MiniWindow::DebugProcess(float dT)
{
	//Toggle God mode checkbox:
	if (m_GodMode->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isGodMode = m_GodMode->GetValueAsBoolean();
	}

	//Toggle Silent Shoes checkbox:
	if (m_SilentShoes->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isSilentShoes = m_SilentShoes->GetValueAsBoolean();
	}

	//Toggle Show Noise checkbox:
	if (m_ShowNoise->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isShowingNoise = m_ShowNoise->GetValueAsBoolean();
	}

	//Toggle Show Hitboxes checkbox:
	if (m_ShowHitboxes->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isShowingHitboxes = m_ShowHitboxes->GetValueAsBoolean();
	}

	//Toggle Show Hearing checkbox:
	if (m_ShowHearing->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isShowingHearing = m_ShowHearing->GetValueAsBoolean();
	}

	//Toggle Show Sight checkbox:
	if (m_ShowSight->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isShowingSight = m_ShowSight->GetValueAsBoolean();
	}

	//Toggle Show Patrol checkbox:
	if (m_ShowPatrol->HandleMouse(m_mousePos.x, m_mousePos.y))
	{
		isShowingPatrol = m_ShowPatrol->GetValueAsBoolean();
	}
}