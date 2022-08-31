//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Player.cpp
// Description   	: A class to handle the player within the world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "Player.h"

//Local Includes
#include "SpriteMap.h"
#include "Box.h"
#include "NoiseSystem.h"

//Library Includes
#include <cmath>

//Constructor
Player::Player()
	: m_pAnim(0)
	, m_width(0), m_height(0), m_hitbox(0, 0, 0, 0)
	, m_isGrounded(false), m_isWalking(false), m_isDead(false)
	, m_isTouchWallLeft(false), m_isTouchWallRight(false)
	, m_prevLoc(0, 0), m_pBox(0)
	, m_soundDelay(0), m_targetVelocityX(0)
	, m_direction(1)
	, m_state(IDLE)
{

}

//Destructor
Player::~Player()
{

}

// Initialise( BackBuffer& ,  std::string)
//
// Description:	Sets up the Player's values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	BackBuffer&		Backbuffer to create the sprite from.
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
//
// @return	boolean		Status of the initialisation.
//
bool Player::Initialise(BackBuffer& buffer, std::string spawnCode)
{
	//Destruct the spawn code:
	int pos0 = spawnCode.find_first_of('x');
	int pos1 = spawnCode.find_first_of('y');
	std::string x = spawnCode.substr(0, pos0);
	std::string y = spawnCode.substr(pos0 + 1, pos1 - pos0 - 1);
	m_position.x = static_cast<float>(std::atoi(x.c_str()));
	m_position.y = static_cast<float>(std::atoi(y.c_str()));

	//Create sprite:
	sf::Sprite* source = buffer.CreateSprite("assets/player.png");
	source->setScale(0.3f, 0.3f);

	int animCol = 8;
	int animRows = 7;

	m_width = static_cast<int>((source->getTexture()->getSize().x / animCol) * 0.3f);
	m_height = static_cast<int>((source->getTexture()->getSize().y / animRows) * 0.3f);

	m_hitbox.width = 20;
	m_hitbox.height = 58;

	//Create Animation:
	m_pAnim = new SpriteMap();
	m_pAnim->Initialise(source, animCol, animRows);
	TransitionTo(AnimState::IDLE);

	return Entity::Initialise(source);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the player to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Player::Deinitialise()
{
	if (m_pAnim != 0)
	{
		delete m_pAnim;
		m_pAnim = 0;
	}
	if (m_pBox != 0)
	{
		delete m_pBox;
		m_pBox = 0;
	}
	return Entity::Deinitialise();
}

// Draw( BackBuffer& )
//
// Description:	Draws the player to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the player to.
//
// @return	n/a
//
void Player::Draw(BackBuffer& buffer)
{
	//Calculate matrix for direction:
	sf::Transform trans = sf::Transform::Identity;
	trans.translate(static_cast<float>(m_width / 2.0f + m_position.x), static_cast<float>(m_height / 2.0f + m_position.y));
	trans.scale(static_cast<float>(-m_direction), 1);
	trans.translate(static_cast<float>(-m_width / 2.0f), static_cast<float>(-m_height / 2.0f));

	//Draw Player:
	buffer.PushTransform(trans);
	buffer.Draw(*m_pDrawable);
	buffer.SetColour(sf::Color::Black);
	buffer.PopTransform(trans);
}

// Process( float )
//
// Description:	Process the current state of the player since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void Player::Process(float dT)
{
	//Animation:
	AnimationLogic();
	m_pAnim->ProcessX(dT);
	
	if (!m_isDead) //Player is not dead:
	{
		//Lerp velocity:
		float lerpVal = m_targetVelocityX != 0 ? 0.2f : 0.6f;
		if (m_isGrounded)
		{
			m_velocity.x = m_velocity.x + ((m_targetVelocityX - m_velocity.x)  * lerpVal);
		}
		else
		{
			m_velocity.x = m_velocity.x + ((m_targetVelocityX / 2 - m_velocity.x)  * lerpVal);
		}

		//Delay timers:
		if(m_soundDelay > 0)
			m_soundDelay -= dT;

		//Create walking/Running sound
		if (m_isWalking && m_isGrounded)
		{
			//Create walking sound
			if (m_targetVelocityX != 0 && m_soundDelay <= 0)
			{
				m_soundDelay = 0.6f;
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.15f, 1, "assets/footstep.wav", SourceType::PLAYER);
			}
			else if (m_targetVelocityX != 0)
			{
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.15f, 1, "", SourceType::PLAYER);
			}
		}
		else if (m_isGrounded)
		{
			if (m_targetVelocityX != 0 && m_soundDelay <= 0)
			{
				m_soundDelay = 0.6f;
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.5f, 1, "assets/Running.wav", SourceType::PLAYER);
			}
			else if (m_targetVelocityX != 0)
			{
				NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2.0f), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.5f, 1, "", SourceType::PLAYER);
			}
		}

		//Update previous location
		m_prevLoc.x = m_position.x;
		m_prevLoc.y = m_position.y;

		//Update position
		m_position.x += m_velocity.x * dT;
		m_position.y += m_velocity.y * dT;

		//Update Hitbox
		m_hitbox.left = static_cast<int>(m_position.x + m_width / 2 - m_hitbox.width / 2);
		m_hitbox.top = static_cast<int>(m_position.y + m_height - m_hitbox.height);
	}
}

// UnProcessX( )
//
// Description:	Reverts the previous position change in the x-axis,
//				since the last process call.				
//
// @return	n/a
void Player::UnProcessX()
{
	m_position.x = m_prevLoc.x;
}

// UnProcessY( )
//
// Description:	Reverts the previous position change in the y-axis,
//				since the last process call.				
//
// @return	n/a
void Player::UnProcessY()
{
	m_position.y = m_prevLoc.y;
}

// AnimationLogic( )
//
// Description:	Process the current state of the player animation 
//				since the last call.
//
// @return	n/a
void Player::AnimationLogic()
{
	if (m_state == AnimState::DEATH)
	{
		//Do nothing
		return;
	}

	//Moving Animation:
	if (std::abs(m_velocity.x) > 0.5 && m_isGrounded)
	{
		if (m_state != AnimState::MOVING)
		{
			TransitionTo(AnimState::MOVING);
		}
		
	} //Jumping Animation:
	else if (!m_isGrounded && m_velocity.y < 0.5)
	{
		if (m_state != AnimState::JUMPING)
		{
			TransitionTo(AnimState::JUMPING);
		}
		
	} //Falling Animation:
	else if (!m_isGrounded && m_velocity.y > 0.5)
	{
		if (m_state != AnimState::FALLING)
		{
			TransitionTo(AnimState::FALLING);
		}
	} //Landing Animation
	else if (m_isGrounded && (m_state == AnimState::FALLING || m_state == Player::AnimState::LANDING))
	{
		if (m_pAnim->IsLooping())
		{
 			TransitionTo(AnimState::LANDING);
		}
		else if (m_pAnim->IsAnimationFinished())
		{
			TransitionTo(AnimState::IDLE);
			NoiseSystem::GetInstance().CreateNoise(sf::Vector2f(static_cast<float>(m_hitbox.left + m_hitbox.width / 2), static_cast<float>(m_hitbox.top + m_hitbox.height)), 0.5f, 1, "assets/Landing.wav", SourceType::PLAYER);
		}
	}
	else //Idle Animation
	{
		if (m_state != AnimState::IDLE)
		{
			TransitionTo(AnimState::IDLE);
		}
	}
}

// TransitionTo( AnimState )
//
// Description:	Sets up all the conditions for a animation states.
//				Sets the animation frame, velocity and anim delay.
//				
// @param	AnimState	new animation state which the player is transitioning into.
//
// @return	n/a
void Player::TransitionTo(AnimState newState)
{
	switch (newState)
	{
	default:
	case Player::AnimState::IDLE:
		//Transition:
		m_state = Player::AnimState::IDLE;

		//Animation changes:
		m_pAnim->SetIsLooping(true);
		m_pAnim->SetLoopingBoundsX(0, 7);
		m_pAnim->SetIndexX(0);
		m_pAnim->SetIndexY(0);
		m_pAnim->SetDelay(0.2f, 0);
		break;
	case  Player::AnimState::MOVING:
		//Transition:
		m_state = Player::AnimState::MOVING;

		if (m_isWalking)
		{
			//Animation changes:
			m_pAnim->SetIsLooping(true);
			m_pAnim->SetLoopingBoundsX(0, 5);
			m_pAnim->SetIndexX(0);
			m_pAnim->SetIndexY(1);
			m_pAnim->SetDelay(0.2f, 0);
		}
		else //running
		{
			//Animation changes:
			m_pAnim->SetIsLooping(true);
			m_pAnim->SetLoopingBoundsX(0, 7);
			m_pAnim->SetIndexX(0);
			m_pAnim->SetIndexY(2);
			m_pAnim->SetDelay(0.1f, 0);
		}
		break;
	case  Player::AnimState::FALLING:
		//Transition:
		m_state = Player::AnimState::FALLING;

		//Animation changes:
		m_pAnim->SetIsLooping(true);
		m_pAnim->SetLoopingBoundsX(0, 7);
		m_pAnim->SetIndexX(0);
		m_pAnim->SetIndexY(4);
		m_pAnim->SetDelay(0.1f, 0);
		break;
	case  Player::AnimState::LANDING:
		//Transition:
		m_state = Player::AnimState::LANDING;

		//Animation changes:
		m_pAnim->SetIsLooping(false);
		m_pAnim->SetLoopingBoundsX(0, 7);
		m_pAnim->SetIndexX(0);
		m_pAnim->SetIndexY(5);
		m_pAnim->SetDelay(0.05f, 0);
		break;
	case  Player::AnimState::JUMPING:
		//Transition:
		m_state = Player::AnimState::JUMPING;
		//Animation changes:
		m_pAnim->SetIsLooping(true);
		m_pAnim->SetLoopingBoundsX(0, 7);
		m_pAnim->SetIndexX(0);
		m_pAnim->SetIndexY(3);
		m_pAnim->SetDelay(0.1f, 0);
		
		break;
	case  Player::AnimState::DEATH:
		//Transition:
		m_state = Player::AnimState::DEATH;

		//Animation changes:
		m_pAnim->SetIsLooping(false);
		m_pAnim->SetLoopingBoundsX(0, 7);
		m_pAnim->SetIndexX(0);
		m_pAnim->SetIndexY(6);
		m_pAnim->SetDelay(0.15f, 0);
		break;
	}
}

// SetWalking( bool )
//
// Description:	Updates the player if they are walking (by pressing the l.shift)
//				
// @param	boolean 	new state of walking.
//
// @return	n/a
void Player::SetWalking(bool _isWalking)
{
	if (m_isWalking != _isWalking && m_state == AnimState::MOVING)
	{
		m_isWalking = _isWalking;
		TransitionTo(AnimState::MOVING);
		return;
	}
	m_isWalking = _isWalking;
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this player.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool Player::IsCollidingWith(const sf::IntRect& otherDiment) const
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
//				is overlapping an invisible hitbox just above this player.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool Player::IsBelowOf(const sf::IntRect& otherDiment) const
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

// GetCenterPosition()
//
// Description:	Calculates the center position of the player.				
//
// @return	sf::Vector2f	Center position of the player.
//
const sf::Vector2f Player::GetCenterPosition() const
{
	return sf::Vector2f(m_position.x + m_width/2, m_position.y + m_height / 2);
}

// Move( int)
//
// Description:	Updates the target velocity and direction of the player.
//				Where -1 is left and 1 is right.
//
// @param	int		new direction of the player.
//
// @return	n/a
//
void Player::Move(int _x)
{
	if (_x != 0 && !m_isDead)
	{
		m_direction = _x;
	}

	if (m_isWalking)
	{
		m_targetVelocityX = 150.0f * _x;
	}
	else
	{
		m_targetVelocityX = 300.0f * _x;
	}
}

// Jump( float )
//
// Description:	Activates the player's jump with the power provided.
//
// @param	float	percentage of power within the jump.
//
// @return	n/a
//
void Player::Jump(float powerCent)
{
	if (m_isGrounded)
	{
		m_velocity.y = -400 * powerCent;
	}
}

// IsGrounded( )
//
// Description:	Returns the current state of if the player is grounded.
//
// @return	boolean		is the player is grounded?
//
bool Player::IsGrounded()
{
	return m_isGrounded;
}

// ApplyGravity( float _grav )
//
// Description:	Adjusts the velocity of the player based on the gravity provided.
//				
// @param	float	Gravity being applied to the player.
//
// @return	n/a
//
void Player::ApplyGravity(float _grav)
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

// SetGrounded( bool)
//
// Description:	Updates the grounded state of the player.		
//
// @param	boolean		New grounded state of the player.
//
// @return	n/a.
void Player::SetGrounded(bool _isGrounded)
{
	m_isGrounded = _isGrounded;
}

// GetSize( )
//
// Description:	Returns the width and height of this player		
//
// @return	sf::Vector2f	Size of this player.
//
const sf::Vector2f Player::GetSize() const
{
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this player		
//
// @return	sf::IntRect		Hitbox of this player.
//
const sf::IntRect Player::GetHitBox() const
{
	return m_hitbox;
}

// SetTouchWall( bool)
//
// Description:	Updates the wall state of the player, preventing movement.		
//
// @param	boolean		New wall state of the player.
//
// @return	n/a.
void Player::SetTouchWall(bool _isWall)
{
	//Check which wall it must have been:
	m_isTouchWallLeft = m_velocity.x < 0 && _isWall;
	m_isTouchWallRight = m_velocity.x > 0 && _isWall;
	
	if (m_targetVelocityX < 0 && m_isTouchWallLeft)
	{
		m_targetVelocityX = 0;
	}

	if (m_targetVelocityX > 0 && m_isTouchWallRight)
	{
		m_targetVelocityX = 0;
	}
}

// PickUpBox( Box*)
//
// Description:	Stores the reference of the box on the player.		
//
// @param	Box*	new box the player has picked up.
//
// @return	n/a.
void Player::PickUpBox(Box* newBox)
{
	if (m_pBox == 0)
	{
		m_pBox = newBox;
	}
}

// DropBox( )
//
// Description:	removes reference of the box from the player into the world.		
//
// @return	Box*	The box the player has dropped.
Box* Player::DropBox()
{
	Box* temp = m_pBox;

	//Calculate new position of the box:
	sf::Vector2f newPos = this->GetCenterPosition();
	if (m_direction < 0)
	{
		newPos.x += (m_pBox->GetSize().x / 2) * m_direction;
	}
	else
	{
		newPos.x -= (m_pBox->GetSize().x / 2) * m_direction;
	}

	//Update box to this position:
	m_pBox->SetWorldPosition(newPos);

	//Lose box
	m_pBox = 0;

	return temp;
}

// EmptyHanded( )
//
// Description:	Returns if the player does not have a box.
//
// @return	boolean		the player doesn't have a box?
//
bool Player::EmptyHanded() const
{
	return m_pBox == 0;
}

// KillPlayer( )
//
// Description:	Starts the death animation of the player.
//
// @return	n/a
//
void Player::KillPlayer()
{
	if (!m_isDead)
	{
		TransitionTo(AnimState::DEATH);
		m_isDead = true;
	}
}

// IsPlayerDying( )
//
// Description:	Returns true if the player is dead.
//
// @return	bool	if the player is dead.
//
bool Player::IsPlayerDying()
{
	if (m_isDead)
		int i = 0;
	return m_isDead;
}

// IsPlayerDead( )
//
// Description:	Returns true if the player is still in the death animation.
//
// @return	bool	player is still in the death animation
//
bool Player::IsPlayerDead()
{
	return m_isDead && m_pAnim->IsAnimationFinished();
}

// PushPlayer( sf::Vector2f )
//
// Description: Used with collision to force the player in a direction.
//
// @param	sf::Vector2f&	   Push the player in a direction.
//
// @return	n/a
void Player::PushPlayer(sf::Vector2f _vec)
{
	m_position.x += _vec.x;
	m_position.y += _vec.y;
	if(_vec.x != 0)
		m_hitbox.left = static_cast<int>(m_position.x);
	if (_vec.y != 0)
		m_hitbox.top = static_cast<int>(m_position.y);
}