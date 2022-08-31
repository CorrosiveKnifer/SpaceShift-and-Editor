//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Playform.cpp
// Description   	: A physical platform for the player to stand on.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "Platform.h"

//Constructor
Platform::Platform()
	: m_width(0), m_height(0)
	, m_render(0)
{

}

//Destructor
Platform::~Platform()
{

}

// Initialise( std::string platformCode )
//
// Description:	Uses string manipulation to produce a platform. This function MUST
//				be called before using any of the other functions.
//
// @param	std::string		String in the form of ("0x0y100w100h"). Assumes perfect information
//
// @return	boolean		Status of the initialisation.
//
bool Platform::Initialise(std::string platformCode) //TODO: what if imperfect info
{
	std::string x, y, w, h;
	int pos[4];
	pos[0] = platformCode.find_first_of('x');
	pos[1] = platformCode.find_first_of('y');
	pos[2] = platformCode.find_first_of('w');
	pos[3] = platformCode.find_first_of('h');

	x = platformCode.substr(0, pos[0]);
	y = platformCode.substr(pos[0] + 1 , pos[1] - pos[0] - 1);
	w = platformCode.substr(pos[1] + 1, pos[2] - pos[1] - 1);
	h = platformCode.substr(pos[2] + 1, pos[3] - pos[2] - 1);
	
	pos[0] = std::atoi(x.c_str());
	pos[1] = std::atoi(y.c_str());
	pos[2] = std::atoi(w.c_str());
	pos[3] = std::atoi(h.c_str());

	m_width = pos[2];
	m_height = pos[3];

	return Initialise(sf::IntRect(pos[0], pos[1], pos[2], pos[3]));
}

// Initialise( sf::IntRect _info )
//
// Description: Initialises a platform. This function MUST
//				be called before using any of the other functions.
//
// @param	sf::Drawable*	main body of the entity which is drawable.
//
// @return	boolean		Status of the initialisation.
//
bool Platform::Initialise(sf::IntRect _info)
{
	m_render = new sf::RectangleShape(sf::Vector2f(static_cast<float>(_info.width), static_cast<float>(_info.height)));
	this->m_position = sf::Vector2f(static_cast<float>(_info.left), static_cast<float>(_info.top));
	m_render->setFillColor(sf::Color::White);
	return Entity::Initialise(m_render);
}

// Deinitialise( )
//
// Description:	Clears the memory used by the platform to be reused.
//				MUST be called before deletion.
//
// @return	boolean		Status of the Deinitialisation.
//
bool Platform::Deinitialise()
{
	return Entity::Deinitialise();
}

// Draw( BackBuffer& )
//
// Description:	Draws the platform to the back buffer provided.
//
// @param	BackBuffer&	   BackBuffer to draw the platform to.
//
// @return	n/a
//
void Platform::Draw(BackBuffer& buffer)
{
	buffer.DrawAt(*m_pDrawable, m_position);
}

// Process( float )
//
// Description:	Process the current state of the platform since the last call.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void Platform::Process(float dT)
{
	//Do nothing.
}

// ApplyGravity( float _grav )
//
// Description:	Adjusts the velocity of the platform based on the gravity provided.
//				
// @param	float	Gravity being applied to the platform.
//
// @return	n/a
//
void Platform::ApplyGravity(float _grav)
{
	//Do nothing
}

// IsCollidingWith( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping with this platform.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool Platform::IsCollidingWith(const sf::IntRect& otherDiment) const
{
	bool cond1 = m_position.x + m_width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > m_position.x;
	bool cond3 = m_position.y + m_height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > m_position.y;

	return cond1 && cond2 && cond3 && cond4;
}

// IsBelowOf( const sf::IntRect& )
//
// Description:	Calculates using Rect vs. Rect collision if the other
//				is overlapping an invisible hitbox just above this platform.
//				
// @param	sf::IntRect&	Hitbox of the other.
//
// @return	boolean		If an overlap occured.
//
bool Platform::IsBelowOf(const sf::IntRect& otherDiment) const
{
	int width = m_width;
	int height = 2; //pixels above
	sf::Vector2f pos1(m_position.x, m_position.y - height);

	bool cond1 = pos1.x + width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > pos1.x;
	bool cond3 = pos1.y + height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > pos1.y;

	return cond1 && cond2 && cond3 && cond4;
}

// GetSize( )
//
// Description:	Returns the width and height of this box		
//
// @return	sf::Vector2f	Size of this box.
//
const sf::Vector2f Platform::GetSize() const
{
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}

// GetHitBox( )
//
// Description:	Returns the hitbox of this box		
//
// @return	sf::IntRect		Hitbox of this box.
//
const sf::IntRect Platform::GetHitBox() const
{
	return sf::IntRect(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_width, m_height);
}

// GetRandomPointAbove( )
//
// Description:	Returns a random point within the platform.	
//
// @return	sf::Vector2f		a random point within the platform.
//
sf::Vector2f Platform::GetRandomPointAbove()
{
	return sf::Vector2f(m_position.x + (rand() % m_width), m_position.y); //0 - (w - 1)
}

// GetPointAboveFrom( sf::Vector2f, float )
//
// Description:	With the provided parameters, it returns a more selective,
//				random point within the platform. Providing a point which is
//				further away from the entity.
//
//	@param	sf::Vector2f	The position of the entity.
//	@param	float			Minimum distance away from the entity.
//
// @return	sf::Vector2f		a random point within the platform.
//
sf::Vector2f Platform::GetPointAboveFrom(sf::Vector2f pos, float minDist)
{
	//If the platform is too small:
	if (minDist > m_width)
	{
		return pos; //Don't Move
	}
	//If the position is too close to the left edge: 
	if (m_position.x > pos.x - minDist)
	{
		//Can not go left
		int newWidth = static_cast<int>(m_width - minDist);
		return sf::Vector2f(m_position.x + (rand() % newWidth) + minDist, m_position.y);
	}

	//If the position is too close to the right edge: 
	if (m_position.x + m_width < pos.x + minDist)
	{
		//Can not go right
		int newWidth = static_cast<int>((pos.x - m_position.x) - minDist);
		return sf::Vector2f(m_position.x + (rand() % newWidth), m_position.y);
	}
	
	//Move towards the direction with the most space.
	if (std::pow(pos.x - m_position.x, 2) < std::pow(m_position.x + m_width - pos.x, 2))
	{
		int newWidth = static_cast<int>(m_width - minDist);
		return sf::Vector2f(m_position.x + (rand() % newWidth) + minDist, m_position.y);
	}
	else
	{
		int newWidth = static_cast<int>((pos.x - m_position.x) - minDist);
		return sf::Vector2f(m_position.x + (rand() % newWidth), m_position.y);
	}

	return GetRandomPointAbove();
}

// GetProjectedPoint( sf::Vector2f )
//
// Description:	Projects the point provided onto the platform.
//
//	@param	sf::Vector2f	The position of the entity to be projected.
//
// @return	sf::Vector2f	The projected point
//
sf::Vector2f Platform::GetProjectedPoint(sf::Vector2f pos)
{
	if (pos.x < m_position.x)
	{
		//Position is off the left side.
		return GetStartPoint();
	}
	else if (pos.x > m_position.x + m_width)
	{
		//Position is off the right side.
		return GetEndPoint();
	}
	else
	{
		//Projected point
		return sf::Vector2f(pos.x, m_position.y);
	}
}

// GetStartPoint( )
//
// Description:	Returns the start location of the platform.
//
// @return	sf::Vector2f	the start location of the platform
//
sf::Vector2f Platform::GetStartPoint()
{
	return m_position; 
}

// GetEndPoint( )
//
// Description:	Returns the end location of the platform.
//
// @return	sf::Vector2f	the end location of the platform
//
sf::Vector2f Platform::GetEndPoint()
{
	return sf::Vector2f(m_position.x + m_width, m_position.y);
}

// GetCollider( const sf::IntRect& )
//
// Description:	Gets the collision type using Rect vs. Rect collision.
//
// @param	 sf::IntRect&	Hitbox of the other.
//
// @return	 ColliderType	type of collision.
//
Entity::ColliderType Platform::GetCollider(const sf::IntRect& otherDiment)
{
	float offset = 2.0;

	sf::FloatRect hitBoxes[4];
	//Construct Hitboxes:
	hitBoxes[0] = sf::FloatRect(m_position.x, static_cast<float>(m_position.y - offset), static_cast<float>(m_width), offset); //North lowest priority
	hitBoxes[1] = sf::FloatRect(static_cast<float>(m_position.x + m_width), m_position.y, offset, static_cast<float>(m_height)); //East
	hitBoxes[2] = sf::FloatRect(static_cast<float>(m_position.x - offset), m_position.y, offset, static_cast<float>(m_height)); //West
	hitBoxes[3] = sf::FloatRect(m_position.x, static_cast<float>(m_position.y + m_height), static_cast<float>(m_width), offset); //South
	
	for (int i = 1; i < 4; i++) //All but north first
	{
		bool cond1 = hitBoxes[i].left + hitBoxes[i].width > otherDiment.left;
		bool cond2 = otherDiment.left + otherDiment.width > hitBoxes[i].left;
		bool cond3 = hitBoxes[i].top + hitBoxes[i].height > otherDiment.top;
		bool cond4 = otherDiment.top + otherDiment.height > hitBoxes[i].top;

		if (cond1 && cond2 && cond3 && cond4)
		{
			return static_cast<ColliderType>(i + 1);
		}
	}

	//For North:
	bool cond1 = hitBoxes[0].left + hitBoxes[0].width > otherDiment.left;
	bool cond2 = otherDiment.left + otherDiment.width > hitBoxes[0].left;
	bool cond3 = hitBoxes[0].top + hitBoxes[0].height > otherDiment.top;
	bool cond4 = otherDiment.top + otherDiment.height > hitBoxes[0].top;

	if (cond1 && cond2 && cond3 && cond4)
	{
		return static_cast<ColliderType>(1);
	}

	return ColliderType::NONE; //No collision
}

// DrawHitboxes( BackBuffer& )
//
// Description:	Draw all the invisible hitboxes of this platform.
//
// @param	BackBuffer&	   BackBuffer to draw the hitboxes to.
//
// @return	n/a
//
void Platform::DrawHitboxes(BackBuffer& buffer)
{
	buffer.SetColour(sf::Color::Red);
	float offset = 2;

	sf::FloatRect hitBoxes[4];
	//Construct Hitboxes:
	hitBoxes[0] = sf::FloatRect(m_position.x, static_cast<float>(m_position.y - offset), static_cast<float>(m_width), offset); //North lowest priority
	hitBoxes[1] = sf::FloatRect(static_cast<float>(m_position.x + m_width), m_position.y, offset, static_cast<float>(m_height)); //East
	hitBoxes[2] = sf::FloatRect(static_cast<float>(m_position.x - offset), m_position.y, offset, static_cast<float>(m_height)); //West
	hitBoxes[3] = sf::FloatRect(m_position.x, static_cast<float>(m_position.y + m_height), static_cast<float>(m_width), offset); //South

	for (int i = 0; i < 4; i++)
	{
		buffer.DrawRectangle(hitBoxes[i].left, hitBoxes[i].top, hitBoxes[i].width, hitBoxes[i].height);
	}
}