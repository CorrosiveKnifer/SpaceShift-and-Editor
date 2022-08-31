#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Background.h
// Description   	: Paralax background
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
#include "Backbuffer.h"

#include <vector>

class Background
{
	//Member functions
public:
	Background();
	~Background();

	bool Initialise(int width, int height);

	void Draw(BackBuffer& buffer);
	void Process(sf::Vector2f playerPos);

	void Refresh();

protected:

private:
	//Member Data
public:

protected:
	sf::Vector2f* m_playerPos;
	std::vector<sf::Vector2f> m_largeStars;
	std::vector<sf::Vector2f> m_mediumStars;
	std::vector<sf::Vector2f> m_smallStars;

	int m_width, m_height;

	const int m_largeCount = 10;
	const int m_mediumCount = 20;
	const int m_smallCount = 30;

	const float m_largeScale = 4;
	const float m_mediumScale = 2;
	const float m_smallScale = 1;

	const float m_largeSpeed = 0.30f;
	const float m_mediumSpeed = 0.15f;
	const float m_smallSpeed = 0.10f;
private:

};
#endif // !_BACKGROUND_H_
