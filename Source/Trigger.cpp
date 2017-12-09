/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Trigger::Trigger()
{
	position.zero();
	enabled = true;
	script = "";
	range = 0;
	id = -1;
}
/******************************************************************************/
void Trigger::create(Game::ObjParams &obj)
{
	position=obj.matrixFinal().pos;

	if(Param *p = obj.findParam("script")) script = p->asText();
	if(Param *p = obj.findParam("enabled")) enabled = p->asBool();
	if(Param *p = obj.findParam("range")) range = p->asInt();
	if(Param *p = obj.findParam("id")) id = p->asInt();
}
/******************************************************************************/
Bool Trigger::update()
{
	//if(__super::update())
	{
		if(enabled == true)
		{
			if(Dist(Players[0].pos(), pos()) <= range)
			{
				
				lua_State* L;

				L = lua_open();
				luaL_openlibs(L);

				//Defining Lua functions
				SF_registerAllFunctions(L);
				RegisterLuaBind(L);
				RegisterWorldGlobals(L);

				char tmp[128];
				Set(tmp, script);

				luaL_dofile(L, tmp);

				lua_getglobal(L, "main");
				lua_call(L, 0, 0);

				lua_close(L);

			}
		}

		return true;
	}

	return false;
}
/******************************************************************************/
void Trigger::save(File &f)
{
   __super::save(f);

   f.putBool(enabled);
   f.putStr(script);
   f.putInt(range);
   f.putInt(id);
}
Bool Trigger::load(File &f)
{
   if(__super::load(f))
   {
      enabled = f.getBool();
	  f.getStr(script);
	  range = f.getInt();
	  id = f.getInt();

	  return true;
   }

   return false;
}
/******************************************************************************/