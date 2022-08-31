#ifndef _GUARD_H_
#define _GUARD_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : Guard.h
// Description   : An entity which opposes the player.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Parent Include
#include "Entity.h"

//Forward Declaration
class SpriteMap;
class Platform;

class Guard : public Entity
{
	//Member functions
public:
	Guard();
	virtual ~Guard();

	bool Initialise(BackBuffer& buffer, std::string spawnCode);
	bool Deinitialise();

	virtual void Draw(BackBuffer& buffer);
	virtual void Process(float dT);
	void UnProcess();

	//Inherited Function:
	virtual void ApplyGravity(float _grav);
	virtual bool IsCollidingWith(const sf::IntRect& otherDiment) const;
	virtual bool IsBelowOf(const sf::IntRect& otherDiment) const;
	virtual const sf::IntRect GetHitBox() const;
	virtual const sf::Vector2f GetSize() const;

	//AI related functions:
	void AIProcess(float dT);
	bool IsWithinVision(const sf::IntRect& otherDiment) const;
	void HandleNoise();
	void HandleSight();

	//External AI functions:
	void CalmDown();
	void Alert(sf::Vector2f sourceLoc);
	void Detected(Entity* offender);
	void Halt();
	void SetGrounded(bool _isGrounded, Platform* p);

	//Debug Functions:
	void DrawHearing(BackBuffer& buffer);
	void DrawSight(BackBuffer& buffer);
	void DrawPatrol(BackBuffer& buffer);
	void PushGuard(sf::Vector2f _vec);

protected:
	//AI SYSTEM:
	enum AIState { IDLE, MOVING, ATTACK};
	enum AIMental { CALM, ALERT, ENGAGED};

	void GetATargetLocation();
	void TransitionStateTo(AIState newState);
	void TransitionMentalTo(AIMental newMental);

private:
	void ScaleVision();

	//Member Data
public:

protected:
	SpriteMap* m_pAnim;
	sf::Sprite* m_pVisionShape;
	int m_width, m_height;
	sf::IntRect m_hitbox;
	float m_alertLevel;
	bool m_isGrounded;

	//Not owned
	Platform* m_pGround; 
	Entity* m_pTarget;

	//AI State:
	AIState m_state;
	AIMental m_mental;

	sf::Vector2f m_prevLoc, m_targLoc;
	sf::Vector2u m_visionSize;

	//Adjustables:
	float m_moveSpeed, m_waitDelay, m_currTimer, m_alertDecay;
	float m_alertSoundDelay, m_movingSoundDelay;
	sf::Vector2f m_hearingDist, m_sightRange;

private:
	int m_direction;
};

#endif // !_GUARD_H_
