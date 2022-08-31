#ifndef _SPRITE_MAP_H_
#define _SPRITE_MAP_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : SpriteMap.h
// Description   : Functionality for a grid of sprites to be converted 
//				   into animation. Note: the logic of the animation is up to
//				   the class who has a sprite map.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Library Includes
#include <SFML/Graphics.hpp>

//Implementation
class SpriteMap
{
	//Member functions
public:
	SpriteMap();
	~SpriteMap();

	bool Initialise(sf::Sprite* _Source, int frameWidth, int frameHeight);

	void ProcessX(float dT);
	void ProcessY(float dT);

	void SetDelay(float x, float y);
	void SetIsLooping(bool _isLooping);

	int GetIndexX() const;
	int GetIndexY() const;
	void SetIndexX(int _x);
	void SetIndexY(int _y);
	void SetLoopingBoundsX(unsigned int start, unsigned int end);
	bool IsAnimationFinished();
	bool IsLooping();
private:

protected:

	//Member data
public:

private:

protected:
	sf::Sprite* m_Source;

	int* m_frameX;
	int* m_frameY;
	int m_width, m_height;
	unsigned int m_indexX, m_indexY;
	unsigned int m_MaxIndexX, m_MaxIndexY;
	bool m_isLooping;
	unsigned int m_loopS, m_loopE;
	float m_delayX, m_delayY; //in seconds
	float m_timeX, m_timeY;
};
#endif // _SPRITE_MAP_H_