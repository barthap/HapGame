/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
InventoryGui InvGui;
MENUGUI2 PauseGui;
/******************************************************************************/
// MANAGE
/******************************************************************************/
void InventoryGui::create()
{
   Flt w = 0.11, //Default unit width
       s = 0.03; //Default unit spacing

   //Window
   Gui += window.create(Rect_R(D.w() - 0.1, 0, 1.1, 1.75f), "Ekwipunek");
   window.visible(false);

   //Slots
   {
      //Background images
      //Image *slot = Images("Gfx/Inventory/slot.gfx");

      window += slot_img[SLOT_HEAD][0].create(Rect_U(window.crect.w() / 2, -0.05, w * 2, w * 2), Images("Gfx/Inventory/slot1.gfx")).desc("G³owa");
	  window += slot_img[SLOT_NECK][0].create(Rect_LU(slot_img[SLOT_HEAD][0].rect.max.x + s, slot_img[SLOT_BODY][0].rect.min.y - (w * 1.45), w, w), Images("Gfx/Inventory/slot2.gfx")).desc("Szyja");

	  window += slot_img[SLOT_BODY][0].create(Rect_U(window.crect.w() / 2, slot_img[SLOT_HEAD][0].rect.min.y - s, w * 2,w * 3), Images("Gfx/Inventory/slot3.gfx")).desc("Klatka piersiowa");
      window += slot_img[SLOT_ARM_L][0].create(Rect_RU(slot_img[SLOT_BODY][0].rect.min.x - s, slot_img[SLOT_BODY][0].rect.max.y, w * 2, w * 3), Images("Gfx/Inventory/slot4.gfx")).desc("Lewa rêka");
      window += slot_img[SLOT_ARM_R][0].create(Rect_LU(slot_img[SLOT_BODY][0].rect.max.x + s, slot_img[SLOT_BODY][0].rect.max.y, w * 2, w * 3), Images("Gfx/Inventory/slot5.gfx")).desc("Prawa rêka");
	  window += slot_img[SLOT_HAND_L][0].create(Rect_RU(slot_img[SLOT_ARM_L][0].rect.min.x - s, slot_img[SLOT_ARM_L][0].rect.min.y + w, w, w), Images("Gfx/Inventory/slot6.gfx")).desc("Lewa D³oñ");
	  window += slot_img[SLOT_HAND_R][0].create(Rect_LU(slot_img[SLOT_ARM_R][0].rect.max.x + s, slot_img[SLOT_ARM_L][0].rect.min.y + w, w, w), Images("Gfx/Inventory/slot7.gfx")).desc("Prawa D³oñ");

	  window += slot_img[SLOT_PANTS][0].create(Rect_U(window.crect.w() / 2, slot_img[SLOT_BODY][0].rect.min.y - s, w * 2, w * 2), Images("Gfx/Inventory/slot8.gfx")).desc("Spodnie");
	  window += slot_img[SLOT_GAUNTLET][0].create(Rect_RU(slot_img[SLOT_PANTS][0].rect.min.x - s, slot_img[SLOT_PANTS][0].rect.max.y, w * 2, w * 2), Images("Gfx/Inventory/slot9.gfx")).desc("Rêkawice");
	  window += slot_img[SLOT_BOOT][0].create(Rect_LU(slot_img[SLOT_PANTS][0].rect.max.x + s, slot_img[SLOT_PANTS][0].rect.max.y, w * 2, w * 2), Images("Gfx/Inventory/slot10.gfx")).desc("Buty");

      //Item images
      REPA(slot_img) if(i != SLOT_TEMP)
      {
         window += slot_img[i][1].create(slot_img[i][0].rect).desc(slot_img[i][0].desc());
		 slot_img[i][1].rect_color = Color();
      }
   }

   //Region
   {
      Rect_LU rect(0, 0, window.crect.w(), window.crect.h() - 0.06);

      rect.max.y = rect.min.y + 0.76f;
      rect.extend(-0.05f);

      window += region.create(rect);
   }

   //List
   {
      ListGroup lg[]=
      {
         ListGroup(MEMBER(Item, icon ), 0.2f, L"Ikona"),
         ListGroup(MEMBER(Item, name ), 0.36f, L"Nazwa"),
         ListGroup(MEMBER(Item, power), 0.13f, L"Atak"),
		 ListGroup(MEMBER(Item, defence), 0.16f, L"Obrona"),
		 ListGroup(MEMBER(Item, weight), 0.15f, L"Waga"),
      };

      lg[2].precision = 0; //Set showing decimal precision for float attribute
	  lg[3].precision = 0;
	  lg[4].precision = 1;

      region += list.create(lg, Elms(lg));

      list.cur_mode = LCM_MOUSE;
      list.draw_mode = LDM_RECTS;
   }

   //Gold
   {
	   window += goldText.create(Rect_U(window.crect.w() / 4, region.rect.min.y - (s / 4), w * 3, w), S + "Z³oto: 0");
	   window += goldImage.create(Rect_RU(goldText.rect.min.x - 0.01, goldText.rect.max.y, w, w), Images("Gfx/Inventory/coin.gfx"));

		goldImage.rect_color = Color(0,0,0,0);
   }

   //Weight
   {
      window += weightText.create(Rect_U((window.crect.w() / 4) * 3, region.rect.min.y - (s / 4), w * 3, w), S + "Waga: 0/0");
		window += weightImage.create(Rect_RU(weightText.rect.min.x - 0.01, weightText.rect.max.y, w+0.1, w), Images("Gfx/Inventory/kowadlo.gfx"));

		weightImage.rect_color = Color(0,0,0,0);
   }
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void InventoryGui::link(Inventory *inv)
{
   if(T.inv != inv)
   {
      T.inv = inv;
      if(inv) inv->inv_gui = this;

      setGui();
   }
}
/******************************************************************************/
void InventoryGui::setGui()
{
   if(inv)
   {
	   // set item list
	   {
		   // here we have to hide the items which are assigned to slots
		   // create a 'is' array which determines visibility of an element (1=visible, 0=hidden)
		   Bool  *is=Alloc<Bool>(inv->items.elms()); // allocate bytes for all items in container
		   SetMem(is,1,inv->items.elms());           // set memory to '1' (make all items visible by default)
		   REPA(inv->slot) // iterate through all slots
		   {
			   if(inv->slot[i].valid()) // if the slot is valid
			   {
				   Int index=inv->items.validIndex(&inv->slot[i]()); // get index of a slot item in items container
				   if(InRange(index,inv->items)) // if its valid
					   is[index]=0; // set visibility for item assigned to a slot to 0, to be hidden on the list
			   }
		   }
		   list.setData(inv->items,is); // set list data from items container and visibility list
		   Free(is); // free allocated memory
	   }

      //Set slot images
      REP(SLOT_NUM) //For all slots
         if(i != SLOT_TEMP) //Skip temporary slot because it's not drawn using 'Image' class
      {
         GuiImage &img_back = slot_img[i][0],    //Background image, we use it for accessing its rectangle
                  &img_item = slot_img[i][1];    //Item image
         Reference<Item> &item = inv->slot[i]; //Item at slot

         if(!item.valid()) img_item.set(NULL); else //If there is no item then clear the item image slot
         {
            Image *icon = item().icon; //Access item's icon
            img_item.set(icon);        //Set slot image as the item's icon
            if(icon)                   //Set proper scaling
            {
               Vec2 size(icon->x(), icon->y());
			   size *= PIXEL_SIZE; //Set default size

               if(size.x > img_back.rect.w()) size *= img_back.rect.w() / size.x; //Clamp item size to background slot width
               if(size.y > img_back.rect.h()) size *= img_back.rect.h() / size.y; //Clamp item size to background slot height

               Rect rect(img_back.rect.center());
			   rect.extend(size / 2);

               img_item.setRect(rect);
            }
         }
      }
   } else
   {
      list.clear();
      REP(SLOT_NUM) slot_img[i][1].set(NULL);
   }
}
void InventoryGui::toggle()
{
	window.visibleToggle();
	setMouseVisibility();
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void InventoryGui::update(Game::Chr &owner)
{
   if(inv)
   {
      if(Ms.bp(0)) // If mouse button pressed
      {
         if(inv->slot[SLOT_TEMP].valid()) //If we have an item attached with mouse
         {
            if(Gui.ms()==&list || window.hidden()) //If mouse cursor is on the list
            {
               inv->slot[SLOT_TEMP].clear(); //Clear the slot reference which will result in "putting back the item into the list"
               setGui();                     //Update visuals
            }
			else if(Gui.ms() == Gui.desktop() || !Gui.ms()) //If mouse cursor is on the desktop or nothing (this means the game world area)
            {
               owner.itemDropDown(inv->slot[SLOT_TEMP]()); //Drop the item onto the world
            }
			else if(Gui.ms() == Shortcuts.bg)
			{
				Int i = (Ms.pos().x - Shortcuts.bg->pos().x) / 64;
				
				Shortcuts.slots[i] = inv->slot[SLOT_TEMP];
				inv->slot[SLOT_TEMP].clear();
			}
			else 
			{
				if(Players[0].L_loot)
					if(Gui.ms()==&Players[0].L_loot->loot_gui->L_list)
					{
						Players[0].itemPutTo(inv->slot[SLOT_TEMP](),*Players[0].L_loot);
					}

			}
         }
         else //We don't have an item so we want to get one
         {
            if(Gui.ms() == &list) //From the list
            {
               if(Item *item = list())
               {
                  inv->slot[SLOT_TEMP] = item;
                  setGui();
               }
            }
         }

         REP(SLOT_NUM)
            if(Gui.ms() == &slot_img[i][0] || Gui.ms() == &slot_img[i][1]) //If we want to put the item onto the i-th slot
               if(inv->slotsCanBeSwapped(SLOT_TEMP, i))
               {
                  Swap(inv->slot[SLOT_TEMP], inv->slot[i]); //Swap temporary with i-th slot
                  setGui();                                 //Update visuals
               }
      }//Ms.b(0)
		else if(Ms.bp(1))
	  {
		  if(!inv->slot[SLOT_TEMP].valid())
		  {
			  if(Gui.ms() == &list)
			  {
				  if(Item *item = list())
				  {
					  item->use();
				  }//item list
			  }//gui ms list
		  }//inv slot temp valid
	  }//ms bp 1
		else
		{
			if(inv->slot[SLOT_TEMP].valid()) //If we have an item attached with mouse
			{
				if(window.hidden()) //If mouse cursor is on the list
				{
					inv->slot[SLOT_TEMP].clear(); //Clear the slot reference which will result in "putting back the item into the list"
					setGui();                     //Update visuals
				}
			}
		}

   }//if inv

   goldText.set(S + "Z³oto: " + Players[0].gold);
   weightText.set(S + "Waga: " + (Int)inv->inventoryWeight + "/" + (Int)(Players[0].strength * 2.5/* * (Byte)(Players[0].strenght / 10)*/));
}
/******************************************************************************/
// DRAW
/******************************************************************************/
void InventoryGui::draw()
{
   if(inv && inv->slot[SLOT_TEMP].valid()) inv->slot[SLOT_TEMP]().drawIcon(Ms.pos());
}
/******************************************************************************/
/*****************************FUNKCJE MENU*************************************/
void Start(Ptr) //uruchamia gre
{
	StateGame.set(0.5);
}
void Wyjscie(Ptr) //zamyka program
{
	StateExit.set();
}
void Czyt(Ptr) //wczytuje gre
{
	SG.load("Saves/quicksave.sav");
}
void Opcje(Ptr) //w-¦cza okno opcji
{
	Options.window.visibleToggle();
	PauseGui.window.visible(false);
}
void Aut(Ptr) //autorzy
{
	//StAu.set(1.0);
}
void Mn(Ptr) //menu
{
	StateMenu.set(0.5);
}
void Start2(Ptr) //pauza
{
	PauseGui.toggle();
}

void MENUGUI2::create()
{
	Gui   +=window.create(Rect(-0.5,-0.4,0.5,0.3),"Menu Pauzy"); // create window and add it to Gui
	window+=start.create(Rect(0.35f,-0.17f,0.65f,-0.1f),"Powrót").func(Start2);
	window+=load.create(Rect(0.35f,-0.27f,0.65f,-0.2f),"Wczytaj").func(Czyt);
	window+=opcje.create(Rect(0.35f,-0.37f,0.65f,-0.3f),"Opcje").func(Opcje);
	window+=wyjscie.create(Rect(0.35f, -0.47f,0.65f,-0.4f), "Menu Gry").func(Mn);
}
void MENUGUI2::toggle()
{
	window.visibleToggle();
	setMouseVisibility();
}
void MENUGUI2::update() {}