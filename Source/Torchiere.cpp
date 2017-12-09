/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
// MANAGE
/******************************************************************************/
void Torchiere::create(Game::ObjParams &obj)
{
	super::create(obj);

	// add all sub objects
	for(Game::ObjParamsPtr op=&obj; op; op=op->base())
		FREPA(op->sub_objs)
	{
		Item &item=items.New();
		item.create(op->sub_objs[i]);

		itemAdded(item);
	}
}
/******************************************************************************/
// ITEMS
/******************************************************************************/
void Torchiere::itemRemoved(Game::Obj &item)
{
	if(Item *i=CAST(Item,&item))
	{
		i->actor.active(false);
	//	i->unequipped();
	}
}
void Torchiere::itemAdded(Game::Obj &item)
{
	if(Item *i=CAST(Item,&item))
	{
		i->actor.matrix   (Matrix().setRotateX(0.27f).move(0,-0.2f,0.08f)*matrix())
			.active   (true)
			.kinematic(true)
			.obj      (this); // force 'obj' pointer to the torchiere instead of the torch

	//	i->equipped();
	}
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
Bool Torchiere::update()
{
	REPAO(items).update();
	return super::update();
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Torchiere::drawPrepare()
{
	UInt       modes =super  ::drawPrepare();
	REPA(items)modes|=items[i].drawPrepare();
	return     modes;
}
void Torchiere::drawShadow () {REPAO(items).drawShadow (); super::drawShadow();}
void Torchiere::drawPalette() {REPAO(items).drawPalette();}
void Torchiere::drawBlend  () {REPAO(items).drawBlend  ();}
/******************************************************************************/
// IO
/******************************************************************************/
Bool Torchiere::load(File &f)
{
	if(super::load(f))
	{
		REPAO(items).actor.obj(this); // force 'obj' pointer to the torchiere instead of the torch
	//	REPAO(items).sound.play();
		return true;
	}
	return false;
}
/******************************************************************************/
