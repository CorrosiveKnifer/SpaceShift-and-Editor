//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     : Guard.cpp
// Description   : An entity which opposes the player.
// Author        : Michael  Jordan
// Mail          : Michael.jor8834@mediadesign.com
//
//Self Include
#include "Guard.h"

//Local Includes
#include "SpriteMap.h"
#include "Platform.h"
#include "Game.h"
#include "NoiseSystem.h"
#include "Player.h"

//Constructor
Guard::Guard()
	: m_pAnim(0), m_pVisionShape(0)
	, m_width(0), m_height(0), m_hitbox(0, 0, 0, 0)
	, m_alertLevel(0) , m_isGrounded(false)
	, m_pGround(0), m_pTarget(0)
	, m_state(AIState::IDLE), m_mental(AIMental::CALM)
	, m_prevLoc(0, 0), m_targLoc(0, 0), m_visionSize(0, 0)
	, m_hearingDist(0, 0), m_sightRange(0, 0)
	, m_moveSpeed(0) , m_waitDelay(1.0f), m_currTimer(0), m_alertDecay(0.1f)
	, m_alertSoundDelay(0), m_movingSoundDelay(0)
	, m_direction(1)
{

}

//Destructor
Guard::~Guard()
{

}

// Initialise( BackBuffer& ,  std::string)
//
// Description:	Sets up the Guard's values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	BackBuffer&		Backbuffer to create the sprite from.
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
//
// @return	boolean		Status of the initialisation.
//
bool Guard::Initialise(BackBuffer& buffer, std::string spawnCode)
{
	//Destruct the spawn code:
	int pos0 = spawnCode.find_first_of('x');
	int pos1 = spawnCode.find_first_of('y');
	std::string x = spawnCode.substr(0, pos0);
	std::string y = spawnCode.substr(pos0 + 1, pos1 - pos0 - 1);
	m_position.x = static_cast<float>(std::atoi(x.c_str()));
	m_position.y = static_cast<float>(std::atoi(y.c_str()));

	//Create sprite:
	sf::Sprite* source = buffer.CreateSprite("assets/guard.png");
	source->setScale(0.25, 0.25);
	int animCol = 8;
	int animRows = 5;

	m_width = static_cast<int>((source->getTexture()->getSize().x / animCol) * 0.25f);
	m_height = static_cast<int>((source->getTexture()->getSize().y / animRows) * 0.25f);

	m_hitbox.width = 26;
	m_hitbox.height = 50;

	//Create Animation
	m_pAnim = new SpriteMap();
	m_pAnim->Initialise(source, animCol, animRows);
	m_pAnim->SetIsLooping(true);
	m_pAnim->SetDelay(0.2f, 0);

	m_currTimer = m_waitDelay * 2;

	//Vision Sprite:
	m_pVisionShape = buffer.CreateSprite("assets/vision.png");
	m_pVisionShape->setColor(sf::Color(255, 255, 255, 105));
	m_visionSize = m_pVisionShape->getTexture()->getSize();

	//Load AI base states:
	TransitionStateTo(AIState::IDLE);
	TransitionMentalTo(AIMental::CALM);
	ScaleVision();
	
	return Entity::Initialise(source);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the entity to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Guard::Deinitialise()
{
	m_pGround = 0;
	m_pTarget = 0;

	if (m_pAnim != 0)
	{
		delete m_pAnim;
		m_pAnim = 0;
	}

	if (m_pVisionShape != 0)
	{
		delete m_pVisionShape;
		m_pVisionShape = 0;
	}

	return Entity::Deinitialise();
}

// Draw( BackBuffer& )
//
// Description:	Draws the guard to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the entity to.
//
// @return	n/a
//
void Guard::Draw(BackBuffer& buffer)
{
	//Construct the matrix for different directions	
	sf::Transform trans = sf::Transform::Identity;
	trans.translate(static_cast<float>(m_width / 2.0f + m_position.x), static_cast<float>(m_height / 2.0f + m_position.y));
	trans.scale(static_cast<float>(m_direction), 1);
	trans.translate(static_cast<float>(-m_width / 2.0f), static_cast<float>(-m_height / 2.0f));

	//Display engaged or alert ui above the guard:
	if (m_mental == AIMental::ENGAGED)
	{
		buffer.SetColour(sf::Color(255, 0, 0, 255 * static_cast<int>((m_alertLevel / 10))));
		buffer.SetFontSize(15);
		buffer.DrawTextToWorld("!", m_position.x + m_width * 3 / 8, m_position.y - 18);
	}
	else if (m_mental == AIMental::ALERT)
	{
		buffer.SetColour(sf::Color(255, 255, 0, 255 * static_cast<int>((m_alertLevel / 5))));
		buffer.SetFontSize(15);
		buffer.DrawTextToWorld("?", m_position.x + m_width * 3 / 8, m_position.y - 18);
	}

	//Reset color
	buffer.SetColour(sf::Color::White);

	ScaleVision(); //Scale the vision based on current state.

	//Draw the guard:
	buffer.PushTransform(trans);
	buffer.Draw(*m_pDrawable);
	buffer.SetColour(sf::Color::Black);
	buffer.PopTransform(trans);
}

// Process( float )
//
// Description:	Process the current state of the guard since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Guard::Process(float dT)
{
	//Process animation
	m_pAnim->ProcessX(dT);

	//Update prev location
	m_prevLoc.x = m_position.x;
	m_prevLoc.y = m_position.y;
	
	//Move guard current position.
	m_position.x += m_velocity.x * dT;
	m_position.y += m_velocity.y * dT;

	//Update hit box:
	m_hitbox.left = static_cast<int>(m_position.x + m_width / 2.0f - m_hitbox.width / 2.0f);
	m_hitbox.top = static_cast<int>(m_position.y + m_height - m_hitbox.height);
	
	//Decay timers:
	if(m_alertSoundDelay > 0)
		m_alertSoundDelay -= dT;

	if (m_movingSoundDelay > 0)
		m_movingSoundDelay -= dT;

	//Decay Alert Level:
	if (m_alertLevel > 0)
	{
		m_alertLevel -= m_alertDecay * dT;
	}
	else if(m_mental != AIMental::CALM) //Transition to calm
	{
		TransitionMentalTo(AIMental::CALM);
		//Update Guard to new Mental version of moving
		if (m_state == AIState::MOVING)
		{
			TransitionStateTo(AIState::MOVING);
		}
	}

	//Create Moving sound:
	if (m_state == AIState::MOVING)
	{
		if (m_movingSoundDelay <= 0) //If Sound timer is expired.
		{
			switch (m_mental)
			{
			default:
			case Guard::CALM: //Walk
				m_movingSoundDelay = 0.6f;
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.15f, 1, "assets/footstep.wav", SourceType::GUARD);
				break;
			case Guard::ALERT: //Run
				m_movingSoundDelay = 0.6f;
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0f), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.5f, 1, "assets/Running.wav", SourceType::GUARD);
				break;
			}
		}
		
	}

	//Update vision cone position:
	m_pVisionShape->setPosition(sf::Vector2f(m_position.x + 30, m_position.y - (m_visionSize.y / m_sightRange.y) / 2 - 26));
	
	//Handle External sources:
	HandleNoise();
	HandleSight();

	//Process AI:
	AIProcess(dT);
}

// AIProcess( float )
//
// Description:	Process the current state of the Guard AI since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Guard::AIProcess(float dT)
{
	switch (m_state)
	{
		case IDLE: 
			//Wait until timer expires:
			m_currTimer -= dT;
			if (m_currTimer <= 0)
			{				
				if (m_pGround != 0)
				{
					//Transition to Moving:
					TransitionStateTo(AIState::MOVING);
					m_currTimer = m_waitDelay; //Reset idle timer for next time.

					GetATargetLocation();
				}
			}
			break;
		case MOVING:
			if (m_targLoc.x > m_position.x && m_targLoc.x < m_position.x + m_width / 2)
			{
				//Reached target location, start waiting:
				TransitionStateTo(AIState::IDLE);
				break;
			}
			else if (m_targLoc.x > m_position.x + m_width / 2)
			{
				//Move to the Right
				m_velocity.x = m_moveSpeed;
				m_direction = 1;
			}
			else if (m_targLoc.x < m_position.x + m_width / 2)
			{
				//Move to the Left
				m_velocity.x = -m_moveSpeed;
				m_direction = -1;
			}
			//else do nothing
			break;
		case ATTACK: //Can't be attacking and calm
			if (m_mental != AIMental::CALM)
			{
				if (m_pAnim->GetIndexX() == 5) //Is at gun fired frame
				{
					//Spawn bullet:
					Game::GetInstance().CreateBullet(sf::Vector2f(static_cast<float>(m_direction), 0), sf::Vector2f(m_position.x, m_position.y + 10));
					NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 1.0f, 1.0f, "assets/GunFire.wav");
					
					//Skip Delay:
					m_pAnim->SetIndexX(6);
				}
			}
			break;
	}
}

// TransitionStateTo( AIState )
//
// Description:	Sets up all the conditions for a specific AI State.
//				Sets the animation frame, velocity and anim delay.
//				
// @param	AIState		new state which the guard is transitioning into.
//
// @return	n/a
void Guard::TransitionStateTo(AIState newState)
{
	switch (newState)
	{
		case IDLE:
			//Transition:
			m_state = AIState::IDLE;

			//Update Animation sheet:
			m_pAnim->SetIndexX(0);
			m_pAnim->SetIndexY(0);
			m_pAnim->SetDelay(0.2f, 0);

			//Stop the guard:
			m_velocity.x = 0;
			break;

		case MOVING:
			//Transition:
			m_state = AIState::MOVING;

			//Update Animation sheet:
			m_pAnim->SetIndexX(0);

			if(m_mental == AIMental::ENGAGED)
				m_pAnim->SetIndexY(2); 
			else
				m_pAnim->SetIndexY(m_mental + 1);
			
			m_pAnim->SetDelay(0.20f, 0);
			break;

		case ATTACK:
			if (m_mental != AIMental::CALM)
			{
				//Transition:
				m_state = AIState::ATTACK;

				//Update Animation sheet:
				m_pAnim->SetIndexX(0);
				m_pAnim->SetIndexY(3);
				m_pAnim->SetDelay(0.15f, 0);

				//Stop the guard:
				m_velocity.x = 0;
				break;
			}
			//Can't be attacking an be calm, divert to idle
			TransitionStateTo(IDLE);
			break;
	}
}

// TransitionMentalTo( AIMental )
//
// Description:	Sets up all the conditions for a specific AI Mental state.
//				Sets the movement speed, alert, hearing and vision ranges.
//				
// @param	AIState		new mental state which the guard is transitioning into.
//
// @return	n/a
void Guard::TransitionMentalTo(AIMental newMental)
{
	switch (newMental)
	{
		case AIMental::CALM:
			//Transition:
			m_mental = AIMental::CALM;

			//Speed and delays:
			m_moveSpeed = 75;
			m_waitDelay = 2.0f;
			m_alertDecay = 0.7f;

			//Idle timer:
			m_currTimer = m_waitDelay;

			//Hearing Range:
			m_hearingDist.x = 80;
			m_hearingDist.y = 30;

			//Sight Range:
			m_sightRange.x = 250;
			m_sightRange.y = 60;
			break;

		case AIMental::ALERT:
			//Transition:
			m_mental = AIMental::ALERT;

			//Speed and delays:
			m_moveSpeed = 200;
			m_waitDelay = 1.0f;
			m_currTimer = m_waitDelay;

			//Hearing Range:
			m_hearingDist.x = 160;
			m_hearingDist.y = 50;

			//Alert:
			m_alertLevel = 5;
			m_alertDecay = 0.3f;

			//Sight Range:
			m_sightRange.x = 300;
			m_sightRange.y = 70;
			break;

		case AIMental::ENGAGED:
			//Transition:
			m_mental = AIMental::ENGAGED;

			//Speed and delays:
			m_moveSpeed = 300;
			m_waitDelay = 0.5f;
			m_currTimer = m_waitDelay;

			//Hearing Range:
			m_hearingDist.x = 160;
			m_hearingDist.y = 50;

			//Alert:
			m_alertLevel = 10;
			m_alertDecay = 0.0f;

			//Sight Range:
			m_sightRange.x = 400;
			m_sightRange.y = 80;
			break;
	}
}

// UnProcess( )
//
// Description:	Reverts the previous position change since the last process call.				
//
// @return	n/a
void Guard::UnProcess()
{
	m_position.x = m_prevLoc.x;
	m_position.y = m_prevLoc.y;
}

// ApplyGravity( float _grav )
//
// Description:	Adjusts the velocity of the guard based on the gravity provided.
//				
// @param	float	Gravity being applied to the guard.
//
// @return	n/a
void Guard::ApplyGravity(float _grav)
{
	if (m_isGrounded)
	{
		m_velocity.y = 0;
	}
	else
	{
		m_velocity.y += _grav;
	}
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this guard.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool Guard::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	bool cond1 = m_hitbox.left + m_hitbox.width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > m_hitbox.left;
	bool cond3 = m_hitbox.top + m_hitbox.height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > m_hitbox.top;

	return cond1 && cond2 && cond3 && cond4;
}

// IsBelowOf( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping an invisible hitbox just above this guard.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool Guard::IsBelowOf(const sf::IntRect& otherDiment) const
{
	int width = m_width;
	int height = 1; //pixels above
	sf::Vector2f pos1(static_cast<float>(m_hitbox.left), static_cast<float>(m_hitbox.top - height));

	bool cond1 = pos1.x + width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > pos1.x;
	bool cond3 = pos1.y + height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > pos1.y;

	return cond1 && cond2 && cond3 && cond4;
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this box		
//
// @return	sf::IntRect		Hitbox of this box.
const sf::IntRect Guard::GetHitBox() const
{
	return m_hitbox;
}

// GetSize( )
//
// Description:	Returns the width and height of this box		
//
// @return	sf::Vector2f	Size of this box.
const sf::Vector2f Guard::GetSize() const
{
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}

// SetGrounded( bool)
//
// Description:	Updates the grounded state of the box.		
//
// @param	boolean		New grounded state of the box.
//
// @return	n/a.
void Guard::SetGrounded(bool _isGrounded, Platform* e)
{
	m_isGrounded = _isGrounded;
	m_pGround = e;
}

// GetATargetLocation()
//
// Description:	Depending on the mental state, the guard either gets the
//				next patrol location or a random location on the platform below.
//
// @return	n/a.
void Guard::GetATargetLocation()
{
	if (m_pGround != 0)
	{
		switch (m_mental)
		{
			default:
			case Guard::CALM:
				//Get Patrol point:
				if (m_targLoc == m_pGround->GetStartPoint() || m_targLoc == m_pGround->GetEndPoint())
				{
					if (m_targLoc != m_pGround->GetStartPoint()) //Already been at the end
					{
						m_targLoc = m_pGround->GetStartPoint();
					}
					else //Already been at the start
					{
						m_targLoc = m_pGround->GetEndPoint();
					}
				}
				else
				{	
					m_targLoc = m_pGround->GetStartPoint();
				}
				break;
			case Guard::ALERT:
				//Get random location:
				m_targLoc = m_pGround->GetPointAboveFrom(m_position, static_cast<float>(m_width));
				break;
			case Guard::ENGAGED:
				if (m_pTarget != 0)
				{
					//Get Projected point of the player.
					m_pGround->GetProjectedPoint(m_pTarget->GetWorldPosition());
				}
				//else Do nothing
				break;
		}
	}
}

// IsWithinVision(const sf::IntRect&)
//
// Description:	Checks if the other is overlapping the vision cone.
//
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		status of the overlapping
bool Guard::IsWithinVision(const sf::IntRect& otherDiment) const
{
	sf::Vector2f pos = m_pVisionShape->getPosition();
	sf::Vector2f siz = sf::Vector2f(static_cast<float>(m_pVisionShape->getTexture()->getSize().x), static_cast<float>(m_pVisionShape->getTexture()->getSize().y));
	siz.x *= m_sightRange.x / m_visionSize.x;
	siz.y *= m_sightRange.y / m_visionSize.y;
	
	if (m_direction < 0)
	{
		pos.x -= siz.x;
	}

	bool cond1 = pos.x + siz.x > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > pos.x;
	bool cond3 = pos.y + siz.y > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > pos.y;

	return cond1 && cond2 && cond3 && cond4;
}

// HandleNoise()
//
// Description:	Checks with the noise system to see if a sound was within range
//				of the guard's ears.
//
// @return	n/a
void Guard::HandleNoise()
{
	//Prompt the NoiseSystem to get he loudest heard sound.
	Noise* noise = NoiseSystem::GetInstance().HasHeardANoise(sf::Vector2f(m_position.x + m_width / 2, m_position.y + m_height / 8), m_hearingDist);
	
	if (noise != 0) //If the noise exists
	{
		switch (m_mental)
		{
		default:
		case Guard::CALM:
			Alert(noise->worldPos); //Transition to alert.
			break;
		case Guard::ALERT: 
			//Set target location to the noise.
			if (m_pGround != 0)
			{
				m_targLoc = m_pGround->GetProjectedPoint(noise->worldPos);
			}
			else
			{
				m_targLoc = noise->worldPos;
			}
			break;
		case Guard::ENGAGED:
			//Ignore sound.
			break;
		}
	}
}

// HandleSight()
//
// Description:	Checks if the target is overlapping the vision cone.
//
// @return	n/a
void Guard::HandleSight()
{
	if (m_pTarget != 0)
	{
		//Cast target to player (only possible target)
		Player* player = dynamic_cast<Player*>(m_pTarget);

		if (!this->IsWithinVision(m_pTarget->GetHitBox()))
		{
			//Lost sight, transition to alert:
			TransitionMentalTo(AIMental::ALERT);
			TransitionStateTo(AIState::MOVING);

			//Lose target
			m_pTarget = 0;
		}
		else if (m_pGround != 0)
		{
			//Update target location to the player's position
			m_targLoc = m_pGround->GetProjectedPoint(m_pTarget->GetWorldPosition());
		}
	}
}

// CalmDown()
//
// Description:	Force the guard to transition to a calm state.
//
// @return	n/a
void  Guard::CalmDown()
{
	if (m_mental != AIMental::CALM)
	{
		TransitionMentalTo(AIMental::CALM);
		TransitionStateTo(AIState::IDLE);
	}
}

// Alert(sf::Vector2f)
//
// Description:	Force the guard to transition to a Alert state.
//
// @param	sf::Vector2f	Location to alert to.
//
// @return	n/a
void Guard::Alert(sf::Vector2f sourceLoc)
{
	if (m_mental != AIMental::ALERT)
	{
		TransitionMentalTo(AIMental::ALERT);
		TransitionStateTo(AIState::MOVING);

		//Cast m_pTarget to player
		Player* player = dynamic_cast<Player*>(m_pTarget);
		if (player != 0) //If cast succeded.
		{
			if (m_alertSoundDelay <= 0 && !player->IsPlayerDying()) //Play huh:
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0, 1, "assets/GuardAlert.wav", SourceType::GUARD);
		}
		else
		{
			if (m_alertSoundDelay <= 0) //Play huh:
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0, 1, "assets/GuardAlert.wav", SourceType::GUARD);
		}
		
		m_targLoc = sourceLoc;
	}
}

// Detected(sf::Vector2f)
//
// Description:	Force the guard to transition to an Engaged state.
//
// @param	Entity*		Offender that has been detected.
//
// @return	n/a
void Guard::Detected(Entity* offender)
{
	if (m_mental != AIMental::ENGAGED)
	{
		m_pTarget = offender;

		//Play sound:
		NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0f), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0, 1, "assets/GuardEngaged.wav", SourceType::GUARD);
		TransitionMentalTo(AIMental::ENGAGED);
		TransitionStateTo(AIState::ATTACK);
	}
}

// Detected()
//
// Description:	Force the guard to transition to an Idle state (due to hitting a wall).
//
// @return	n/a
void Guard::Halt()
{
	TransitionStateTo(AIState::IDLE);
	m_direction *= -1; //Switch direction.
}

// DrawHearing( BackBuffer& )
//
// Description:	Used for debugging, draws the hearing radius into the world.
//
// @param	BackBuffer&	   BackBuffer to draw the hearing to.
//
// @return	n/a
void Guard::DrawHearing(BackBuffer& buffer)
{
	buffer.SetColour(sf::Color::Red);
	buffer.DrawEllipse(m_position.x + m_width / 2 - m_hearingDist.x, m_position.y + m_height / 8 - m_hearingDist.y, m_hearingDist.x, m_hearingDist.y);
}

// DrawSight( BackBuffer& )
//
// Description:	Used for debugging, draws the sight cone into the world.
//
// @param	BackBuffer&	   BackBuffer to draw the sight to.
//
// @return	n/a
void  Guard::DrawSight(BackBuffer& buffer)
{
	buffer.Draw(*m_pVisionShape);
}

// DrawPatrol( BackBuffer& )
//
// Description:	Used for debugging, draws the target location into the world.
//
// @param	BackBuffer&	   BackBuffer to draw the target location to.
//
// @return	n/a
void Guard::DrawPatrol(BackBuffer& buffer)
{
	buffer.SetColour(sf::Color::Red);
	buffer.DrawFillCircle(m_targLoc.x, m_targLoc.y, 3);
}

// PushGuard( sf::Vector2f )
//
// Description: Used with collision to force the guard in a direction.
//
// @param	sf::Vector2f&	   Push the guard in a direction.
//
// @return	n/a
void Guard::PushGuard(sf::Vector2f _vec)
{
	m_position.x += _vec.x;
	m_position.y += _vec.y;
	m_hitbox.left = static_cast<int>(m_position.x);
	m_hitbox.top = static_cast<int>(m_position.y);
}

// ScaleVision( )
//
// Description: Scale the vision cone to the new sight range.
//
// @return	n/a
void Guard::ScaleVision()
{
	m_pVisionShape->setScale((m_sightRange.x / m_visionSize.x) * m_direction, m_sightRange.y / m_visionSize.y);
}