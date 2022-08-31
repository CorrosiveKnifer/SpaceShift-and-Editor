#ifndef _PLAYER_H_
#define _PLAYER_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Player.h
// Description   	: A class to handle the player within the world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Includes
#include "Entity.h"

//Forward Declaration
class SpriteMap;
class Box;

//Implementation
class Player : public Entity
{
	//Member functions
public:
	//Enum state for animation
	enum AnimState { IDLE, MOVING, FALLING, LANDING, JUMPING, DEATH };

	Player();
	virtual  ~Player();

	bool Initialise(BackBuffer& buffer, std::string spawnCode);
	bool Deinitialise();

	//Inherited functions:
	virtual void Draw(BackBuffer& buffer);
	virtual void Process(float dT);
	virtual void ApplyGravity(float _grav);
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;
	virtual const sf::IntRect GetHitBox() const;
	virtual const sf::Vector2f GetSize() const;

	void UnProcessX();
	void UnProcessY();
	const sf::Vector2f GetCenterPosition() const;

	//External Actions
	void Move(int _x);
	void Jump(float powerCent);
	void KillPlayer();
	void PickUpBox(Box* newBox);
	Box* DropBox();
	void SetGrounded(bool _isGrounded);
	void SetTouchWall(bool _isWall);
	void PushPlayer(sf::Vector2f _vec);

	void AnimationLogic();
	void TransitionTo(AnimState newState);
	void SetWalking(bool _isWalking);
	bool EmptyHanded() const;
	
	bool IsPlayerDead();
	bool IsPlayerDying();
	bool IsGrounded();
protected:

private:

	//Member Data
public:

protected:
	SpriteMap* m_pAnim;
	int m_width, m_height; 
	sf::IntRect m_hitbox;
	bool m_isGrounded, m_isWalking, m_isDead;
	bool m_isTouchWallLeft, m_isTouchWallRight;

	sf::Vector2f m_prevLoc;
	Box* m_pBox;

private:
	float m_soundDelay, m_targetVelocityX;
	int m_direction;
	AnimState m_state;
};
#endif // !_PLAYER_H_
