#ifndef _LOGOWINDOW_H_
#define _LOGOWINDOW_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: LogoWindow.h
// Description   	: Used to display a single logo which fades in and out.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Window.h"

//Library Includes
#include <vector>

//Forward Declaration
class TextObject;

//Implementation
class LogoWindow : public Window
{
	//Member function
public:
	LogoWindow();
	virtual ~LogoWindow();

	bool Initialise(BackBuffer* _backBuffer, SoundBuffer* _soundBuffer);
	bool Deinitialise();

	//Inheritance function:
	virtual bool DoWindowLoop();
	virtual void DrawContents(BackBuffer& buffer);
	virtual void Process(float dT);

	void AddText(std::string msg, sf::Vector2f loc);
	void ClearTexts();

	void SetInputEnabled(bool _input);
	void SetLogoLoc(sf::Vector2f logoLoc);
	void SetLogo(sf::Sprite* sp);
	void SetBackgroundColour(sf::Color col);
	void SetTimeLength(float _time);
private:

protected:

	//Member data
public:

private:

protected:
	sf::Sprite* m_logo;
	sf::Color m_screenColor;
	std::vector<TextObject*> m_texts;
	bool m_isInputEnabled;
};
#endif //_LOGOWINDOW_H_