#ifndef _NOISESYSTEM_H_
#define _NOISESYSTEM_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : NoiseSystem.h
// Description   : NoiseSystem is a singleton, factory that is incharge 
//				   of handling ingame sound.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Local Includes
#include "BackBuffer.h"
#include "SoundBuffer.h"

//Library Includes
#include <queue>

#define NoiseVector std::vector<Noise*>

enum SourceType {NONE, PLAYER, GUARD, PROJ, WORLD};

struct Noise {
	sf::Vector2f worldPos;
	SourceType sourceType;
	float volume; //0.0f - 1.0f
	float decay; //0.0f - 1.0f;
};

class NoiseSystem
{
	//Member functions
public:
	static NoiseSystem& GetInstance();
	static void DestroyInstance();
	~NoiseSystem();

	void UpdateSoundBuffer(SoundBuffer& buffer);
	void CreateNoise(sf::Vector2f Location, float volume, float decay, std::string soundEffect, SourceType type = SourceType::NONE);
	void Process(float dT);
	void Draw(BackBuffer& buffer);

	bool operator() (Noise* _a, Noise* _b) { return (_a->volume > _b->volume); };
	Noise* HasHeardANoise(sf::Vector2f earPos, sf::Vector2f hearRange);
	void UpdatePlayerSilent(bool isSilent);

	void Clear();
protected:

private:
	NoiseSystem();
	NoiseSystem(const NoiseSystem& noiseSyst);
	NoiseSystem& operator=(const NoiseSystem& noiseSyst);

	void UpdateNoiseQueue();
	//Member Data
public:

protected:
	static NoiseSystem* sm_pInstance;
	static const int NoiseRange;

	bool isPlayerSilent;
	NoiseVector m_noiseQueue;
	SoundBuffer* m_soundBuffer;
private:
};

#endif // !_NOISESYSTEM_H_
