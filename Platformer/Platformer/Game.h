#ifndef _GAME_H_
#define _GAME_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : Game.h
// Description   : Declare the game class to be used in the core of the game. 
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Parent Include
#include "Window.h"

//Library Includes
#include <vector>
#include <map>

//Forward Declaration
class Platform;
class Player;
class Entity;
class Box;
class ButtonObject;
class Door;
class Guard;
class Bullet;
class TextObject;
class End;
class Background;

//Implementation
class Game : public Window
{
	//Member functions
public:
	Game();
	virtual ~Game();
	static Game& GetInstance();

	bool Initialise(BackBuffer& _backBuffer, SoundBuffer& _soundBuffer);
	bool Deinitialise();

	void LoadLevel(std::string levelLoc);
	void RestartLevel();
	void ClearLevel();

	void DrawContents(BackBuffer& buffer);
	
	//Inherited Functions
	virtual void Process(float dT);
	virtual bool DoWindowLoop();
	virtual void MouseMoved(const int& _mouseX, const int& _mouseY);
	virtual void MousePressed(const int& _mouseX, const int& _mouseY);
	virtual void MouseReleased(const int& _mouseX, const int& _mouseY);

	//Player input functions
	void PlayerWalking(bool _isWalking);
	void MovePlayer(int _x);
	void JumpPlayer(bool isJumping);
	void PickUpItem();
	void DropItem();

	bool HasFinished();

	//Guard input functions
	void CreateBullet(sf::Vector2f dirVect, sf::Vector2f origin);

	//Extra window functions:
	void ToggleDebug();
	void TogglePause();

	void GetDebugInfo(bool& GM, bool& SShoes, bool& SN, bool& SHitbox, bool& SHear, bool& SSight, bool& SP);
	void SetDebugInfo(bool GM, bool SShoes, bool SN, bool SHitbox, bool SHear, bool SSight, bool SP);
	bool HasBeenDetected();
private:
	void ApplyGravity(float dT);

	void PlayerProcess(float dT);
	void GuardProcess(float dT);
protected:
	//Member data
public:

private:
	std::vector<Platform*> m_platforms;
	std::vector<Box*> m_boxes;
	std::vector<Door*> m_doors;
	std::vector<ButtonObject*> m_buttons;
	std::vector<Guard*> m_guards;
	std::vector<Bullet*> m_bullets;
	std::vector<TextObject*> m_TextObjects;

	sf::Vector2f m_cameraPos;

	Player* m_pPlayer;
	End* m_pEnd;

	const float m_gravity = 500;
	sf::IntRect m_WorldBox;
	std::string m_levelFileLoc;

	bool m_portalReached;
	Background* m_pBackground;
	sf::Color m_fade;
	float m_alpha;

protected:
	float m_jumpDelay;

	bool m_isGodMode;
	bool m_isSilentShoes;
	bool m_isShowingNoise;
	bool m_isShowingHitboxes;
	bool m_isShowingHearing;
	bool m_isShowingSight;
	bool m_isShowingPatrol;
	bool m_hasBeenDetected;

	unsigned int m_width, m_height;

	static Game* sm_pInstance;

};
#endif //_GAME_H_