/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
_Storage Storage;
/******************************************************************************/
void _Storage::create()
{
	Gui += L_window.create(Rect_L(-D.w() + 0.02, 0, 1, 1), nazwa).hide();

	{
		Rect_LU rect(0, 0, L_window.crect.w(), L_window.crect.h());
		rect.max.y = rect.min.y + 0.9f;
		rect.extend(-0.05f);
		L_window += L_region.create(rect);
	}
	
	{
		ListGroup lg[]=
		{
			ListGroup(MEMBER(Item, icon), 0.2f, L"Obrazek" ),
			ListGroup(MEMBER(Item, name), 0.3f, L"Nazwa" ),
			ListGroup(MEMBER(Item, power), 0.2f, L"Si³a"),
		};
		
		lg[2].precision = 0;
		L_region += L_list.create(lg, Elms(lg));
		
		L_list.cur_mode = LCM_MOUSE;
		L_list.draw_mode = LDM_RECTS;
	}
}
void _Storage::link(AI *Chr_loot)
{
   if(T.Chr_loot) unlink();

   T.Chr_loot = Chr_loot;

   setGui();
}
void _Storage::setGui()
{
   if(Chr_loot)
   {
		 Bool *is = Alloc<Bool>(Chr_loot->inv.items.elms());

         SetMem(is, 1, Chr_loot->inv.items.elms());

         REPA(Chr_loot->inv.slot)
         {
            if(Chr_loot->inv.slot[i].valid())
            {
				Int index = Chr_loot->inv.items.validIndex(&Chr_loot->inv.slot[i]());

                if(InRange(index, Chr_loot->inv.items))
                  is[index] = 0;
            }
         }

		 L_list.setData(Chr_loot->inv.items, is);
         Free(is);
   }
}
void _Storage::toggle()
{
   L_window.visibleToggle();
   setMouseVisibility();
}
void _Storage::update(Game::Chr &owner)
{
   if(Chr_loot)
   {
      if(Ms.bp(0))
      {
         if(Chr_loot->inv.slot[SLOT_TEMP].valid())
         {
            if(Gui.ms() == &L_list)
            {
               Chr_loot->inv.slot[SLOT_TEMP].clear();
               setGui();
            }
			else if(Gui.ms() == Gui.desktop() || !Gui.ms())
            {
				Chr_loot->itemDropDown(Chr_loot->inv.slot[SLOT_TEMP]());
				setGui();
            }
			else if(Gui.ms() == &Players[0].inv.inv_gui->list)
			{
				if(Players[0].inv.canAddWeight(Chr_loot->inv.slot[SLOT_TEMP]().weight))
				{
					Chr_loot->itemPutTo(Chr_loot->inv.slot[SLOT_TEMP](), Players[0]);
					setGui();
				}
				else
				{
					AddMessage("Nie mo¿esz podnieœæ wiêcej przedmiotów");
				}
			}
         }
         else
         {
            if(Gui.ms() == &L_list)
            {
               if(Item *item = L_list())
               {
                  Chr_loot->inv.slot[SLOT_TEMP] = item;
                  setGui();
               }
            }
         }
      }
   }
}
void _Storage::draw()
{
	if(Chr_loot && Chr_loot->inv.slot[SLOT_TEMP].valid()) Chr_loot->inv.slot[SLOT_TEMP]().drawIcon(Ms.pos());
}
/******************************************************************************/