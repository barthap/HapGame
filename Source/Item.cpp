/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Item::Item()
{
   name[0] = 0;
   type = ITEM_NONE;

   power = 0;
   defence = 0;
   weight = 0;
   cost = 0;

   usable = true;

   icon = NULL;
   loot_gui = NULL;
}
/******************************************************************************/
// MANAGE
/******************************************************************************/
void Item::create(Game::ObjParams &obj)
{
	//hasOwner = false;

   __super::create(obj);

   //Set name
   if(Param *p = obj.findParam("name")) Set(name,p->asText());

   //Set type
   if(Param *p = obj.findParam("type")) type = p->asEnum();
   if(Param *p = obj.findParam("type2")) type2 = p->asEnum();

   //Set power
   if(Param *p = obj.findParam("power")) power = p->asFlt();

   //Set defence
   if(Param *p = obj.findParam("defence")) defence = p->asFlt();

   //Set weight
   if(Param *p = obj.findParam("weight")) weight = p->asFlt();

   //Set wcost
   if(Param *p = obj.findParam("cost")) cost = p->asFlt();

   //Set usable
   if(Param *p = obj.findParam("usable")) usable = p->asBool();

	//Add custom children
   for(Game::ObjParamsPtr cur=&obj; cur; cur=cur->base()) // check 'obj' and all its bases
	   FREPA(cur->sub_objs) // for each sub-object in 'cur'
   {
	   Game::ObjParams &o=cur->sub_objs[i];
	   switch(Game::World.objType(o.type())) // check the type of sub-object
	   {
	   case OBJ_ITEM: // if it's light, then create our member from its parameters
		   //Loot.New().create(o);
		   loot.New().create(o);
		   break;
	   case OBJ_PARTICLES:
		   if(Param *param=o.findParam("particles"))
			   if(Particles *p=ParticlesCache.get(param->asText()))
			   {
				   particles.create(*p);
				   particles.matrix=matrix();

				   Flt scale=o.scale()*1.2;
				   particles.radius      *=scale;
				   particles.offset_range*=scale;
				   particles.vel_random  *=scale;
				   particles.vel_constant*=scale;
				   particles.accel       *=scale;
				   particles.shape       *=scale;
				   particles.shape       +=Vec(0,0.8f,0);

				   //sound.create("sound/fire/torch.wav", particles.shape.pos()*particles.matrix, 2, true, 0.3f).frac(RandomF());
				   break;
			   }
	   }
   }
   if(loot.elms()!=0)
   REPA(loot)
		loot[i].actor.active(false);

   //Get icon
   for(Game::ObjParamsPtr op = &obj; op; op = op->base())              //Iterate through all ObjParams, to find first with a valid icon
      if(CChar *op_file_name = Game::Objs(op))                       //If current ObjParams is stored in a file (the file name is not NULL)
         if(icon = Images.get(GetExtNot(op_file_name) + ".ico.gfx")) //If there exists an icon with the same name as ObjParams but with different extension
            break;

   moved();
}
Bool Item::update()
{
	if(particles.is())
	{
		particles.matrix=matrix();
		particles.update();

		Vec center=particles.shape.pos()*particles.matrix;
	}
	return __super::update();
}

void Item::use()
{
	lua_State* L;
	
	L = lua_open();
	luaL_openlibs(L);

	//Defining Lua functions
	SF_registerAllFunctions(L);
	RegisterLuaBind(L);
	RegisterWorldGlobals(L);

	if(type==ITEM_HEALTH_POTION)
	{
		luaL_dofile(L, "Scripts/potions.lua");

		Players[0].itemDestroy(T);
		
		lua_getglobal(L, "healthPotion");
		lua_pushnumber(L, power);
		lua_call(L, 1, 0);
	}
	else if(type==ITEM_MANA_POTION)
	{
		luaL_dofile(L, "Scripts/potions.lua");

		Players[0].itemDestroy(T);

		lua_getglobal(L, "manaPotion");
		lua_pushnumber(L, power);
		lua_call(L, 1, 0);
	}
	
	lua_close(L);
}
/******************************************************************************/
// DRAW
void Item::moved()
{
	if(particles.is())
	{
		REPA(particles)particles.p(i).life+=particles.p(i).life_max; // set every single particle life to be at death time, this will force resetting all of the particles in the next update
	}
}
/******************************************************************************/
UInt Item::drawPrepare()
{
   if(Lit == this) SetHighlight(Color(48, 48, 48, 0));
   UInt modes = __super::drawPrepare();

   SetHighlight();

   if(particles.is())
   {
	   modes|=IndexToFlag(particles.renderMode());
	   modes|=IndexToFlag(RM_BLEND); // single particle

	   Vec center;
	   Flt opacity=particles.opacity(&center);
	   LightSqr(8*opacity,center, Vec(1, 0.8, 0)).add(true,this);
   }

   return modes;
}
/******************************************************************************/
void Item::drawIcon(Vec2 pos)
{
   if(icon)
   {
      icon->drawFit(pos.x, pos.y, icon->x() * PIXEL_SIZE, icon->y() * PIXEL_SIZE);
   }
}
void Item::drawBlend   () {{particles.draw(); DrawParticle(*Images("particle/gauss.gfx"),0,Color(255,255,170,0),0.07f,0.9f,0,particles.shape.pos()*particles.matrix,Vec(0));}}
void Item::drawPalette () {particles.draw();}
void Item::drawPalette1() {particles.draw();}
/******************************************************************************/
// IO
/******************************************************************************/
void Item::save(File &f)
{
   __super::save(f);

   f << name << power << defence << weight << type;
   //f.putBool(hasOwner);
   f.putStr(Images(icon));
}
Bool Item::load(File &f)
{
   if(__super::load(f))
   {
      f >> name >> power >> defence >> weight >> type;
	  //hasOwner = f.getBool();
	  icon = Images(f.getStr());

      return true;
   }
   return false;
}
/******************************************************************************/
/******************************************************************************/
// Funkcje do obs³ugi sub_obj
/******************************************************************************/
/******************************************************************************/
// GET
/******************************************************************************/
Bool Item::slotCanBePutTo(Int src,Int dest)
{
   if(!InRange(src,SLOT_NUM) || !InRange(dest,SLOT_NUM))return false;

   if(slots[src].valid())switch(dest)
   {
      case SLOT_HEAD:
      case SLOT_BODY:
         if(slots[src]().type==ITEM_WEAPON)return true;
      break;
   }
   return false;
}
Bool Item::slotsCanBeSwapped(Int a,Int b)
{
   return slotCanBePutTo(a,b) && slotCanBePutTo(b,a);
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Item::itemRemoved(Game::Item &item) // when an item is being removed from a character
{
   // perform check if it is a equipped item
   REPA(slots) // for all slots
      if(slots[i]==item) // if i-th slot is set to item which is being removed
         slots[i].clear(); // clear the slot so it can no longer be referenced to the removed item
}
void Item::itemRemoved(                ){setGui();moved();} // when item has been removed from a character
void Item::itemAdded  (Game::Item &item){setGui();moved();} // when item has been added to a character
void Item::setGui     (                ){if(loot_gui)loot_gui->setGui();}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void Item::l_update(Game::Chr &owner)
{
   if(loot_gui)
   {
	   loot_gui->update(owner);
   }
}
/*****************************************************************************/