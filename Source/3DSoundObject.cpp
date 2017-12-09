/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/

Sound3D::Sound3D() // initialize values in constructor
{
	position.zero();
	path = NULL;
	range = 1;
	volume = 0.5;
}
void Sound3D::create(Game::ObjParams &obj)
{
	// now setup custom parameters from 'obj'
	position=obj.matrixFinal().pos; // obtain our 'position' member from 'obj'
	
	if(Param *p = obj.findParam("path"))path = p->asText();
	if(Param *p = obj.findParam("range"))range = p->asFlt();
	if(Param *p = obj.findParam("volume"))volume = p->asFlt();

	sound.play(path,position,range,true,volume);
}
/******************************************************************************/
UInt Sound3D::drawPrepare()
{
	return 0; // return 0 because no additional rendering modes are required
}

Bool Sound3D::update()
{
	sound.pos(position);
	return true;
}

/******************************************************************************/