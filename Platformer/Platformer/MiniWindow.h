#ifndef _MINIWINDOW_H_
#define _MINIWINDOW_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: MiniWindow.h
// Description   	: Acts as a pop up infront of another window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8823@mediadesign.com
//
//Parent Include
#include "Window.h"

//Forward Declaration
class ScaleBar;
class CheckBox;
class Button;

enum Type //of pop up
{
	SETTING, PAUSE, DEBUG
};

//Implementation
class MiniWindow : public Window
{
	//Member funcction
public:
	MiniWindow();
	virtual ~MiniWindow();

	bool Initialise(Window* _belowWindow, sf::Vector2f _topLeftPos, Type _type);

	virtual bool DoWindowLoop();
	virtual void DrawContents(BackBuffer& buffer);
	virtual void Process(float dT);
	
	virtual void MouseMoved(const int& _mouseX, const int& _mouseY);
	virtual void MousePressed(const int& _mouseX, const int& _mouseY);
	virtual void MouseReleased(const int& _mouseX, const int& _mouseY);

private:
	void InitialiseDebug(BackBuffer& buffer);
	void InitialiseSettings(BackBuffer& bBuffer, SoundBuffer& sBuffer);
	void InitialisePause(BackBuffer& buffer);

	void DebugDraw(BackBuffer& buffer);
	void SettingsDraw(BackBuffer& buffer);
	void PauseDraw(BackBuffer& buffer);

	void DebugProcess(float dT);
	void SettingsProcess(float dT);
	void PauseProcess(float dT);
protected:

	//Member data
public:

private:
	bool isGodMode;
	bool isSilentShoes;
	bool isShowingNoise;
	bool isShowingHitboxes;
	bool isShowingHearing;
	bool isShowingSight;
	bool isShowingPatrol;

protected:
	Window* m_backWindow;
	sf::Vector2f m_topLeftPos;
	sf::Vector2f m_size;
	sf::Vector2f m_mousePress;
	bool m_hasClicked;
	Type m_windowType;

	//Debug:
	CheckBox* m_GodMode;
	CheckBox* m_SilentShoes;
	CheckBox* m_ShowNoise;
	CheckBox* m_ShowHitboxes;
	CheckBox* m_ShowHearing;
	CheckBox* m_ShowSight;
	CheckBox* m_ShowPatrol;

	//Settings:
	ScaleBar* m_BGBar;
	ScaleBar* m_SEBar;
	CheckBox* m_DebugMode;
	CheckBox* m_FullScreenMode;
	int m_highscore;

	//Pause:
	Button* m_pBtnSettings;
	Button* m_pBtnBack;
	Button* m_pBtnQuit;
};
#endif // _MINIWINDOW_H_