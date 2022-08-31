//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Playformer.cpp
// Description   	: Implementation of Playformer.h first object to be called.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Includes
#include "Platformer.h"

//Local Includes
#include "BackBuffer.h"
#include "SoundBuffer.h"
#include "Game.h"
#include "iniParser.h"
#include "MenuWindow.h"
#include "LogoWindow.h"
#include "resource.h"

//Library Includes
#include <ctime>

//Constructor
Platformer::Platformer()
{

}

//Destructor
Platformer::~Platformer()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pSoundBuffer;
	m_pSoundBuffer = 0;
}

// Initialise( )
//
// Description:	Sets up the Main's values for use. This function MUST
//				be called before using any of the other functions.
//
// @return	boolean		Status of the initialisation.
//
bool Platformer::Initialise()
{
	m_pBackBuffer = new BackBuffer();
	m_pSoundBuffer = new SoundBuffer();

	m_pBackBuffer->Initialise(1280, 720);
	m_pBackBuffer->LoadFont("assets/Hack-Regular.ttf");

	m_pSoundBuffer->Initialise();

	//If .ini file does not exist:
	if (!IniParser::GetInstance().LoadIniFile("assets/settings.ini"))
	{
		//Create file
		IniParser::GetInstance().CreateIniFile("assets/settings.ini");
		IniParser::GetInstance().AddNewValue("Settings", "BGMusic", "100");
		IniParser::GetInstance().AddNewValue("Settings", "SEMusic", "100");
		IniParser::GetInstance().AddNewValue("Settings", "DBMode", "false");
		IniParser::GetInstance().AddNewValue("Settings", "FSMode", "false");
		IniParser::GetInstance().SaveIniFile();
	}

	m_pBackBuffer->SetFullScreen(IniParser::GetInstance().GetValueAsBoolean("Settings", "FSMode"));
	return true;
}

// Start( )
//
// Description:	Starts the game.
//
// @param	n/a
//
// @return	n/a
//
void Platformer::Start()
{
	srand(static_cast<unsigned int>(std::time(0))); //Set the Seed

	LogoWindow logo;
	if (logo.Initialise(m_pBackBuffer, m_pSoundBuffer))
	{
		logo.SetLogo(m_pBackBuffer->CreateSprite("assets/MDS.png"));
		logo.SetTimeLength(5.0f);
		if (logo.DoWindowLoop())
		{
			return;
		}

		logo.SetLogo(m_pBackBuffer->CreateSprite("assets/CorrosiveGames.png"));
		logo.SetTimeLength(5.0f);
		logo.SetBackgroundColour(sf::Color(185, 103, 255));
		if (logo.DoWindowLoop())
		{
			return;
		}
	}

	MenuWindow menu;
	if (menu.Initialise(m_pBackBuffer, m_pSoundBuffer))
	{
		menu.DoWindowLoop();
	}
}