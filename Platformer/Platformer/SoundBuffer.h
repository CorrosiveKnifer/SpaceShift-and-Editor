#ifndef _SOUND_BUFFER_H_
#define _SOUND_BUFFER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: SoundBuffer.h
// Description   	: A Class in charge of the sound and playing sounds.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

//Library Includes
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

struct SoundEffect
{
	~SoundEffect() { delete sound; };
	sf::Sound* sound;
	sf::Vector2f location;
	bool isGlobal;
};

//Implementation
class SoundBuffer
{
	//Member function
public:
	SoundBuffer();
	~SoundBuffer();

	bool Initialise();

	void Process(sf::Vector2f playerLoc);

	bool LoadBackgroundMusic(const std::string _fileLoc); //switch background music
	void PlayBackgroundMusic();
	void PauseBackgroundMusic();
	void HoldBackgroundMusic();
	void LoadHeldBackgroundMusic();
	void StopBackgroundMusic();

	//bool PlaySoundEffect(const std::string _fileLoc); //add to list
	bool PlaySoundEffect(const std::string _fileLoc, sf::Vector2f _soundLoc = sf::Vector2f(0, 0), bool _global = true);
	void StopAllSoundEffects();

	void SetBGLooping(bool _looping);
	void UpdateBGVolume(unsigned int _vol);
	void UpdateSEVolume(unsigned int _vol);
	void UpdateVolumeModifier(float _cent);
	unsigned int GetBGVolume();
	unsigned int GetSEVolume();

private:
	void DumpSounds();
	float CalculateVol(sf::Vector2f loc);
protected:

	//Member data
public:

private:

protected:
	float m_backgroundVol, m_soundVol, m_localDistance;
	float m_volModifier;
	sf::SoundBuffer m_buffer;
	std::vector<SoundEffect*> m_soundList;
	sf::Vector2f m_playerHearing;
	sf::Music* m_backgroundMusic;
	sf::Music* m_heldMusic;
};
#endif // _SOUND_BUFFER_H_