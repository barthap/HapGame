/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
LOOTGUI LootGui;
/******************************************************************************/
void LOOTGUI::create()
{
	Gui+=L_window.create(Rect_L(-D.w()+0.02,0,1,1),nazwa).hide();
	// region
	{
		Rect_LU rect(0,0,L_window.crect.w(),L_window.crect.h());
		rect.max.y=rect.min.y+0.9f;
		rect.extend(-0.05f);
		L_window+=L_region.create(rect);
	}
	
	// list
	{
		ListGroup lg[]=
		{
			ListGroup(MEMBER(Item,icon ),0.2f,L"Obrazek" ), // 0
			ListGroup(MEMBER(Item,name ),0.3f,L"Nazwa" ), // 1
			ListGroup(MEMBER(Item,power),0.2f,L"Si³a"), // 3
		};
		
		lg[2].precision=0; // set showing only 1 decimal precision for power float attribute
		L_region+=L_list.create(lg,Elms(lg));
		
		L_list. cur_mode=LCM_MOUSE;
		L_list.draw_mode=LDM_RECTS;
	}
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void LOOTGUI::link(Item *L_loot)
{
	//if(T.L_loot) unlink();
	//if(T.Chr_loot) unlink2();
   if(T.L_loot!=L_loot)
   {
      T.L_loot=L_loot;

      if(L_loot)
	    L_loot->loot_gui=this;

      setGui();
   }
}
/*void LOOTGUI::link2(AI *Chr_loot)
{
	//if(T.L_loot) unlink();
	//if(T.Chr_loot) unlink2();
   if(T.Chr_loot != Chr_loot)
   {
      T.Chr_loot = Chr_loot;

      if(Chr_loot)
		  Players[0].loot_gui = this;

      setGui();
   }
}*/
/******************************************************************************/
void LOOTGUI::setGui()
{
   if(L_loot)
   {
      // set item list
      {
         // here we have to hide the items which are assigned to slots
         // create a 'is' array which determines visibility of an element (1=visible, 0=hidden)
		   Bool  *is=Alloc<Bool>(L_loot->loot.elms()); // allocate bytes for all items in container
         SetMem(is,1,L_loot->loot.elms());           // set memory to '1' (make all items visible by default)
         REPA(L_loot->slots) // iterate through all slots
         {
            if(L_loot->slots[i].valid()) // if the slot is valid
            {
				Int index=L_loot->loot.validIndex(&L_loot->slots[i]()); // get index of a slot item in items container
               if(InRange(index,L_loot->loot)) // if its valid
                  is[index]=0; // set visibility for item assigned to a slot to 0, to be hidden on the list
            }
         }
			 L_list.setData(L_loot->loot,is); // set list data from items container and visibility list
         Free(is); // free allocated memory
      }
   }

   /*if(Chr_loot)
   {
	     Bool *is = Alloc<Bool>(Chr_loot->inv.items.elms());
         SetMem(is, 1, Chr_loot->inv.items.elms());

         REPA(Chr_loot->inv.slot)
         {
            if(Chr_loot->inv.slot[i].valid()) // if the slot is valid
            {
				Int index = Chr_loot->inv.items.validIndex(&Chr_loot->inv.slot[i]());
                if(InRange(index, Chr_loot->inv.items))
                  is[index] = 0;
            }
         }

		 L_list.setData(Chr_loot->inv.items, is);
         Free(is);
   }*/
}
/******************************************************************************/
void LOOTGUI::toggle()
{
	//if(L_window.visible())
	//{
		//L_loot->loot.del();
		//L_loot->loot_gui->L_item=NULL;
	//}
   L_window.visibleToggle();
   /*if(!L_window.visible())
   {
	   Players[0].loot_gui = NULL;
	   unlink();
	   unlink2();
   }*/
   setMouseVisibility();
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void LOOTGUI::update(Game::Chr &owner)
{
   if(L_loot)
   {
      if(Ms.bp(0)) // if mouse button pressed
      {
         if(L_loot->slots[SLOT_TEMP].valid()) // if we have an item attached with mouse
         {
            if(Gui.ms()==&L_list) // if mouse cursor is on the list
            {
               L_loot->slots[SLOT_TEMP].clear(); // clear the slot reference which will result in "putting back the item into the list"
               setGui();                     // update visuals
            }
			else if(Gui.ms()==Gui.desktop() || !Gui.ms()) // if mouse cursor is on the desktop or nothing (this means the game world area)
            {
				L_loot->itemDropDown(L_loot->slots[SLOT_TEMP](), &Players[0].matrix());
				setGui();
            }
			else if(Gui.ms()==&Players[0].inv.inv_gui->list)
			{
				if(Players[0].inv.canAddWeight(L_loot->slots[SLOT_TEMP]().weight))
				{
					L_loot->itemPutTo(L_loot->slots[SLOT_TEMP](),Players[0]);
					setGui();
				}
				else
				{
					AddMessage("Nie mo¿esz podnieœæ wiêcej przedmiotów");
				}
			}
         }
         else // we don't have an item so we want to get one
         {
            if(Gui.ms()==&L_list) // from the list
            {
               if(Item *item=L_list())
               {
                  L_loot->slots[SLOT_TEMP]=item;
                  setGui();
               }
            }
         }
      }//Ms.bp(0)
   }//L_loot

   /*if(Chr_loot)
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
					Chr_loot->itemDropDown(Chr_loot->inv.slot[SLOT_TEMP](), &Matrix(Chr_loot->inv.slot[SLOT_TEMP]().scale, Players[0].pos() + Vec(0.3, 0.3, 0.3)));
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
					if(ItemEC *item = L_list())
					{
						Chr_loot->inv.slot[SLOT_TEMP] = item;
						setGui();
					}
				}
			}
		}
	}*/
}
/******************************************************************************/
// DRAW
/******************************************************************************/
void LOOTGUI::draw()
{
	if(L_loot && L_loot->slots[SLOT_TEMP].valid()) L_loot->slots[SLOT_TEMP]().drawIcon(Ms.pos());
	//if(Chr_loot && Chr_loot->inv.slot[SLOT_TEMP].valid()) Chr_loot->inv.slot[SLOT_TEMP]().drawIcon(Ms.pos);
}
/******************************************************************************/