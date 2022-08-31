//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : Game.cpp
// Description   : Define the game class to be used in the core of the game. 
//				   Handles the game loop.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Library Includes
#include <ctime>

//Self Includes
#include "Game.h"

//Local Includes
#include "BackBuffer.h"
#include "SoundBuffer.h"
#include "InputHandler.h"
#include "IniParser.h"
#include "Platform.h"
#include "Player.h"
#include "Box.h"
#include "ButtonObject.h"
#include "Door.h"
#include "Guard.h"
#include "Bullet.h"
#include "TextObject.h"
#include "NoiseSystem.h"
#include "Background.h"
#include "End.h"
#include "MiniWindow.h"

//Static variables:
Game* Game::sm_pInstance = 0;

//Constructor
Game::Game()
	: m_width(0), m_height(0), m_WorldBox(0, 0, 0, 0), m_portalReached(false)
	, m_fade(0, 0, 0, 255), m_alpha(255)
	, m_cameraPos(0, 0)
	, m_pPlayer(0), m_pEnd(0), m_pBackground(0)
	, m_levelFileLoc("")
	, m_isGodMode(false), m_isSilentShoes(false), m_isShowingNoise(false)
	, m_isShowingHitboxes(false), m_isShowingHearing(false), m_isShowingSight(false)
	, m_isShowingPatrol(false), m_hasBeenDetected(false)
{	  
	
}

//Destructor
Game::~Game()
{
	
}

// GetInstance( )
// 
// Description: Calls the constructor of the Iniparser if it doesn't exist.
//
// @returns	 IniParser& 	Reference to the singleton Ini Parser.
//
Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}
	return *sm_pInstance;
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
bool Game::Initialise(BackBuffer& _backBuffer, SoundBuffer& _soundBuffer)
{
	m_width = _backBuffer.GetWindowSize().x;
	m_height = _backBuffer.GetWindowSize().y;
	m_WorldBox.top = -50;
	m_WorldBox.left = -50;
	m_WorldBox.width = m_width + 50;
	m_WorldBox.height = m_height + 50;

	_backBuffer.SetFontSize(15);
	srand(static_cast<unsigned>(time(0)));
	NoiseSystem::GetInstance().UpdateSoundBuffer(_soundBuffer);
	_soundBuffer.LoadBackgroundMusic("assets/GameMusic.wav");
	_soundBuffer.SetBGLooping(true);
	return Window::Initialise(_backBuffer, _soundBuffer);
}

// Deinitialise( )
//
// Description:	Releases any memory the window's values for use.
//
// @param	n/a
//
// @return	bool	Status of the deinitialisation.
//
bool Game::Deinitialise()
{
	if (m_levelFileLoc != "")
	{
		ClearLevel();
		m_levelFileLoc = "";
	}

	return Window::Deinitialise();
}

// LoadLevel( std::string )
//
// Description:	Destructs the data from an external level file, and places
//				entities into the scene.
//
// @param	std::string		file loaction of the level.
//
// @return	n/a
//
void Game::LoadLevel(std::string levelLoc)
{
	//Set view size:
	m_pBackBuffer->UpdateViewSize(sf::Vector2f(static_cast<float>(m_width * 0.75f), static_cast<float>(m_height * 0.75f)));

	//Update presets:
	m_levelFileLoc = levelLoc;
	IniParser::GetInstance().LoadIniFile(levelLoc);
	m_IsRunning = true;
	m_HasQuit = false;

	//Construct new background
	m_pBackground = new Background();
	m_pBackground->Initialise(m_WorldBox.width, m_WorldBox.height);

	//Player
	if (IniParser::GetInstance().ContainsSection("PlayerSpawn"))
	{
		m_pPlayer = new Player();
		m_pPlayer->Initialise(*m_pBackBuffer, IniParser::GetInstance().GetValueAsString("PlayerSpawn", "spawn"));
	}

	//End
	if (IniParser::GetInstance().ContainsSection("EndPortal"))
	{
		m_pEnd = new End();
		m_pEnd->Initialise(*m_pBackBuffer, IniParser::GetInstance().GetValueAsString("EndPortal", "end"));
	}

	//Platforms
	if (IniParser::GetInstance().ContainsSection("Platforms"))
	{
		unsigned int platCount = IniParser::GetInstance().GetValueAsInt("Platforms", "pCount");
		for (unsigned int i = 0; i < platCount; ++i)
		{
			Platform* p = new Platform();
			p->Initialise(IniParser::GetInstance().GetValueAsString("Platforms", "p" + std::to_string(i)));
			m_platforms.push_back(p);
		}
	}

	//Boxes
	if (IniParser::GetInstance().ContainsSection("Boxes"))
	{
		unsigned int boxCount = IniParser::GetInstance().GetValueAsInt("Boxes", "BCount");
		for (unsigned int i = 0; i < boxCount; ++i)
		{
			Box* b = new Box();
			b->Initialise(*m_pBackBuffer, IniParser::GetInstance().GetValueAsString("Boxes", "B" + std::to_string(i)));
			m_boxes.push_back(b);
		}
	}

	//Doors
	if (IniParser::GetInstance().ContainsSection("Doors"))
	{
		unsigned int boxCount = IniParser::GetInstance().GetValueAsInt("Doors", "dCount");
		for (unsigned int i = 0; i < boxCount; ++i)
		{
			Door* d = new Door();
			d->Initialise(IniParser::GetInstance().GetValueAsString("Doors", "dRect" + std::to_string(i)));
			d->SetOpen(IniParser::GetInstance().GetValueAsBoolean("Doors", "dOpen" + std::to_string(i)));
			m_doors.push_back(d);

		}
	}

	//Button objects
	if (IniParser::GetInstance().ContainsSection("Buttons"))
	{
		unsigned int boxCount = IniParser::GetInstance().GetValueAsInt("Buttons", "bCount");
		for (unsigned int i = 0; i < boxCount; ++i)
		{
			ButtonObject* b = new ButtonObject();
			b->Initialise(*m_pBackBuffer, IniParser::GetInstance().GetValueAsString("Buttons", "b" + std::to_string(i)));
			m_buttons.push_back(b);

			//Link button to doors:
			if (IniParser::GetInstance().ContainsSection("b" + std::to_string(i)))
			{
				int linkCount = IniParser::GetInstance().GetValueAsInt("b" + std::to_string(i), "linkCount");
				for (int j = 0; j < linkCount; j++)
				{
					int index = IniParser::GetInstance().GetValueAsInt("b" + std::to_string(i), "L" + std::to_string(j));
					b->AddLink(m_doors.at(index));
				}
			}
		}
	}

	//Guards
	if (IniParser::GetInstance().ContainsSection("Guards"))
	{
		unsigned int guardCount = IniParser::GetInstance().GetValueAsInt("Guards", "gCount");
		for (unsigned int i = 0; i < guardCount; i++)
		{
			Guard* g = new Guard();
			g->Initialise(*m_pBackBuffer, IniParser::GetInstance().GetValueAsString("Guards", "g" + std::to_string(i)));
			m_guards.push_back(g);
		}
	}

	//Text Objects
	if (IniParser::GetInstance().ContainsSection("Text"))
	{
		unsigned int tCount = IniParser::GetInstance().GetValueAsInt("Text", "tCount");
		for (unsigned int i = 0; i < tCount; i++)
		{
			std::string text = IniParser::GetInstance().GetValueAsString("Text", "tText"+ std::to_string(i));
			int size = IniParser::GetInstance().GetValueAsInt("Text", "tSize" + std::to_string(i));
			std::string loc = IniParser::GetInstance().GetValueAsString("Text", "tLoc" + std::to_string(i));
			TextObject* temp = new TextObject();
			temp->Initialise(text, loc, size);
			m_TextObjects.push_back(temp);
		}
	}
	m_cameraPos = m_pPlayer->GetCenterPosition();

	//Play Background Music
	m_pSoundBuffer->PlayBackgroundMusic();
}

// RestartLevel( )
//
// Description:	Deconstruct the level and reload the level.
//
// @return	n/a
//
void Game::RestartLevel()
{
	if (m_levelFileLoc != "")
	{
		std::string file = m_levelFileLoc;
		ClearLevel();
		LoadLevel(file);
	}
}

// ClearLevel( )
//
// Description:	Deconstruct the level.
//
// @return	n/a
//
void Game::ClearLevel()
{
	NoiseSystem::GetInstance().Clear();
	m_levelFileLoc = "";

	m_jumpDelay = 0;

	if (m_pBackground != 0)
	{
		delete m_pBackground;
		m_pBackground = 0;
	}
	
	m_pBackBuffer->ResetView(); 
	m_portalReached = false;
	m_hasBeenDetected = false;

	//Iterators for Level Entities:
	std::vector<Platform*>::iterator pIter = m_platforms.begin();
	std::vector<Box*>::iterator bIter = m_boxes.begin();
	std::vector<Door*>::iterator dIter = m_doors.begin();
	std::vector<ButtonObject*>::iterator BIter = m_buttons.begin();
	std::vector<Guard*>::iterator gIter = m_guards.begin();
	std::vector<Bullet*>::iterator bullIter = m_bullets.begin();
	std::vector<TextObject*>::iterator textIter = m_TextObjects.begin();

	//Player
	if (m_pPlayer != 0)
	{
		delete m_pPlayer;
		m_pPlayer = 0;
	}

	//End
	if (m_pEnd != 0)
	{
		delete m_pEnd;
		m_pEnd = 0;
	}

	//Platforms
	while (pIter != m_platforms.end())
	{
		delete * pIter;
		pIter = m_platforms.erase(pIter);
	}

	//Boxes
	while (bIter != m_boxes.end())
	{
		delete * bIter;
		bIter = m_boxes.erase(bIter);
	}

	//Doors
	while (dIter != m_doors.end())
	{
		delete * dIter;
		dIter = m_doors.erase(dIter);
	}

	//Button Objects
	while (BIter != m_buttons.end())
	{
		delete * BIter;
		BIter = m_buttons.erase(BIter);
	}

	//Guards
	while (gIter != m_guards.end())
	{
		delete * gIter;
		gIter = m_guards.erase(gIter);
	}

	//Bullets
	while (bullIter != m_bullets.end())
	{
		delete * bullIter;
		bullIter = m_bullets.erase(bullIter);
	}

	//Text Objects
	while (textIter != m_TextObjects.end())
	{
		delete * textIter;
		textIter = m_TextObjects.erase(textIter);
	}

	//Pause music
	m_pSoundBuffer->PauseBackgroundMusic();
}

// DoWindowLoop( )
//
// Description:	The main while loop that takes input, processes then draws the frame.
//				
// @param	n/a
//
// @return	bool	if the loop was stopped due to force close.
//
bool Game::DoWindowLoop()
{
	m_Clock.restart(); //Clean up clock.
	do
	{
		float deltaTime = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();

		m_pInputHandler->HandleGameInput(*this);

		m_lag += deltaTime;
		if (m_lag >= sm_frameTime)
		{
			Process(sm_frameTime);
			m_lag -= sm_frameTime;
			Draw(*m_pBackBuffer);
		}
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
void Game::DrawContents(BackBuffer& buffer)
{
	//Background
	m_pBackground->Draw(buffer);

	//Barriers
	buffer.SetColour(sf::Color::White);
	buffer.DrawRectangle(static_cast<float>(m_WorldBox.left), static_cast<float>(m_WorldBox.top), static_cast<float>(m_WorldBox.width), 1.0f);
	buffer.DrawRectangle(static_cast<float>(m_WorldBox.left), static_cast<float>(m_WorldBox.top), 1.0f, static_cast<float>(m_WorldBox.height));
	buffer.DrawRectangle(static_cast<float>(m_WorldBox.left + m_WorldBox.width), static_cast<float>(m_WorldBox.top), 1.0f, static_cast<float>(m_WorldBox.height));
	
	buffer.SetColour(sf::Color::Red);
	buffer.DrawRectangle(static_cast<float>(m_WorldBox.left), static_cast<float>(m_WorldBox.top + m_WorldBox.height), static_cast<float>(m_WorldBox.width), 1.0f);

	//Platforms
	for (Platform* p : m_platforms)
	{
		if (m_isShowingHitboxes) //Debug
		{
			p->DrawHitboxes(buffer);
		}
		p->Draw(buffer);
	}

	//Doors
	for (Door* d : m_doors)
	{
		if (m_isShowingHitboxes && !d->IsOpen()) //Debug
		{
			d->DrawHitboxes(buffer);
		}
		d->Draw(buffer);
	}

	//Text Objects
	for (TextObject* t : m_TextObjects)
	{
		t->Draw(buffer);
	}

	//Button Objects
	for (ButtonObject* b : m_buttons)
	{
		b->Draw(buffer);
	}

	//Player
	if (m_pPlayer != 0)
	{
		if (m_isShowingHitboxes) //Debug
		{
			buffer.SetColour(sf::Color::Green);
			buffer.DrawRectangle(static_cast<float>(m_pPlayer->GetHitBox().left), static_cast<float>(m_pPlayer->GetHitBox().top), static_cast<float>(m_pPlayer->GetHitBox().width), static_cast<float>(m_pPlayer->GetHitBox().height));
		}
		m_pPlayer->Draw(buffer);
	}

	//End
	if (m_pEnd != 0)
	{
		if (m_isShowingHitboxes) //Debug
		{
			buffer.SetColour(sf::Color::Green);
			buffer.DrawRectangle(static_cast<float>(m_pEnd->GetHitBox().left), static_cast<float>(m_pEnd->GetHitBox().top), static_cast<float>(m_pEnd->GetHitBox().width), static_cast<float>(m_pEnd->GetHitBox().height));
		}
		m_pEnd->Draw(buffer);
	}

	//Guard
	for (Guard* g : m_guards)
	{
		if (m_isShowingHitboxes) //Debug
		{
			buffer.SetColour(sf::Color::Green);
			buffer.DrawRectangle(static_cast<float>(g->GetHitBox().left), static_cast<float>(g->GetHitBox().top), static_cast<float>(g->GetHitBox().width), static_cast<float>(g->GetHitBox().height));
		}

		if (m_isShowingSight) //Debug
		{
			g->DrawSight(buffer);
		}

		if (m_isShowingHearing) //Debug
		{
			g->DrawHearing(buffer);
		}

		if (m_isShowingPatrol) //Debug
		{
			g->DrawPatrol(buffer);
		}

		g->Draw(buffer);
	}

	//Box
	for (Box* b : m_boxes)
	{
		b->Draw(buffer);
	}

	//Bullets
	for (Bullet* b : m_bullets)
	{
		b->Draw(buffer);
	}

	//Overlay Text:
	if (!m_pPlayer->EmptyHanded())
	{
		buffer.SetColour(sf::Color::Yellow);
		buffer.DrawTextToView("Inventory: A box.", 30, 30);
		buffer.SetColour(sf::Color::Black);
	}

	if (m_isShowingNoise) //Debug
	{
		NoiseSystem::GetInstance().Draw(buffer);
	}

	//Background Fade
	buffer.SetColour(m_fade);
	buffer.DrawRectangle(0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height));

	//Overlay Text:
	if (m_pPlayer->IsPlayerDying()) 
	{
		buffer.SetColour(sf::Color::Red);
		buffer.ResetView();
		buffer.SetFontSize(25);
		buffer.DrawTextToView("Press R to restart!", static_cast<float>(m_width / 2.0f - 125), static_cast<float>(m_height / 2.0f - 25));
	}

	//Reset colour
	buffer.SetColour(sf::Color::Black);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Game::Process(float dT)
{
	//LERP Camera position to player
	m_cameraPos.x += static_cast<float>(0.1 * (m_pPlayer->GetCenterPosition().x - m_cameraPos.x));
	m_cameraPos.y += static_cast<float>(0.1 * (m_pPlayer->GetCenterPosition().y - m_cameraPos.y));
	m_pBackBuffer->UpdateViewCenter(m_cameraPos);

	//Process 2D sound effect volume
	m_pSoundBuffer->Process(m_pPlayer->GetCenterPosition());

	//Prioritise Fade effect (at the start or end of a level)
	if (m_portalReached) //Out
	{
		m_alpha += 300.0f * dT;
		m_fade = sf::Color(0, 0, 0, static_cast<int>(m_alpha));
		m_IsRunning = (m_alpha < 255);
		return; //No other Processing
	}
	else if(m_alpha > 0) //In
	{
		m_alpha -= 300.0f * dT;
		m_fade = sf::Color(0, 0, 0, static_cast<int>(m_alpha));
		return; //No other Processing
	}

	//Player Process
	PlayerProcess(dT);

	//End Process
	m_pEnd->Process(dT);

	//Background Process
	m_pBackground->Process(m_pPlayer->GetCenterPosition());

	//Overlap with player vs. end check:
	if (m_pEnd->IsCollidingWith(m_pPlayer->GetHitBox()))
	{
		m_portalReached = true;
		return; //No other Processing
	}

	//Guard Process
	GuardProcess(dT);

	//NoiseSystem Process
	NoiseSystem::GetInstance().Process(dT);

	//Boxes Handling
	std::vector<Box*>::iterator bIter = m_boxes.begin();
	while (bIter != m_boxes.end())
	{
		bool cond3 = false;
		Platform* platform = 0;

		//Handle overlap with box vs. platform/door
		for (unsigned int i = 0; i < (m_platforms.size() + m_doors.size()); i++) //Platforms
		{
			//Get next platform:
			if (i < m_platforms.size())
			{
				platform = m_platforms.at(i);
			}
			else if (!m_doors.at(i - m_platforms.size())->IsOpen())
			{
				platform = m_doors.at(i - m_platforms.size());
			}
			else
			{
				platform = 0;
			}

			if (platform != 0)
			{
				if (platform->IsBelowOf((*bIter)->GetHitBox()))
				{
					cond3 = true;
					break;
				}
			}
		}

		(*bIter)->SetGrounded(cond3);

		//Boxes process
		(*bIter)->Process(dT);

		//Check if box is still within bounds
		if ((*bIter)->GetHitBox().top < m_WorldBox.top || (*bIter)->GetHitBox().top > m_WorldBox.height)
		{
			delete * bIter;
			bIter = m_boxes.erase(bIter);
		}
		else
		{
			bIter++;
		}
	}

	//handle overlap button vs. player
	for (unsigned int i = 0; i < m_buttons.size(); i++)
	{
		bool trigger = m_buttons.at(i)->IsBelowOf(m_pPlayer->GetHitBox());
		
		if (!trigger) //Check if any boxes are colliding
		{
			for (unsigned int j = 0; j < m_boxes.size(); j++)
			{
				if (m_buttons.at(i)->IsBelowOf(m_boxes.at(j)->GetHitBox()))
				{
					trigger = true;
					break;
				}
			}
		}
		m_buttons.at(i)->SetTriggered(trigger);
	}

	//Bullet Handle
	std::vector<Bullet*>::iterator iter = m_bullets.begin();
	while(iter != m_bullets.end())
	{
		bool BulletIsAlive = true;
		//Bullet process
		(*iter)->Process(dT);

		//Handle overlap bullet vs. player
		if ((*iter)->IsCollidingWith(m_pPlayer->GetHitBox()))
		{
			delete * iter;
			iter = m_bullets.erase(iter);
			if (!m_isGodMode)
			{
				m_pPlayer->KillPlayer();
				m_pSoundBuffer->PauseBackgroundMusic();
			}
			BulletIsAlive = false;
		}

		//Handle overlap bullet vs. Platform
		if(BulletIsAlive) 
		{
			for (Platform* p : m_platforms)
			{
				if (p->IsCollidingWith((*iter)->GetHitBox()))
				{
					delete * iter;
					iter = m_bullets.erase(iter);
					BulletIsAlive = false;
					break;
				}
			}		
		}

		//Handle overlap bullet vs. Door
		if (BulletIsAlive)
		{
			for (Door* d : m_doors)
			{
				if (d->IsCollidingWith((*iter)->GetHitBox()))
				{
					delete * iter;
					iter = m_bullets.erase(iter);
					BulletIsAlive = false;
					break;
				}
			}
		}

		//Out of Bounds
		if (BulletIsAlive)
		{
			if ((*iter)->GetHitBox().left < m_WorldBox.left || (*iter)->GetHitBox().left > m_WorldBox.width)
			{
				delete * iter;
				iter = m_bullets.erase(iter);
				BulletIsAlive = false;
				break;
			}
		}

		if (BulletIsAlive)
		{
			iter++; //Next bullet
		}
	}
	
	//Apply gravity to entities.
	ApplyGravity(dT);
}

// PlayerProcess( float dT )
//
// Description:	Handles specific case for the player.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Game::PlayerProcess(float dT)
{
	if (m_pPlayer != 0) //Player exists
	{
		bool cond1 = false;

		//Player process
		m_pPlayer->Process(dT);

		bool xCond = m_pPlayer->GetHitBox().left < m_WorldBox.left || m_pPlayer->GetHitBox().left > m_WorldBox.width;
		bool yCond = m_pPlayer->GetHitBox().top < m_WorldBox.top || m_pPlayer->GetHitBox().top > m_WorldBox.height;

		//Out of bounds check:
		if ((xCond || yCond))
		{
			m_pPlayer->UnProcessX();
			m_pPlayer->UnProcessY();
			if (m_pPlayer->GetHitBox().top + m_pPlayer->GetHitBox().height > m_WorldBox.height)
			{
				if (!m_pPlayer->IsPlayerDying())
				{
					m_pPlayer->KillPlayer();
					m_pSoundBuffer->PauseBackgroundMusic();
				}
				m_pPlayer->SetGrounded(true);
			}
				
			return; //No more processing
		}

		//Handle overlap with Player vs. Platform/Door
		Platform* platform = 0;
		for (unsigned int i = 0; i < (m_platforms.size() + m_doors.size()); i++) //Platforms
		{
			//Get next platform:
			if (i < m_platforms.size())
			{
				platform = m_platforms.at(i);
			}
			else if(!m_doors.at(i - m_platforms.size())->IsOpen())
			{
				platform = m_doors.at(i - m_platforms.size());
			}
			else
			{
				platform = 0;
			}

			if (platform != 0) //platform exists
			{
				//Test for complete Overlap:
				if (platform->IsCollidingWith(m_pPlayer->GetHitBox()))
				{
					if (platform->GetCollider(m_pPlayer->GetHitBox()) == Entity::ColliderType::NORTH)
					{ //Player landed on platform/door
						m_pPlayer->UnProcessY(); //Undo the y axis
						cond1 = true; //Set grounded
					}
					else if (platform->GetCollider(m_pPlayer->GetHitBox()) == Entity::ColliderType::SOUTH)
					{ //Player hit their head
						m_pPlayer->UnProcessY(); //Undo the y axis
					}
				}
		
				//Double check for edge Overlap:
				Entity::ColliderType collider = platform->GetCollider(m_pPlayer->GetHitBox());
				switch (collider)
				{
				default:
				case Entity::ColliderType::NONE:
					break; //the switch
				case Entity::ColliderType::NORTH:
					cond1 = true; //Set grounded
					break; //the switch
				case Entity::ColliderType::EAST:
					m_pPlayer->UnProcessX(); //Undo the x axis
					if (m_pPlayer->IsBelowOf(platform->GetHitBox()))
					{ //has also hit their head
						m_pPlayer->UnProcessY(); //Undo the y axis
						m_pPlayer->SetVelocity(sf::Vector2f(0, 0)); //stop upwards velocity
					}
					m_pPlayer->SetTouchWall(true);
					break; //the switch
				case Entity::ColliderType::WEST:
					m_pPlayer->UnProcessX(); //Undo the x axis
					if (m_pPlayer->IsBelowOf(platform->GetHitBox()))
					{
						m_pPlayer->UnProcessY(); //Undo the y axis
						m_pPlayer->SetVelocity(sf::Vector2f(0, 0)); //stop upwards velocity
					}
					m_pPlayer->SetTouchWall(true);
					break; //the switch
				case Entity::ColliderType::SOUTH:
					//Keep pushing the player away:
					while (platform->GetCollider(m_pPlayer->GetHitBox()) == Entity::ColliderType::SOUTH)
					{
						m_pPlayer->PushPlayer(sf::Vector2f(0, 1));
					}
					m_pPlayer->SetVelocity(sf::Vector2f(0, 0)); // stop upwards velocity
					break; //the switch
				}
			}
		}
		//Set grounded
		m_pPlayer->SetGrounded(cond1);
	}
}

// GuardProcess( float dT )
//
// Description:	Handles specific case for each guard.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Game::GuardProcess(float dT)
{
	//For each guard
	for (Guard* g : m_guards)
	{
		g->Process(dT);
		Platform* collision = 0;
		bool cond1 = false; //Grounded
		bool cond2 = false; //Collided with wall

		//Check if overlap with Platforms
		for (unsigned int j = 0; j < m_platforms.size(); j++)
		{
			if (m_platforms.at(j)->IsCollidingWith(g->GetHitBox()))
			{
				g->UnProcess();
				collision = m_platforms.at(j);
				break;
			}
		}
		//Check if overlap with Doors
		if (collision == 0)
		{
			for (unsigned int j = 0; j < m_doors.size(); j++)
			{
				if (m_doors.at(j)->IsCollidingWith(g->GetHitBox()))
				{
					g->UnProcess();

					//Push guard upwards (out of the door)
					while (m_doors.at(j)->IsCollidingWith(g->GetHitBox()))
					{
						g->PushGuard(sf::Vector2f(0, -5));
					} 

					collision = m_doors.at(j);
					break;
				}
			}
		}

		//Set guard grounded
		g->SetGrounded((collision != 0), collision);

		if (collision != 0) //Overlap Existed
		{
			g->Halt();
		}

		//Double check collision (for edge overlap):
		if (collision == 0)
		{
			for (unsigned int j = 0; j < m_platforms.size(); j++) //Platforms
			{
				if (m_platforms.at(j)->IsBelowOf(g->GetHitBox()))
				{
					collision = m_platforms.at(j);
					break;
				}
			}
			if (collision == 0) //Doors
			{
				for (unsigned int j = 0; j < m_doors.size(); j++)
				{
					if (m_doors.at(j)->IsBelowOf(g->GetHitBox()))
					{
						collision = m_doors.at(j);
						break;
					}
				}
			}
			//Set guard grounded
			g->SetGrounded((collision != 0), collision);
		}

		//Update Vision
		if (!m_pPlayer->IsPlayerDying())
		{
			//Handle if the player has been detected
			if (g->IsWithinVision(m_pPlayer->GetHitBox()))
			{
				g->Detected(m_pPlayer);
				m_hasBeenDetected = true;
			}
		}
		else //player is dead, calm down.
		{
			g->CalmDown();
		}
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
void Game::MouseMoved(const int& _mouseX, const int& _mouseY)
{
	m_mousePos.x = static_cast<float>(_mouseX);
	m_mousePos.y = static_cast<float>(_mouseY);
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
void Game::MousePressed(const int& _mouseX, const int& _mouseY)
{
	m_mousePos.x = static_cast<float>(_mouseX);
	m_mousePos.y = static_cast<float>(_mouseY);
}

// MouseReleased( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseClickReleased event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
//
// @Return	n/a
//
void Game::MouseReleased(const int& _mouseX, const int& _mouseY)
{
	m_mousePos.x = static_cast<float>(_mouseX);
	m_mousePos.y = static_cast<float>(_mouseY);
}

// PlayerWalking( bool )
//
// Description:	Toggles the player's current status on walking.
//				
// @Param	bool	Is the l.shift button pressed for walking.
//
// @Return	n/a
//
void Game::PlayerWalking(bool _isWalking)
{
	m_pPlayer->SetWalking(_isWalking);
}

// MovePlayer( int )
//
// Description:	Moves the player in a direction. Positive is to the right.
//				
// @Param	int		Player direction (ideally 1, 0 or -1)
//
// @Return	n/a
//
void  Game::MovePlayer(int _x)
{
	m_pPlayer->Move(_x);
}

// JumpPlayer( bool )
//
// Description:	Stores potential power, till the player is launched (in a jump).
//				Allows different height jumps to be created.
//				
// @Param	bool	player is charging the jump button (max = 5 input calls)
//
// @Return	n/a
//
void Game::JumpPlayer(bool isJumping)
{
	if (isJumping)
	{
		//Assuming the player is grounded:
		if (m_pPlayer->IsGrounded())
		{
			//Add to the jump delay.
			m_jumpDelay += 0.20f;
			if (m_jumpDelay >= 1)
			{ //Exceended
				m_pPlayer->Jump(1);
				m_jumpDelay = 0;
			}
		}
	}
	else if(m_jumpDelay > 0)
	{
		m_pPlayer->Jump(m_jumpDelay);
		m_jumpDelay = 0;
	}
}

// ApplyGravity( float dT )
//
// Description:	Applies gravitiy to all enitites in the scene, regardless if they are
//				affected by gravity. How gravity affects them is within the class.
//				
// @Param	float	change in time since last call of this function.
//
// @Return	n/a
//
void Game::ApplyGravity(float dT)
{
	float gravVal = m_gravity * dT;
	m_pPlayer->ApplyGravity(gravVal);
	for (Platform* p : m_platforms)
	{
		p->ApplyGravity(gravVal);
	}
	for (Box* b : m_boxes)
	{
		b->ApplyGravity(gravVal);
	}
	for (Guard* g : m_guards)
	{
		g->ApplyGravity(gravVal);
	}
}

// PickUpItem( )
//
// Description:	Will prompt the player to pick up the overlapping box. 
//				 
// @Return	n/a
//
void Game::PickUpItem()
{
	std::vector<Box*>::iterator iter;
	Box* item = 0;

	//Check for the box which is overlapping
	for (iter = m_boxes.begin(); iter != m_boxes.end(); iter++)
	{
		if (m_pPlayer->IsCollidingWith((*iter)->GetHitBox()) && m_pPlayer->EmptyHanded())
		{
			item = *iter;
			m_boxes.erase(iter);
			break;
		}
	}
	if (item != 0)
	{
		m_pPlayer->PickUpBox(item);
	}
}

// DropItem( )
//
// Description:	Will prompt the player to drop the box that they are holding. 
//				 
// @Return	n/a
//
void Game::DropItem()
{
	if (!m_pPlayer->EmptyHanded())
	{
		Box* box = m_pPlayer->DropBox();

		//reintroduce the box into the world
		m_boxes.push_back(box);
	}
}

// CreateBullet(sf::Vector2f, sf::Vector2f)
//
// Description:	Create a bulled into the world with a direction vector.
//				
// @Param	sf::Vector2f	Directional vector of the bullet.
// @Param	sf::Vector2f	Bullet origin.
//
// @Return	n/a
//
void Game::CreateBullet(sf::Vector2f dirVect, sf::Vector2f origin)
{
	Bullet* b = new Bullet();
	b->Initialise(dirVect, origin);
	m_bullets.push_back(b);
}

// HasFinished()
//
// Description:	Returns true if the player reached the portal.
//				
// @Return	boolean		the player has reached the portal.
//
bool Game::HasFinished()
{
	return m_portalReached;
}

// ToggleDebug()
//
// Description:	Loads up the debug menu.
//				
// @Return n/a
//
void Game::ToggleDebug()
{
	MiniWindow debug;
	sf::Vector2f Pos(m_pBackBuffer->GetWindowSize());
	if (debug.Initialise(this, sf::Vector2f((Pos.x - 500) / 2, (Pos.y - 600) / 2), Type::DEBUG))
	{
		m_pBackBuffer->ResetView();

		if (debug.DoWindowLoop())
		{
			Quit(); //Force quit
		}
		//Reset view back to game:
		m_pBackBuffer->UpdateViewSize(sf::Vector2f(static_cast<float>(m_width *0.75f), static_cast<float>(m_height * 0.75f)));
	}
	NoiseSystem::GetInstance().UpdatePlayerSilent(m_isSilentShoes);
	m_Clock.restart();
}

// TogglePause()
//
// Description:	Loads up the pause menu.
//				
// @Return n/a
//
void Game::TogglePause()
{
	MiniWindow pause;
	sf::Vector2f Pos(m_pBackBuffer->GetWindowSize());
	if (pause.Initialise(this, sf::Vector2f((Pos.x - 500) / 2, (Pos.y - 600) / 2), Type::PAUSE))
	{
		m_pBackBuffer->ResetView();
		if (pause.DoWindowLoop())
		{
			Quit(); //Force quit
		}
		//Reset view back to game:
		m_pBackBuffer->UpdateViewSize(sf::Vector2f(static_cast<float>(m_width *0.75f), static_cast<float>(m_height * 0.75f)));
	}
	m_Clock.restart();
}

// GetDebugInfo(bool&, bool&, bool&, bool&, bool&, bool&, bool&)
//
// Description:	Loads up the pause menu.
//				
// @Param	Boolean		Reference to save Debug value for God mode to.
// @Param	Boolean		Reference to save Debug value for Silent Shoes to.
// @Param	Boolean		Reference to save Debug value for Show Noise to.
// @Param	Boolean		Reference to save Debug value for Show hitboxes to.
// @Param	Boolean		Reference to save Debug value for Show hearing to.
// @Param	Boolean		Reference to save Debug value for Show sight to.
// @Param	Boolean		Reference to save Debug value for Show patrol to.
//
// @Return n/a
//
void Game::GetDebugInfo(bool& GM, bool& SShoes, bool& SN, bool& SHitbox, bool& SHear, bool& SSight, bool& SP)
{
	GM = m_isGodMode;
	SShoes = m_isSilentShoes;
	SN = m_isShowingNoise;
	SHitbox = m_isShowingHitboxes;
	SHear = m_isShowingHearing;
	SSight = m_isShowingSight;
	SP = m_isShowingPatrol;
}

// SetDebugInfo(bool&, bool&, bool&, bool&, bool&, bool&, bool&)
//
// Description:	Updates the game's debug values
//				
// @Param	Boolean		Updated values Debug value for God mode.
// @Param	Boolean		Updated values Debug value for Silent Shoes.
// @Param	Boolean		Updated values Debug value for Show Noise.
// @Param	Boolean		Updated values Debug value for Show hitboxes.
// @Param	Boolean		Updated values Debug value for Show hearing.
// @Param	Boolean		Updated values Debug value for Show sight.
// @Param	Boolean		Updated values Debug value for Show patrol.
//
// @Return n/a
//
void Game::SetDebugInfo(bool GM, bool SShoes, bool SN, bool SHitbox, bool SHear, bool SSight, bool SP)
{
	m_isGodMode = GM;
	m_isSilentShoes = SShoes;
	m_isShowingNoise = SN;
	m_isShowingHitboxes = SHitbox;
	m_isShowingHearing = SHear;
	m_isShowingSight = SSight;
	m_isShowingPatrol = SP;
}
// HasBeenDetected()
//
// Description:	Returns true if the player was detected throughout the level.
//	
// @Return Boolean		if the player has been detected by the a guard.
//
bool Game::HasBeenDetected()
{
	return m_hasBeenDetected;
}