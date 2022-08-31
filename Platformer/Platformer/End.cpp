//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: End.cpp
// Description   	: End location which is the goal of the player.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "End.h"

//Local Include
#include "SpriteMap.h"

//Constructor
End::End()
	: m_pAnim(0), m_width(0), m_height(0), m_hitbox(0, 0, 0, 0)
{

}

//Destructor
End::~End()
{

}

// Initialise( BackBuffer& ,  std::string)
//
// Description:	Sets up the Ends' values for use. This function MUST
//				be called before using any of the other functions.
//				Note: Spawn Codes can not contain negatives.
//
// @param	BackBuffer&		Backbuffer to create the sprite from.
// @param	std::string		Spawn code for the starting position (e.g 18x30y).
//
// @return	boolean		Status of the initialisation.
//
bool End::Initialise(BackBuffer& buffer, std::string spawnCode)
{
	//Destruct the spawn code:
	int pos0 = spawnCode.find_first_of('x');
	int pos1 = spawnCode.find_first_of('y');
	std::string x = spawnCode.substr(0, pos0);
	std::string y = spawnCode.substr(pos0 + 1, pos1 - pos0 - 1);
	m_position.x = static_cast<float>(std::atoi(x.c_str()));
	m_position.y = static_cast<float>(std::atoi(y.c_str()));

	//Create sprite:
	sf::Vector2f scale(0.3f, 0.3f);
	sf::Sprite* source = buffer.CreateSprite("assets/portal.png");
	m_pAnim = new SpriteMap();
	m_pAnim->Initialise(source, 9, 1);
	m_pAnim->SetDelay(0.08f, 0);
	m_pAnim->SetIsLooping(true);

	source->setScale(scale);

	m_width = static_cast<int>((source->getTexture()->getSize().x / 9.0f) * scale.x);
	m_height = static_cast<int>((source->getTexture()->getSize().y) * scale.y);

	m_hitbox.left = static_cast<int>(m_position.x);
	m_hitbox.top = static_cast<int>(m_position.y);
	m_hitbox.width = m_width;
	m_hitbox.height = m_height;

	return Entity::Initialise(source);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the End to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool End::Deinitialise()
{  
	if (m_pAnim != 0)
	{
		delete m_pAnim;
		m_pAnim = 0;
	}
	return Entity::Deinitialise();
}

// Draw( BackBuffer& _backBuffer )
//
// Description:	Draws the End to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the End to.
//
// @return	n/a
//
void End::Draw(BackBuffer& buffer)
{
	if (m_position.x < 150) //Closer to the left so flip
	{
		sf::Transform trans;
		trans.translate(m_position.x + (m_width/2), m_position.y);
		trans.scale(-1, 1);
		trans.translate(-(m_position.x + (m_width / 2)), -m_position.y);

		buffer.PushTransform(trans);
		buffer.DrawAt(*m_pDrawable, m_position);
		buffer.PopTransform(trans);
	}
	else
	{
		buffer.DrawAt(*m_pDrawable, m_position);
	}
}

// Process( float dT )
//
// Description:	Process the current state of the end since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void End::Process(float dT)
{
	m_pAnim->ProcessX(dT);
}

// ApplyGravity( float _grav )
//
// Description:	Adjusts the velocity of the End based on the gravity provided.
//				
// @param	float	Gravity being applied to the box.
//
// @return	n/a
void End::ApplyGravity(float _grav)
{
	//Do Nothing
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this End.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool End::IsCollidingWith(const sf::IntRect& otherDiment) const
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
//				is overlapping an invisible hitbox just above this End.
//				Note: No interaction with this End, so it just calculates overlap.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
bool End::IsBelowOf(const sf::IntRect& otherDiment) const
{
	return IsCollidingWith(otherDiment);
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this end.		
//
// @return	sf::IntRect		Hitbox of this box.
const sf::IntRect End::GetHitBox() const
{
	return m_hitbox;
}

// GetSize( )
//
// Description:	Returns the width and height of this end.		
//
// @return	sf::Vector2f	Size of this box.
const sf::Vector2f End::GetSize() const
{
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}