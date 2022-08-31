#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: CheckBox.h
// Description   	: A checkbox that toggles between true and false
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Widget.h"

//Implementation
class SpriteMap;

class CheckBox : public Widget
{
	//Member Function
public:
	CheckBox();
	virtual ~CheckBox();
	CheckBox(const CheckBox& _other);

	bool Initialise(BackBuffer& _backBuffer, float _x, float _y);
	
	//Inherited Functions:
	virtual void Draw(BackBuffer& _backBuffer);
	virtual void Update();
	virtual bool HandleMouse(float _mouseX, float _mouseY);

private:

protected:

	//Member Data
public:

private:
	SpriteMap* m_Anim;

protected:

};
#endif // _CHECKBOX_H_