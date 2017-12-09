/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
Inventory::~Inventory()
{
   if(inv_gui) inv_gui->unlink();
}
/******************************************************************************/
// GET
/******************************************************************************/
Bool Inventory::slotCanBePutTo(Int src, Int dest)
{
   if(!InRange(src, SLOT_NUM) || !InRange(dest, SLOT_NUM)) return false;

   if(slot[src].valid()) switch(dest)
   {
	  /*case SLOT_TEMP:
		  return true;
		  break;*/
	  case SLOT_HEAD:
		  if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_HELMET)) return false;
		  break;
	  case SLOT_NECK:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_NECK)) return false;
		  break;
	  case SLOT_ARM_L:
          if(!(slot[src]().type == ITEM_WEAPON || Contains(slot[src]().name, "Torch") || (slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_SHIELD))) return false;
		  break;
	  case SLOT_ARM_R:
          if(!(slot[src]().type == ITEM_WEAPON || Contains(slot[src]().name, "Torch"))) return false;
		  break;
	  case SLOT_BODY:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_BODY)) return false;
		  break;
	  case SLOT_HAND_L:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_HAND)) return false;
		  break;
	  case SLOT_HAND_R:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_HAND)) return false;
		  break;
	  case SLOT_BOOT:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_BOOT)) return false;
		  break;
	  case SLOT_PANTS:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_PANTS)) return false;
		  break;
	  case SLOT_GAUNTLET:
          if(!(slot[src]().type == ITEM_ARMOR && slot[src]().type2 == ARMOR_GAUNTLET)) return false;
		  break;
   }

   return true;
}
Bool Inventory::slotsCanBeSwapped(Int a, Int b)
{
   return slotCanBePutTo(a, b) && slotCanBePutTo(b, a);
}
Bool Inventory::itemCanBePickUp(Item &item)
{
	return item.weight <= ((Int)(Players[0].strength * 2.5)) - inventoryWeight;
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Inventory::itemRemoved(Game::Item &item)
{
   REPA(slot)
      if(slot[i] == item) //If i-th slot is set to item which is being removed
         slot[i].clear(); //Clear the slot so it can no longer be referenced to the removed item

   setWeight(item, false);
   //if(ItemEC *item2 = CAST(ItemEC, &item)) item2->hasOwner = false;
}
void Inventory::itemRemoved() { setGui(); } //When item has been removed from a character
void Inventory::itemAdded(Game::Item &item)
{
   setWeight(item, true);
   //if(ItemEC *item2 = CAST(ItemEC, &item)) item2->hasOwner = true;

   setGui();
}
void Inventory::setGui() { if(inv_gui) inv_gui->setGui(); }
void Inventory::setWeight(Game::Item &item, bool add)
{
	if(Item *actItem = CAST(Item, &item))
	{
		if(add == true)
		{
			inventoryWeight += actItem->weight;
		}
		else if(add == false)
		{
			inventoryWeight -= actItem->weight;
		}
	}
}
void Inventory::addWeight(Flt weight)
{
	inventoryWeight += weight;
}
void Inventory::subtractWeight(Flt weight)
{
	inventoryWeight -= weight;
}
Bool Inventory::canAddWeight(Flt weight)
{
	return weight <= ((Int)(Players[0].strength * 2.5)) - inventoryWeight;
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void Inventory::update(Game::Chr &owner)
{
   if(inv_gui) inv_gui->update(owner);

   //Set matrixes for items in hands
   if(slot[SLOT_ARM_L].valid())
      if(OrientP *point = owner.cskel.findPoint("HandL"))
         slot[SLOT_ARM_L]().matrix(Matrix().setPosDir(point->pos, point->perp, point->dir));

   if(slot[SLOT_ARM_R].valid())
      if(OrientP *point = owner.cskel.findPoint("HandR"))
         slot[SLOT_ARM_R]().matrix(Matrix().setPosDir(point->pos, point->cross(), point->dir));
}
/******************************************************************************/
// DRAW
/******************************************************************************/
void Inventory::drawPrepare(Game::Chr &owner)
{
   //Draw items in hands
   if(slot[SLOT_ARM_L].valid()) slot[SLOT_ARM_L]().drawPrepare();
   if(slot[SLOT_ARM_R].valid()) slot[SLOT_ARM_R]().drawPrepare();

   //Draw armor
   if(slot[SLOT_HEAD].valid()) slot[SLOT_HEAD]().mesh->draw(owner.cskel);
   if(slot[SLOT_BODY].valid()) slot[SLOT_BODY]().mesh->draw(owner.cskel);
   if(slot[SLOT_PANTS].valid()) slot[SLOT_PANTS]().mesh->draw(owner.cskel);
   if(slot[SLOT_BOOT].valid()) slot[SLOT_BOOT]().mesh->draw(owner.cskel);
   if(slot[SLOT_GAUNTLET].valid()) slot[SLOT_GAUNTLET]().mesh->draw(owner.cskel);
}
void Inventory::drawShadow(Game::Chr &owner)
{
   //Draw items in hands
   if(slot[SLOT_ARM_L].valid()) slot[SLOT_ARM_L]().drawShadow();
   if(slot[SLOT_ARM_R].valid()) slot[SLOT_ARM_R]().drawShadow();
}

void Inventory::drawBlend()
{
	// draw items in hands
	if(slot[SLOT_ARM_L].valid())slot[SLOT_ARM_L]().drawBlend();
	if(slot[SLOT_ARM_R].valid())slot[SLOT_ARM_R]().drawBlend();
}
void Inventory::drawPalette()
{
	// draw items in hands
	if(slot[SLOT_ARM_L].valid())slot[SLOT_ARM_L]().drawPalette();
	if(slot[SLOT_ARM_R].valid())slot[SLOT_ARM_R]().drawPalette();
}
/******************************************************************************/
// IO
/******************************************************************************/
void Inventory::save(File &f)
{
   FREPA(slot) //For all slots
      f.putInt(items.validIndex(&slot[i]())); //Store the valid index of i-th slot item in 'items' container

   f.putFlt(inventoryWeight);
}
Bool Inventory::load(File &f)
{
   FREPA(slot) //For all slots
   {
      Int item_index = f.getInt(); //Read index of i-th slot in 'items' container

      if(InRange(item_index, items))  //If the index is in valid range
         slot[i] = items[item_index]; //Set the slot to point to requested item
	  else
		 slot[i].clear(); //Clear the item reference in i-th slot
   }
   
   inventoryWeight = f.getFlt();

   setGui();

   return true;
}
/******************************************************************************/
