//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: BackBuffer.cpp
// Description   	: A Class in charge of the window and displaying on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self include
#include "BackBuffer.h"

//Local Includes
#include "iniParser.h"

//Constructor
BackBuffer::BackBuffer()
	: m_width(0), m_height(0)
	, m_pWindow(0)
	, m_WindowPos(0, 0)
	, m_pTextureMap(0)
	, m_font(0)
	, m_fontSize(10)
	, m_fontAlign(Align::Left)
	, m_transform(sf::Transform::Identity), m_currShake(sf::Transform::Identity)
	, m_GlobalColour(sf::Color::Black)
	, m_isFullScreen(false)
{

}

//Deconstructor
BackBuffer::~BackBuffer()
{
	
}

// Initialise( float _w, float _h )
//
// Description:	Sets up the window's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	float	width of the window to be created.
// @param	float	height of the window to be created.
//
// @return	bool	Status of the initialisation.
//
bool BackBuffer::Initialise(int _w, int _h)
{
	if (_w > 0 && _h > 0)
	{
		m_width = _w;
		m_height = _h;
		m_pWindow = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "Space Shift");
		m_pWindow->setFramerateLimit(60);
		m_pWindow->setVerticalSyncEnabled(true);
		m_pTextureMap = new std::map<std::string, sf::Texture*>();
		m_WindowPos = m_pWindow->getPosition();
		

		viewport.setCenter(static_cast<float>(m_width / 2.0f), static_cast<float>(m_height / 2.0f));
		viewport.setSize(static_cast<float>(m_width), static_cast<float>(m_height));
		m_pWindow->setView(viewport);
		return true;
	}
	return false;
}

// Deinitialise( )
//
// Description:	Releases any memory the backbuffer's values for use.
//
// @param	n/a
//
// @return	bool	Status of the deinitialisation.
//
bool BackBuffer::Deinitialise()
{
	if (m_font != 0)
		delete m_font;
	m_font = 0;

	if (m_pTextureMap != 0)
	{
		DumpTextures();
		delete m_pTextureMap;
		m_pTextureMap = 0;
	}

	if (m_pWindow != 0)
	{
		m_pWindow->close();
		delete m_pWindow;
	}

	return true;
}

// CreateSprite( std::string fileLoc )
//
// Description:	Creates a new sprite with the texture of the .png provided.
//				Beware: Does create a new Sprite and may return 0 if the texture
//				was not found.
//
// @param	std::string		File Location of the .png 
//
// @return	sf::Sprite*		The new sprite created or nullptr (if texture not found).
//
sf::Sprite* BackBuffer::CreateSprite(std::string fileLoc)
{
	sf::Texture* texture;
	if (m_pTextureMap->find(fileLoc) == m_pTextureMap->end())
	{
		texture = new sf::Texture();
		if (!texture->loadFromFile(fileLoc))
		{
			//texture does not exist.
			return 0;
		}
		m_pTextureMap->insert(std::pair<std::string, sf::Texture*>(fileLoc, texture));
	}
	else
	{
		texture = m_pTextureMap->at(fileLoc);
	}

	return  new sf::Sprite(*texture);
}

// CreateTexture( std::string fileLoc )
//
// Description:	Creates a new texture of the .png provided, un attached to a sprite.
//				Beware: May return 0 if the texture
//				was not found.
//
// @param	std::string		File Location of the .png 
//
// @return	sf::Sprite*		The new sprite created or nullptr (if texture not found).
//
sf::Texture* BackBuffer::CreateTexture(std::string fileLoc)
{
	sf::Texture* texture;
	if (m_pTextureMap->find(fileLoc) == m_pTextureMap->end())
	{
		texture = new sf::Texture();
		if (!texture->loadFromFile(fileLoc))
		{
			//texture does not exist.
			return 0;
		}
		m_pTextureMap->insert(std::pair<std::string, sf::Texture*>(fileLoc, texture));
	}
	else
	{
		texture = m_pTextureMap->at(fileLoc);
	}

	return texture;
}

// PushTransform( const sf::Transform _transform )
//
// Description:	Multiplies the current transform with the one
//				specified.
//
// @param	sf::Transform	3 by 3 matrix to multiply with the current matrix.
//
// @return	n/a
//
void BackBuffer::PushTransform(const sf::Transform _transform)
{
	m_transform.combine(_transform);
}

// PopTransform( const sf::Transform _transform )
//
// Description:	Multiplies the current transform with the inverse of 
//				one	specified, to remove it.
//
// @param	sf::Transform	3 by 3 matrix to be inversed with the current matrix.
//
// @return	n/a
//
void BackBuffer::PopTransform(const sf::Transform _transform)
{
	m_transform.combine(_transform.getInverse());
}

// ExtendWindow( int dx, int dy )
//
// Description:	Extends the window in the direction specified
//				and only requires the change in window size.
//
// @param	int		Extension in the x-axis.
// @param	int		Extension in the y-axis
//
// @return	bool	If the window size is not becoming negative.
//
bool BackBuffer::ExtendWindow(int dx, int dy)
{
	if (m_width + dx > 0 && m_height + dy > 0)
	{
		float xScale = m_width / static_cast<float>(m_width + dx);
		float yScale = m_height / static_cast<float>(m_height + dy);
		m_width += dx;
		m_height += dy;

		m_pWindow->setSize(sf::Vector2u(m_width, m_height));
		
		if (xScale > 1)
		{
			viewport.setViewport(sf::FloatRect(0, 0, 1, 1));
		}
		else
		{
			viewport.setViewport(sf::FloatRect(xScale - 1, 0, xScale, 1));
		}
		

		sf::Transform scale(xScale, 0.0f, 0.0f, 0.0f, yScale, 0.0f, 0.0f, 0.0f, 1.0f);
		PushTransform(scale);
		return true;
	}
	return false;
}

// LoadFont( std::string _fontLoc )
//
// Description:	Loads a font into the backbuffer
//
// @param	std::string		File Location of the font. 
//
// @return	bool	Status of finding the .tff file.
//
bool BackBuffer::LoadFont(std::string _fontLoc)
{
	if (m_font == 0)
		m_font = new sf::Font;
	return m_font->loadFromFile(_fontLoc);
}

// SetFontSize( int _size )
//
// Description:	Changed the size of the font.
//
// @param	int		new pixel size of the font.
//
// @return	n/a
//
void BackBuffer::SetFontSize(int _size)
{
	if (_size > 0)
	{
		m_fontSize = _size;
	}
}

// SetFontAlign( Align _align )
//
// Description:	Changed the alignment of the font.
//
// @param	Align	The new alignment.
//
// @return	n/a
//
void BackBuffer::SetFontAlign(Align _align)
{
	m_fontAlign = _align;
}

// Display( )
//
// Description:	Calls the window's display function.
//
// @param	n/a
//
// @return	n/a
//
void BackBuffer::Display() const
{
	m_pWindow->display();
}

// Clear( )
//
// Description:	Calls the window's clear function.
//
// @param	n/a
//
// @return	n/a
//
void BackBuffer::Clear() const
{
	m_pWindow->clear(m_GlobalColour);
}

// Draw( const sf::Drawable& _sprite )
//
// Description:	Uses the window's draw function to render the 
//				sprite to the screen.
//
// @param	sf::Sprite&		sprite to be rendered.
//
// @return	n/a
//
void BackBuffer::Draw(const sf::Drawable& _object)
{
	m_pWindow->draw(_object, m_transform);
}

// DrawAt( const sf::Drawable& _sprite, sf::Vector2f )
//
// Description:	Used to draw a drawable object translated to a position provied.
//
// @param	sf::Sprite&		sprite to be rendered.
// @param	sf::Vector2f	Translation value.
//
// @return	n/a
//
void BackBuffer::DrawAt(const sf::Drawable& _object, sf::Vector2f _position)
{
	sf::Transform trans(1, 0, _position.x, 0, 1, _position.y, 0, 0, 1);
	
	PushTransform(trans);
	m_pWindow->draw(_object, m_transform);
	PopTransform(trans);
}
// DrawFillCircle( float x, float y, float sizeR )
//
// Description:	Uses the window's function to render a
//				circle to the screen (Filled In).
//
// @param	float	position of the circle on the x-axis.
// @param	float	position of the circle on the y-axis.
// @param	float	size of the radius, in pixels, of the circle.
//
// @return	n/a
//
void BackBuffer::DrawFillCircle(float x, float y, float sizeR)
{
	sf::CircleShape circle(sizeR);
	circle.setPosition(x, y);
	circle.setFillColor(m_GlobalColour);
	m_pWindow->draw(circle, m_transform);
}

// DrawCircle( float x, float y, float sizeR )
//
// Description:	Uses the window's function to render a
//				circle to the screen.
//
// @param	float	position of the circle on the x-axis.
// @param	float	position of the circle on the y-axis.
// @param	float	size of the radius, in pixels, of the circle.
//
// @return	n/a
//
void BackBuffer::DrawCircle(float x, float y, float sizeR)
{
	sf::CircleShape circle(sizeR);
	circle.setPosition(x, y);
	circle.setFillColor(sf::Color(0, 0, 0, 0));
	circle.setOutlineColor(m_GlobalColour);
	circle.setOutlineThickness(1);
	m_pWindow->draw(circle, m_transform);
}

// DrawEllipse( float x, float y, float sizeR_x, float sizeR_y )
//
// Description:	Uses the window's function to render an
//				ellipse to the screen.
//
// @param	float	position of the ellipse on the x-axis.
// @param	float	position of the ellipse on the y-axis.
// @param	float	size of the x-axis radius, in pixels, of the circle.
// @param	float	size of the y-axis radius, in pixels, of the circle.
//
// @return	n/a
//
void BackBuffer::DrawEllipse(float x, float y, float sizeR_x, float sizeR_y)
{
	sf::CircleShape ellipse(sizeR_x);
	ellipse.setScale(1, sizeR_y/sizeR_x);
	ellipse.setPosition(x, y);
	ellipse.setFillColor(sf::Color(0, 0, 0, 0));
	ellipse.setOutlineColor(m_GlobalColour);
	ellipse.setOutlineThickness(2);
	m_pWindow->draw(ellipse, m_transform);
}

// DrawFillEllipse( float x, float y, float sizeR_x, float sizeR_y )
//
// Description:	Uses the window's function to render an
//				ellipse to the screen (Filled In).
//
// @param	float	position of the ellipse on the x-axis.
// @param	float	position of the ellipse on the y-axis.
// @param	float	size of the x-axis radius, in pixels, of the circle.
// @param	float	size of the y-axis radius, in pixels, of the circle.
//
// @return	n/a
//
void BackBuffer::DrawFillEllipse(float x, float y, float sizeR_x, float sizeR_y)
{
	sf::CircleShape ellipse(sizeR_x);
	ellipse.setScale(1, sizeR_y / sizeR_x);
	ellipse.setPosition(x, y);
	ellipse.setFillColor(m_GlobalColour);
	m_pWindow->draw(ellipse, m_transform);
}

// DrawRectangle( float x, float y, float w, float h )
//
// Description:	Uses the window's function to render an
//				rectangle to the screen.
//
// @param	float	position of the top left corner on the x-axis.
// @param	float	position of the top left corner on the y-axis.
// @param	float	width of the rectangle in pixels.
// @param	float	height of the rectangle in pixels.
//
// @return	n/a
//
void BackBuffer::DrawRectangle(float x, float y, float w, float h)
{
	sf::RectangleShape rect;
	rect.setPosition(x, y);
	rect.setSize(sf::Vector2f(w, h));
	rect.setFillColor(m_GlobalColour);
	m_pWindow->draw(rect, m_transform);
}

// DrawTextToWorld( std::string msg, float x, float y )
//
// Description:	Uses the window's function to render the 
//				text to the world, in the alignment preloaded.
//
// @param	std::string 	Message to be rendered.
// @param	float	position of the top left corner on the x-axis.
// @param	float	position of the top left corner on the y-axis.
//
// @return	n/a
//
void BackBuffer::DrawTextToView(std::string msg, float x, float y)
{
	if (m_font != 0)
	{
		sf::Text text;
		text.setFont(*m_font);
		text.setString(msg);
		text.setCharacterSize(m_fontSize);
		text.setFillColor(m_GlobalColour);
		sf::Vector2f pos = viewport.getCenter();
		pos.x += x - viewport.getSize().x / 2;
		pos.y += y - viewport.getSize().y / 2;
		switch (m_fontAlign)
		{
		default:
		case Align::Left:
			text.setPosition(pos);
			break;
		case Align::Centre:
			text.setPosition(pos.x - text.getLocalBounds().width / 2, pos.y);
			break;
		case Align::Right:
			text.setPosition(pos.x - text.getLocalBounds().width, pos.y);
			break;

		}

		m_pWindow->draw(text, m_transform);
	}
	else
	{
		sf::RectangleShape rect;
		rect.setPosition(x, y);
		rect.setSize(sf::Vector2f(100, 50));
		rect.setFillColor(m_GlobalColour);
		m_pWindow->draw(rect, m_transform);
	}
}
// DrawTextToWorld( std::string msg, float x, float y )
//
// Description:	Uses the window's function to render the 
//				text to the world, in the alignment preloaded.
//
// @param	std::string 	Message to be rendered.
// @param	float	position of the top left corner on the x-axis.
// @param	float	position of the top left corner on the y-axis.
//
// @return	n/a
//
void BackBuffer::DrawTextToWorld(std::string msg, float x, float y)
{
	if (m_font != 0)
	{
		sf::Text text;
		text.setFont(*m_font);
		text.setString(msg);
		text.setCharacterSize(m_fontSize);
		text.setFillColor(m_GlobalColour);
		switch (m_fontAlign)
		{
		default:
		case Align::Left:
			text.setPosition(x, y);
			break;
		case Align::Centre:
			text.setPosition(x - text.getLocalBounds().width/2, y);
			break;
		case Align::Right:
			text.setPosition(x - text.getLocalBounds().width, y);
			break;

		}
		
		m_pWindow->draw(text, m_transform);
	}
	else
	{
		sf::RectangleShape rect;
		rect.setPosition(x, y);
		rect.setSize(sf::Vector2f(100, 50));
		rect.setFillColor(m_GlobalColour);
		m_pWindow->draw(rect, m_transform);
	}
}

// GetWindow( )
//
// Description:	returns the window which this backbuffer is incharge of.
//
// @param	n/a
//
// @return	sf::RenderWindow	the window rendered.
//
sf::RenderWindow* BackBuffer::GetWindow() const
{
	return m_pWindow;
}

// GetWindowSize( )
//
// Description:	returns the current size of the window.
//
// @param	n/a
//
// @return	sf::Vector2u
//
const sf::Vector2u BackBuffer::GetWindowSize() const
{
	return sf::Vector2u(m_width, m_height);
}

// SetFullScreen( bool _isFullScreen )
//
// Description:	Deletes and creates a new window with the window 
//				specification provided. Only deletes if the current
//				window has a different specification.
//
// @param	boolean		If the window should be fullscreen or not.
//
// @return	n/a
//
void BackBuffer::SetFullScreen(bool _isFullScreen)
{
	if (_isFullScreen && !m_isFullScreen)
	{
		delete m_pWindow;
		m_pWindow = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "Space Invaders", sf::Style::Fullscreen);
	}
	else if (m_isFullScreen && !_isFullScreen)
	{
		delete m_pWindow;
		m_pWindow = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "Space Invaders");
	}
	m_isFullScreen = _isFullScreen;
}

// SetWindowSize( sf::Vector2u size )
//
// Description:	Updates the size of the window, unlike the extend window function,
//				just requires the new size.
//
// @param	sf::Vector2u	New window size.
//
// @return	n/a
//
void BackBuffer::SetWindowSize(sf::Vector2u size)
{
	if (size.x > 0 && size.y > 0)
	{
		sf::Transform scale(static_cast<float>(size.x / m_width), 0.0f, 0.0f,
			0.0f, static_cast<float>(size.y / m_height), 0.0f,
			0.0f, 0.0f, 1.0f);
		m_transform = sf::Transform::Identity;
		m_width = size.x;
		m_height = size.y;
		m_pWindow->setSize(size);
	}
}

// SetColour( sf::Color _colour )
//
// Description:	Updates the current colour used in rendering functions
//				(Except rendering sprites).
//
// @param	sf::Color	new colour to be used.
//
// @return	n/a
//
void BackBuffer::SetColour(sf::Color _colour)
{
	m_GlobalColour = _colour;
}

// SetScreenOffset( sf::Vector2i _shake )
//
// Description:	Applies a transform to distort all rendering
//				in the specified direction.
//
// @param	sf::Vector2i _shake		direction of the distortion.
//
// @return	n/a
//
void BackBuffer::SetScreenOffset(sf::Vector2i _shake)
{
	PopTransform(m_currShake); //pop old shake
	m_currShake = sf::Transform(1, 0, static_cast<float>(_shake.x), 0, 1, static_cast<float>(_shake.y), 0, 0, 1);
	PushTransform(m_currShake); //Push new shake
}

// UpdateViewCenter( sf::Vector2f )
//
// Description:	Updates the center location of the viewport.
//
// @param	sf::Vector2f	New center position of the viewport.
//
// @return	n/a
//
void BackBuffer::UpdateViewCenter(sf::Vector2f _pos)
{
	viewport.setCenter(_pos);
	m_pWindow->setView(viewport);
}

// UpdateViewSize( sf::Vector2f )
//
// Description:	Updates the size of the viewport.
//
// @param	sf::Vector2f	New size of the viewport.
//
// @return	n/a
//
void BackBuffer::UpdateViewSize(sf::Vector2f _size)
{
	viewport.setSize(_size);
	m_pWindow->setView(viewport);
}

// DumpTextures( )
//
// Description:	Clears all textures from the map.
//
// @return	n/a
//
void BackBuffer::DumpTextures()
{
	std::map<std::string, sf::Texture*>::iterator iter = m_pTextureMap->begin();
	while (iter != m_pTextureMap->end())
	{
		delete (*iter).second;
		iter++;
	}
	m_pTextureMap->clear();
}

// ResetView( )
//
// Description:	Reset the view port.
//
// @return	n/a
//
void BackBuffer::ResetView()
{
	viewport.setCenter(sf::Vector2f(static_cast<float>(m_width/2.0f), static_cast<float>(m_height/2.0f)));
	viewport.setSize(sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height)));
	m_pWindow->setView(viewport);
}