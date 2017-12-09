#include "stdafx.h"
#include "steering.h"

Ps3Controller Ps3;
CONTROLLER activeCtrl = CONTROLLER_KEYBOARD_MOUSE;

bool Ps3Controller::Axis( PS3PAD_AXIS Axis, bool LeftAxis /*= true*/, Flt deadZone)
{
	//pobranie odpowiednich wartosci w zaleznosci od tego, ktory to ma byc drazek
	Vec2 axis = (LeftAxis ? joypad.dir_a[0] : joypad.dir_a[1]);

	switch(Axis)
	{
	case AXIS_X_PLUS:
		if(axis.x > deadZone)return true;
		break;
	case AXIS_X_MINUS:
		if(axis.x < -deadZone)return true;
		break;
	case AXIS_Y_PLUS:
		if(axis.y > deadZone)return true;
		break;
	case AXIS_Y_MINUS:
		if(axis.y < -deadZone)return true;
		break;
	}
	return false;
}

bool Ps3Controller::Btn( int BtnNumber, bool Long /*= true*/ )
{
	bool pressed = (Long ? joypad.b(BtnNumber) : joypad.bp(BtnNumber));
	return pressed;
}

bool Ps3Controller::LR2( bool Left, Flt deadZone /*= 0.2f*/ )
{
	bool pressed;
	if(Left)
	{
		if(joypad.dis.slider[1] < -deadZone)pressed = true;
		else pressed = false;
	}
	else if(!Left) //albo po prostu else, ale dla pewnoœci dalem else if :)
	{
		if(joypad.dis.slider[1] > deadZone)pressed = true;
		else pressed = false;
	}
	return pressed;
}

Vec2 Ps3Controller::GetControllerPosition()
{
	//pobiera pozycjê x i y (tak mam ustawione sterowniki kontrolera)
	return Vec2(joypad.dis.rx, joypad.dis.ry);
}

//konstruktor, ustawienie referencji na pierwszy znaleziony kontroler
Ps3Controller::Ps3Controller() : joypad(*Joypad) {}

bool Ps3Controller::PointOfViewButtonPressed( POINTOFVIEW_BUTTONS button, bool Long )
{
	if(!Long)
	{
		if(pov_pressed[button])
		{
			pov_pressed[button] = false;
			return false;
		}
		else
		{
			pov_pressed[button] = true;
		}
	}

	switch(button)
	{
	case POV_TOP:
		if(joypad.dir.y > 0.5)return true;
		break;
	case POV_BOTTOM:
		if(joypad.dir.y < -0.5)return true;
		break;
	case POV_LEFT:
		if(joypad.dir.x < -0.5)return true;
		break;
	case POV_RIGHT:
		if(joypad.dir.x > 0.5)return true;
	}

	return false;
}
