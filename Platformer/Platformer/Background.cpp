//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Background.cpp
// Description   	: Paralax background
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
#include "Background.h"

Background::Background()
	: m_playerPos(0)
{

}

Background::~Background()
{
	if (m_playerPos == 0)
	{
		delete m_playerPos;
		m_playerPos = 0;
	}
}

bool Background::Initialise(int width, int height)
{
	m_width = width;
	m_height = height;

	for (int i = 0; i < m_largeCount; i++)
	{
		m_largeStars.push_back(sf::Vector2f(static_cast<float>(rand() % width), static_cast<float>(rand() % height)));
	}

	for (int i = 0; i < m_mediumCount; i++)
	{
		m_mediumStars.push_back(sf::Vector2f(static_cast<float>(rand() % width), static_cast<float>(rand() % height)));
	}

	for (int i = 0; i < m_smallCount; i++)
	{
		m_smallStars.push_back(sf::Vector2f(static_cast<float>(rand() % width), static_cast<float>(rand() % height)));
	}
	return true;
}

void Background::Draw(BackBuffer& buffer)
{
	buffer.SetColour(sf::Color::Black);
	buffer.DrawRectangle(0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height));
	
	buffer.SetColour(sf::Color::White);

	for (int i = 0; i < m_largeCount; i++)
	{
		buffer.DrawFillCircle(m_largeStars.at(i).x, m_largeStars.at(i).y, m_largeScale);
	}

	for (int i = 0; i < m_mediumCount; i++)
	{
		buffer.DrawFillCircle(m_mediumStars.at(i).x, m_mediumStars.at(i).y, m_mediumScale);
	}

	for (int i = 0; i < m_smallCount; i++)
	{
		buffer.DrawFillCircle(m_smallStars.at(i).x, m_smallStars.at(i).y, m_smallScale);
	}
}

void Background::Process(sf::Vector2f playerPos)
{
	if (m_playerPos == 0)
	{	//Initialise the player position
		m_playerPos = new sf::Vector2f(playerPos.x, playerPos.y);
		return;
	}
	sf::Vector2f dist(playerPos.x - m_playerPos->x, playerPos.y - m_playerPos->y);

	for (int i = 0; i < m_largeCount; i++)
	{
		m_largeStars.at(i).x += dist.x * m_largeSpeed;
		m_largeStars.at(i).y += dist.y * m_largeSpeed;
	}

	for (int i = 0; i < m_mediumCount; i++)
	{
		m_mediumStars.at(i).x += dist.x * m_mediumSpeed;
		m_mediumStars.at(i).y += dist.y * m_mediumSpeed;
	}

	for (int i = 0; i < m_smallCount; i++)
	{
		m_smallStars.at(i).x += dist.x * m_smallSpeed;
		m_smallStars.at(i).y += dist.y * m_smallSpeed;
	}

	m_playerPos->x = playerPos.x;
	m_playerPos->y = playerPos.y;
}

void Background::Refresh()
{
	delete m_playerPos;
	m_playerPos = 0;
}