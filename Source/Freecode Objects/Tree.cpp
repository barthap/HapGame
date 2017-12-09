/******************************************************************************/
#include "stdafx.h"
#include "../Main.h"
/******************************************************************************/
Bool Tree::update()
{
	return __super::update();
}
void Tree::save(File &f)
{
	__super::save(f);
}
Bool Tree::load(File &f)
{
	if(__super::load(f))
	{
		return true;
	}
	
	return false;
}
/******************************************************************************/