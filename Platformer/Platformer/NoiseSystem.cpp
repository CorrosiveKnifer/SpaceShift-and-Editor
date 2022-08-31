//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : NoiseSystem.cpp
// Description   : NoiseSystem is a singleton, factory that is incharge 
//				   of handling ingame sound.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Self Includes
#include "NoiseSystem.h"

//Static instances:
NoiseSystem* NoiseSystem::sm_pInstance = 0;
const int NoiseSystem::NoiseRange = 200;

// GetInstance( )
//
// Description:	Returns the single instance of the system.
//				Note: Will create the instance if it doesn't exist.
//				
// @Param	n/a
//
// @Return	NoiseSystem&	address location of single instance.
//
NoiseSystem& NoiseSystem::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new NoiseSystem();
	}
	return *sm_pInstance;
}

// DestroyInstance( )
//
// Description:	Deconstructs the system, removing it from memory.
//				
// @Param	n/a
//
// @Return	n/a
//
void NoiseSystem::DestroyInstance()
{
	if (sm_pInstance != 0)
		delete sm_pInstance;
	sm_pInstance = 0;
}

//Constructor
NoiseSystem::NoiseSystem()
{
	m_soundBuffer = 0;
	isPlayerSilent = false;
}

//Deconstructor
NoiseSystem::~NoiseSystem()
{
	Clear();
	m_soundBuffer = 0;
}

// CreateNoise( sf::Vector2f Location, float volume, float decay )
//
// Description:	Creates a noise and stores it within the system.
//				
// @Param	sf::Vector2f  World location of the noise
// @Param	float	Volume of the noise (between 0 and 1)
// @Param	float	Decay of the noise (between 0 and 1)
// @Param	SourceType	Setting a tag to the noise (Default = NONE).
//
// @Return	n/a
//
void NoiseSystem::CreateNoise(sf::Vector2f _location, float _vol, float _decay, std::string soundEffect, SourceType type)
{
	Noise* n = new Noise();
	n->worldPos = _location;
	n->sourceType = type;

	//Clamp
	if (_vol > 1)
	{
		n->volume = 1;
	}
	else
	{
		n->volume = _vol;
	}
	//Clamp
	if (_decay > 1)
	{
		n->decay = 1;
	}
	else
	{
		n->decay = _decay;
	}

	if (!(isPlayerSilent && type == SourceType::PLAYER))
	{
		if (n->volume > 0)
		{
			m_noiseQueue.push_back(n);

			std::push_heap(m_noiseQueue.begin(), m_noiseQueue.end(), NoiseSystem());
		}

		if (m_soundBuffer != 0 && soundEffect != "")
		{
			m_soundBuffer->PlaySoundEffect(soundEffect, _location, false);
		}
	}
}

// UpdateSoundBuffer( SoundBuffer& )
//
// Description:	Update the current sound buffer to queue sound effects to.
//				
// @Param	SoundBuffer& new sound buffer.
//
// @Return	n/a
//
void NoiseSystem::UpdateSoundBuffer(SoundBuffer& buffer)
{
	m_soundBuffer = &buffer;
}

// Process( float )
//
// Description:	Process the current state of the noise system since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void NoiseSystem::Process(float dT)
{
	NoiseVector::iterator iter = m_noiseQueue.begin();
	while (iter != m_noiseQueue.end())
	{
		if ((*iter)->volume <= 0)
		{
			delete * iter;
			iter = m_noiseQueue.erase(iter);
		}
		else
		{
			(*iter)->volume -= (*iter)->decay * dT;
			iter++;
		}
	}
	UpdateNoiseQueue();
}

// UpdateNoiseQueue( )
//
// Description:	Updates the vector to maintain the first value is the
//				closest to expire.
//				
// @Param	n/a
//
// @Return	n/a
//
void NoiseSystem::UpdateNoiseQueue()
{
	std::make_heap(m_noiseQueue.begin(), m_noiseQueue.end(), NoiseSystem());
}

// Draw( BackBuffer& buffer )
//
// Description:	Deconstructs the system, removing it from memory.
//				
// @Param	BackBuffer&		buffer to draw the ranges of each noise.
//
// @Return	n/a
//
void NoiseSystem::Draw(BackBuffer& buffer)
{
	buffer.SetColour(sf::Color::White);
	for (Noise* n : m_noiseQueue)
	{
		buffer.DrawCircle(n->worldPos.x - n->volume * NoiseRange, n->worldPos.y - n->volume * NoiseRange, n->volume * NoiseRange);
		//buffer.DrawCircle(n->worldPos.x, n->worldPos.y, n->volume * NoiseRange);
	}
	buffer.SetColour(sf::Color::Black);
}

// HasHeardANoise( BackBuffer& buffer )
//
// Description:	Deconstructs the system, removing it from memory.
//				Ignores sounds created by guards.
//				
// @Param	sf::Vector2f	Ears position in world space.
// @Param	sf::Vector2f	Hearing range.
//
// @Return	Noise*	Loudest Noise which was heard.
//
Noise* NoiseSystem::HasHeardANoise(sf::Vector2f earPos, sf::Vector2f hearRange)
{
	Noise* Loudest = 0;
	for (Noise* n : m_noiseQueue)
	{
		if (n->sourceType != SourceType::GUARD)
		{
			bool xAxis = std::abs((n->worldPos.x) - (earPos.x)) <= std::abs((n->volume * NoiseRange) + hearRange.x);
			bool yAxis = std::abs((n->worldPos.y) - (earPos.y)) <= std::abs((n->volume * NoiseRange) + hearRange.y);

			if (xAxis && yAxis)
			{
				if (Loudest != 0)
				{
					if (Loudest->volume <= n->volume)
					{
						Loudest = n;
					}
				}
				else
				{
					Loudest = n;
				}
			}
		}
	}

	return Loudest;
}

// UpdatePlayerSilent( bool )
//
// Description:	Updates the debug ability to disable noise creatd with the 
//				tag 'player'.
//				
// @Param	boolean		Toggle status
//
// @Return	n/a
//
void NoiseSystem::UpdatePlayerSilent(bool isSilent)
{
	isPlayerSilent = isSilent;
}

// Clear( )
//
// Description:	Clear all sounds within the system.
//
// @Return	n/a
//
void NoiseSystem::Clear()
{
	NoiseVector::iterator iter = m_noiseQueue.begin();
	while (iter != m_noiseQueue.end())
	{
		delete * iter;
		iter++;
	}
	m_noiseQueue.clear();
}