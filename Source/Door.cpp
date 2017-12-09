#include "stdafx.h"
#include "Main.h"


void Door::create(Game::ObjParams &obj)
{
	__super::create(obj);

	if(Param*p = obj.findParam("doorType"))doorType = p->asText();
}

UInt Door::drawPrepare()
{
	if(Lit==this)SetHighlight(Color(30,30,30,0)); // if the objects is the one under cursor, enable highlight before drawing it
	UInt modes=super::drawPrepare(); // default draw
	SetHighlight(); // clear highlight to zero
	return modes;
}