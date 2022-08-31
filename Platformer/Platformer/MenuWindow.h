#ifndef _MENUWINDOW_H_
#define _MENUWINDOW_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: MenuWindow.h
// Description   	: A class for the Main menu, processed before the game is played.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Window.h"

//Library Includes
#include <vector>

//Forward Declaration
class Button;
class ListBox;

//Implementation
class MenuWindow : public Window
{
	//Member function
public:
	MenuWindow();
	virtual ~MenuWindow();

	bool Initialise(BackBuffer* _backBuffer, SoundBuffer* _soundBuffer);
	
	//Inherited functions:
	virtual bool DoWindowLoop();
	virtual void DrawContents(BackBuffer& buffer);
	virtual void Process(float dT);

	virtual void MouseMoved(const int& _mouseX, const int& _mouseY);
	virtual void MousePressed(const int& _mouseX, const int& _mouseY);
	virtual void MouseReleased(const int& _mouseX, const int& _mouseY);
	
private:
	//Button functions:
	void Play(int _level);
	void Settings();

protected:
	//Member data
public:
	
private:

protected:
	sf::Vector2f m_mousePos;
	sf::Vector2f* m_mousePressPos;

	Button* m_pPlayBtn;
	Button* m_pSelectBtn;
	Button* m_pSettingBtn;
	Button* m_pQuitBtn;
	ListBox* m_levelList;

	sf::Sprite* m_pBackground;
	bool m_hasMouseClicked;
	
};
#endif //_MENUWINDOW_H_