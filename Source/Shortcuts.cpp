/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
_Shortcuts Shortcuts;
/******************************************************************************/
void _Shortcuts::create()
{
	if(guiObjects.load("Gui/Obj/main.gobj"))
    {
      /*Gui +=*/ guiObjects;

      bg = &guiObjects.getImage("shortcuts");

      FREP(9) slot[i] = &guiObjects.getImage(S + "slot" + i);
    }
}
void _Shortcuts::del()
{
      guiObjects.del();
}
void _Shortcuts::useSlot(Int i)
{
	if(slots[i].valid()) slots[i]().use();
}
/******************************************************************************/