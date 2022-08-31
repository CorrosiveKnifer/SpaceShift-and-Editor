//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: SoundBuffer.cpp
// Description   	: A Class in charge of the sound and playing sounds.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "SoundBuffer.h"

//Local Includes
#include "IniParser.h"

//Constructor
SoundBuffer::SoundBuffer()
	: m_backgroundVol(0), m_soundVol(0), m_playerHearing(0, 0), m_localDistance(400)
	, m_volModifier(1.0)
{

}

//Deconstructor
SoundBuffer::~SoundBuffer()
{
	std::vector<SoundEffect*>::iterator soundIter = m_soundList.begin();
	while (soundIter != m_soundList.end())
	{
		delete * soundIter;
		soundIter = m_soundList.erase(soundIter);
	}
	if (m_backgroundMusic != 0)
	{
		delete m_backgroundMusic;
		m_backgroundMusic = 0;
	}

	if (m_heldMusic != 0)
	{
		delete m_heldMusic;
		m_heldMusic = 0;
	}
}

// Initialise( )
//
// Description:	Sets up the sound buffer's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	n/a
//
// @return	bool	Status of the initialisation.
//
bool SoundBuffer::Initialise()
{
	if (IniParser::GetInstance().LoadIniFile("assets/settings.ini"))
	{
		m_backgroundVol = IniParser::GetInstance().GetValueAsFloat("Settings", "BGMusic");
		m_soundVol = IniParser::GetInstance().GetValueAsFloat("Settings", "SEMusic");
		
		return true;
	}
	return false;
}

// Process( sf::Vector2f )
//
// Description:	Process the current state of each 2d sound effect from the
//				player listening position.
//				
// @param	sf::Vector2f	Player's new position.
//
// @return	n/a
//
void SoundBuffer::Process(sf::Vector2f playerLoc)
{
	if (m_playerHearing.x == playerLoc.x && m_playerHearing.y == playerLoc.y)
	{
		return; //Do nothing, the player hasn't moved.
	}

	m_playerHearing = playerLoc;
	//WARNING: MAY PRODUCE LAG DEPENDING ON AMOUNT OF EFFECTS:
	//Adjust the Sound effect volume, because player moved.
	UpdateSEVolume(static_cast<unsigned int>(m_soundVol));
}

// LoadBackgroundMusic( std::string _fileLoc )
//
// Description:	Loads a new music file into the primary position.
//				Overwriting the held slot with the previous music. 
//
// @param	std::string		.wav file location.
//
// @return	boolean		If the .wav file was found.
//
bool SoundBuffer::LoadBackgroundMusic(std::string _fileLoc)
{
	sf::Music* temp = new sf::Music();
	if (!temp->openFromFile(_fileLoc))
	{
		//Load failed.
		return false;
	}
	//Continues:

	//Update volume
	temp->setVolume(m_backgroundVol * m_volModifier);

	//Send previous music to held slot
	HoldBackgroundMusic();
	
	//Store new Music:
	m_backgroundMusic = temp;
	return true;
}

// PlayBackgroundMusic( )
//
// Description:	Plays the background music.
//
// @param	n/a
//
// @return	n/a
//
void SoundBuffer::PlayBackgroundMusic()
{
	m_backgroundMusic->play();
}

// PauseBackgroundMusic( )
//
// Description:	Pause the background music.
//
// @param	n/a
//
// @return	n/a
//
void SoundBuffer::PauseBackgroundMusic()
{
	m_backgroundMusic->pause();
}

// StopMusic( )
//
// Description:	Stop the background music.
//
// @param	n/a
//
// @return	n/a
//
void SoundBuffer::StopBackgroundMusic()
{
	m_backgroundMusic->stop();
}

// HoldBackgroundMusic( )
//
// Description:	Transitions the current background music to the held music.
//				Pausing and Overwriting the current held music.
//
// @return	n/a
//
void SoundBuffer::HoldBackgroundMusic()
{
	if (m_backgroundMusic != 0)
	{
		//Pause and hold the previous music (for safe keeping)
		m_backgroundMusic->pause();

		if (m_backgroundMusic != 0)
		{
			//Delete previous held music
			delete m_heldMusic;
			m_heldMusic = 0;
		}
		//Hold previous music:
		m_heldMusic = m_backgroundMusic;
		m_backgroundMusic = 0;
	}
}

// LoadHeldBackgroundMusic( )
//
// Description:	Switches the current music and held music.
//
// @return	n/a
//
void  SoundBuffer::LoadHeldBackgroundMusic()
{
	if (m_heldMusic != 0)
	{
		sf::Music* temp = m_heldMusic;
		if (m_backgroundMusic != 0)
		{
			m_backgroundMusic->pause();
			m_heldMusic = m_backgroundMusic;
			m_backgroundMusic = 0;
		}
		m_backgroundMusic = temp;
	}
}

// PlaySoundEffect( std::string _fileLoc )
//
// Description:	Creates a sound and plays it imediately and
//				checks if any sound has stopped (to be removed).
//
// @param	std::string		.wav file location.
//
// @return	boolean		If the .wav file was found.
//
bool SoundBuffer::PlaySoundEffect(std::string _fileLoc, sf::Vector2f _soundLoc, bool _global)
{
	//Create sound
	if (!m_buffer.loadFromFile(_fileLoc))
	{
		return false; //Sound Not found
	}

	SoundEffect* e = new SoundEffect();
	e->location = _soundLoc;
	e->isGlobal = _global;
	e->sound = new sf::Sound(m_buffer);
	
	if (e->isGlobal)
	{
		e->sound->setVolume(m_soundVol * m_volModifier);
	}
	else
	{
		e->sound->setVolume(CalculateVol(e->location) * m_volModifier);
	}
	
	m_soundList.push_back(e);
	e->sound->play();

	//Dump excess sound.
	DumpSounds();

	return true;
}

// StopAllSoundEffects( )
//
// Description:	Stops and deletes all sound in the sound list.
//
// @param	n/a
//
// @return	n/a
//
void SoundBuffer::StopAllSoundEffects()
{
	std::vector<SoundEffect*>::iterator soundIter = m_soundList.begin();
	while (soundIter != m_soundList.end())
	{
		(*soundIter)->sound->stop();
		delete * soundIter;
		soundIter = m_soundList.erase(soundIter);
	}
}

// SetBGLooping( bool _looping )
//
// Description:	Updates the background music looping status
//
// @param	boolean		New looping status of the background
//
// @return	n/a
//
void SoundBuffer::SetBGLooping(bool _looping)
{
	m_backgroundMusic->setLoop(_looping);
}

// UpdateBGVolume( unsigned int _vol )
//
// Description:	Sets the volume of the background music.
//
// @param	unsigned int	new volume between 0 and 100.
//
// @return	n/a
//
void SoundBuffer::UpdateBGVolume(unsigned int _vol)
{
	if (_vol > 100)
		_vol = 100;

	m_backgroundVol = static_cast<float>(_vol);
	
	if (m_backgroundMusic != 0)
	{
		m_backgroundMusic->setVolume(static_cast<float>(_vol * m_volModifier));
	}
	
	if (m_heldMusic != 0)
	{
		m_heldMusic->setVolume(static_cast<float>(_vol * m_volModifier));
	}
	
}

// UpdateSEVolume( unsigned int _vol )
//
// Description:	Sets the volume of the all (and future) sounds within the soundlist.
//
// @param	unsigned int	new volume between 0 and 100.
//
// @return	n/a
//
void SoundBuffer::UpdateSEVolume(unsigned int _vol)
{
	if (_vol > 100)
		_vol = 100;
	
	m_soundVol = static_cast<float>(_vol);
	
	for (unsigned int i = 0; i < m_soundList.size(); i++)
	{
		if (m_soundList.at(i)->isGlobal)
		{
			m_soundList.at(i)->sound->setVolume(static_cast<float>(_vol * m_volModifier));
		}
		else
		{
			m_soundList.at(i)->sound->setVolume(CalculateVol(m_soundList.at(i)->location * m_volModifier));
		}
	}
}

// UpdateVolumeModifier( )
//
// Description:	Updates global volume modifier.
//
// @param	float	volume between 0 to 1
//
// @return	n/a
//
void SoundBuffer::UpdateVolumeModifier(float _cent)
{
	m_volModifier = _cent;
	UpdateBGVolume(static_cast<unsigned int>(m_backgroundVol));
	UpdateSEVolume(static_cast<unsigned int>(m_soundVol));
}

// GetBGVolume( )
//
// Description:	Returns the Current volume of the background music.
//
// @param	n/a
//
// @return	unsigned int	volume between 0 to 100.
//
unsigned int SoundBuffer::GetBGVolume()
{
	return static_cast<unsigned int>(m_backgroundVol);
}

// GetSEVolume( )
//
// Description:	Returns the Current volume of sound effect.
//
// @param	n/a
//
// @return	unsigned int	volume between 0 to 100.
//
unsigned int SoundBuffer::GetSEVolume()
{
	return  static_cast<unsigned int>(m_soundVol);
}

// DumpSounds( )
//
// Description:	Deletes all sounds which have the Stopped status.
//
// @param	n/a
//
// @return	n/a
//
void SoundBuffer::DumpSounds()
{
	//All Sounds:
	std::vector<SoundEffect*>::iterator soundIter = m_soundList.begin();
	while (soundIter != m_soundList.end())
	{
		if ((*soundIter)->sound->getStatus() == sf::Sound::Status::Stopped)
		{
			delete * soundIter;
			soundIter = m_soundList.erase(soundIter);
		}
		else
		{
			soundIter++;
		}
	}
}

// CalculateVol( sf::Vector2f loc )
//
// Description:	Calculates the distance between the player and location.
//				Scaling the global volume by a multiplier based on the distance.
//				Note: this calculation is called per update because the player location
//				is constantly moving.
//
// @param	sf::vector2f	location of the sound.
//
// @return	float	new volume to be used for local sounds
//
float SoundBuffer::CalculateVol(sf::Vector2f loc)
{
	float dist = std::sqrtf(std::powf(loc.x - m_playerHearing.x, 2) + std::powf(loc.y - m_playerHearing.y, 2));
	
	if(dist <= m_localDistance)
	{
		float multi = 1 - dist / m_localDistance; //1.0 - 0.0
		float globalVol = m_soundVol / 100;
		return (globalVol * multi) * 100;
	}
	return 0;
}