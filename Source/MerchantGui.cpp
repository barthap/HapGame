/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
_MerchantGui MerchantGui;
/******************************************************************************/
void _MerchantGui::create()
{
	Gui += window.create(Rect_C(Vec2(0, 0), 2.666, 2), "Kupiec").hide();

	{
		//Merchant region
		Rect_C rect(Vec2(0.533, -1), 1, 1);

		//rect.max.y = rect.min.y + 0.9f;
		//rect.extend(-0.05f);

		window += mRegion.create(rect);

		//Player region
		Rect_C rect2(Vec2(2.133, -1), 1, 1);

		//rect2.max.y = rect2.min.y + 0.9f;
		//rect2.extend(-0.05f);

		window += pRegion.create(rect2);
	}
	
	{
		//Merchant list
		ListGroup lg[]=
		{
			ListGroup(MEMBER(Item, icon), 0.2f, L"Obrazek"),
			ListGroup(MEMBER(Item, name), 0.4f, L"Nazwa"),
			ListGroup(MEMBER(Item, power), 0.2f, L"Si³a"),
			ListGroup(MEMBER(Item, cost), 0.2f, L"Cena"),
		};
		
		lg[2].precision = 0;
		lg[3].precision = 2;
		mRegion += mList.create(lg, Elms(lg));
		
		mList.cur_mode = LCM_MOUSE;
		mList.draw_mode = LDM_RECTS;

		//Player list
		ListGroup lg2[]=
		{
			ListGroup(MEMBER(Item, icon), 0.2f, L"Obrazek"),
			ListGroup(MEMBER(Item, name), 0.4f, L"Nazwa"),
			ListGroup(MEMBER(Item, power), 0.2f, L"Si³a"),
			ListGroup(MEMBER(Item, cost), 0.2f, L"Cena"),
		};
		
		lg2[2].precision = 0;
		lg2[3].precision = 2;
		pRegion += pList.create(lg2, Elms(lg2));
		
		pList.cur_mode = LCM_MOUSE;
		pList.draw_mode = LDM_RECTS;
	}

	{
		//Cost Text
		window += textCost.create(Rect_C(Vec2(1.333, -1), 0.7, 0.1), "");

		//Gold Text
		window += textGold.create(Rect_C(Vec2(1.333, -1.15), 0.7, 0.1), "Dostêpne z³oto: 0");
	}

	{
		//Buy Button
		window += buttonBuy.create(Rect_C(Vec2(1.333, -1.30), 0.55, 0.07), "Kup").func(pto_buySelectedItems);

		//Resign Button
		window += buttonResign.create(Rect_C(Vec2(1.333, -1.42), 0.55, 0.07), "Rezygnuj").func(pto_Resign);
	}
}
void _MerchantGui::link(AI *merchant)
{
   if(T.merchant) unlink();

   T.merchant = merchant;
   T.cost = 0;

   T.merchant->nowTrading = true;

   setGui();
}
void _MerchantGui::setGui()
{
   if(merchant)
   {
	     //Merchant
		 Bool *is = Alloc<Bool>(merchant->inv.items.elms());

         SetMem(is, 1, merchant->inv.items.elms());

         REPA(merchant->inv.slot)
         {
            if(merchant->inv.slot[i].valid())
            {
				Int index = merchant->inv.items.validIndex(&merchant->inv.slot[i]());

                if(InRange(index, merchant->inv.items))
                  is[index] = 0;
            }
         }

		 mList.setData(merchant->inv.items, is);
         Free(is);

		 //Player
		 Bool *is2 = Alloc<Bool>(Players[0].inv.items.elms());

         SetMem(is2, 1, Players[0].inv.items.elms());

         REPA(Players[0].inv.slot)
         {
            if(Players[0].inv.slot[i].valid())
            {
				Int index = Players[0].inv.items.validIndex(&Players[0].inv.slot[i]());

                if(InRange(index, Players[0].inv.items))
                  is2[index] = 0;
            }
         }

		 pList.setData(Players[0].inv.items, is2);
         Free(is2);
   }

   textCost.set(S + (cost < 0 ? "Zarobek: " : "Koszt: ") + Abs(cost));
   textGold.set(S + "Dostêpne z³oto: " + Players[0].gold);
}
void _MerchantGui::toggle()
{
   window.visibleToggle();

   setMouseVisibility();
}
void _MerchantGui::update(Game::Chr &owner)
{
   if(merchant)
   {
      if(Ms.bp(0))
      {
         if(merchant->inv.slot[SLOT_TEMP].valid())
         {
            if(Gui.ms() == &mList)
            {
               merchant->inv.slot[SLOT_TEMP].clear();
               setGui();
            }
			else if(Gui.ms() == Gui.desktop() || !Gui.ms())
            {
			   merchant->inv.slot[SLOT_TEMP].clear();
               setGui();
            }
			else if(Gui.ms() == &pList)
			{
				if(Players[0].inv.canAddWeight(merchant->inv.slot[SLOT_TEMP]().weight))
				{
					merchant->itemPutTo(merchant->inv.slot[SLOT_TEMP](), Players[0]);

					refInv2.New();
					refInv2[refInv2.elms() - 1] = Players[0].inv.items[Players[0].inv.items.elms() - 1];

					cost += refInv2[refInv2.elms() - 1]().cost;

					setGui();
				}
				else
				{
					AddMessage("Nie mo¿esz unieœæ wiêcej przedmiotów");
				}
			}
         }
		 if(Players[0].inv.slot[SLOT_TEMP].valid())
         {
            if(Gui.ms() == &pList)
            {
               Players[0].inv.slot[SLOT_TEMP].clear();
               setGui();
            }
			else if(Gui.ms() == Gui.desktop() || !Gui.ms())
            {
			   Players[0].inv.slot[SLOT_TEMP].clear();
               setGui();
            }
			else if(Gui.ms() == &mList)
			{
				Players[0].itemPutTo(Players[0].inv.slot[SLOT_TEMP](), *merchant);

				refInv.New();
				refInv[refInv.elms() - 1] = merchant->inv.items[merchant->inv.items.elms() - 1];

				cost -= refInv[refInv.elms() - 1]().cost;

				setGui();
			}
         }
         else
         {
            if(Gui.ms() == &mList)
            {
               if(Item *item = mList())
               {
                  merchant->inv.slot[SLOT_TEMP] = item;
                  setGui();
               }
            }
			else if(Gui.ms() == &pList)
            {
               if(Item *item = pList())
               {
                  Players[0].inv.slot[SLOT_TEMP] = item;
                  setGui();
               }
            }
         }
      }
   }
}
void _MerchantGui::draw()
{
	if(merchant && merchant->inv.slot[SLOT_TEMP].valid()) merchant->inv.slot[SLOT_TEMP]().drawIcon(Ms.pos());
}
/******************************************************************************/
void pto_buySelectedItems(Ptr)
{
	if(MerchantGui.cost >= 0)
	{
		if(MerchantGui.cost <= Players[0].gold)
		{
			Players[0].gold -= MerchantGui.cost;
		}
		else
		{
			FREPA(MerchantGui.refInv)
			{
				MerchantGui.merchant->itemPutTo(MerchantGui.refInv[i](), Players[0]);
			}

			FREPA(MerchantGui.refInv2)
			{
				Players[0].itemPutTo(MerchantGui.refInv2[i](), *MerchantGui.merchant);
			}

			AddMessage("Nie masz tyle z³ota");
			
			MerchantGui.window.visible(false);
			MerchantGui.unlink();

			return;
		}
	}
	else
	{
		Players[0].gold += Abs(MerchantGui.cost);
	}

	MerchantGui.refInv.clear();
	MerchantGui.refInv2.clear();

	MerchantGui.window.visible(false);
	MerchantGui.unlink();
}
void pto_Resign(Ptr)
{
	FREPA(MerchantGui.refInv)
	{
		MerchantGui.merchant->itemPutTo(MerchantGui.refInv[i](), Players[0]);
	}

	FREPA(MerchantGui.refInv2)
	{
		Players[0].itemPutTo(MerchantGui.refInv2[i](), *MerchantGui.merchant);
	}

	MerchantGui.refInv.clear();
	MerchantGui.refInv2.clear();

	MerchantGui.window.visible(false);
	MerchantGui.unlink();
}
/******************************************************************************/