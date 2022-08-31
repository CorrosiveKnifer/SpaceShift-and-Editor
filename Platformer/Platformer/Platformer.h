#ifndef _PLATFORMER_H
#define _PLATFORMER_H

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Playformer.h
// Description   	: Implementation of Playformer.h first object to be called.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Forward Declaration
class BackBuffer;
class SoundBuffer;

//Implementation
class Platformer
{
	//Member functions
public:
	Platformer();
	~Platformer();
	bool Initialise();
	void Start();

protected:

private:

	//Member Data
public:

protected:
	BackBuffer* m_pBackBuffer;
	SoundBuffer* m_pSoundBuffer;

private:

};

#endif // !_PLATROMER_H