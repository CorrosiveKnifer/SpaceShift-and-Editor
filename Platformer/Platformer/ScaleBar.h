#ifndef _SCALE_BAR_H_
#define _SCALE_BAR_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: ScaleBar.h
// Description   	: A Scale bar that allows the user to select between a range.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Widget.h"

//Implementation
class ScaleBar : public Widget
{
	//Member function
public:
	ScaleBar();
	virtual ~ScaleBar();
	ScaleBar(const ScaleBar& _other);

	bool Initialise(BackBuffer& _backBuffer, float _x, float _y);

	void SetBounds(float _min, float _max);
	void SetIncrement(float _delta);

	//Inherited Functions:
	virtual void Draw(BackBuffer& _backBuffer);
	virtual bool HandleMouse(float _mouseX, float _mouseY);
	virtual void Update();

	void SetWorldPosition(sf::Vector2f _pos);
private:

protected:

	//Member data
public:

private:

protected:
	float m_fMin, m_fMax;
	float m_fInc;
	sf::Sprite* m_pCursor;
	float CursorY;
};
#endif // _SCALE_BAR_H_