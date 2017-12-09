/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Memb<ModClass> Mods;
ModClass *actModCheck;
/******************************************************************************/
ModClass::ModClass()
{
	//Lua init
	L = lua_open();
	luaL_openlibs(L);

	//Defining Lua functions
	//RegisterLuaBind(L);
	SF_registerAllFunctions(L);
	
}

void ModClass::LuaBind()
{
	RegisterLuaBind(L);
	RegisterWorldGlobals(L);
}

void ModClass::init(Str modPath, Str modDirectoryName)
{
	T.pathToMod = modPath;
	T.modDirectoryName = modDirectoryName;
	
	char tmp[128];
	Set(tmp, (Char *)modPath());

	//Run dialogue script
	luaL_dofile(L, tmp);

	//lua_getglobal(L, "");
	//lua_call(L, 0, 0);

	loadInfo();
}
void ModClass::deinit()
{

}

void ModClass::loadInfo()
{
	//Loading info
	FileText info;

	if(info.read(S + "Mods/" + T.modDirectoryName + "/info.dat"))
	{
		for(;info.level();)
        {
           if(info.cur("Name"))        T.modName = info.getName(); else 
           if(info.cur("Description")) T.modDescription = info.getName(); else
		   if(info.cur("Author"))      T.modAuthor = info.getName();
        }
	}
}

void ModClass::onInit()
{
	//Str actIOPath = IOPath();
	//IOPath(S + "Mods/" + T.modDirectoryName);

	lua_getglobal(L, "init");
    lua_call(L, 0, 0);

	//IOPath(actIOPath);
}
void ModClass::onUpdate()
{
	//Str actIOPath = IOPath();
	//IOPath(S + "Mods/" + T.modDirectoryName);

	lua_getglobal(L, "update");
    lua_call(L, 0, 0);

	//IOPath(actIOPath);
}
void ModClass::onDraw()
{
	//Str actIOPath = IOPath();
	//IOPath(S + "Mods/" + T.modDirectoryName);

	lua_getglobal(L, "draw");
    lua_call(L, 0, 0);

	//IOPath(actIOPath);
}
void ModClass::onShut()
{
	//Str actIOPath = IOPath();
	//IOPath(S + "Mods/" + T.modDirectoryName);

	lua_getglobal(L, "shut");
    lua_call(L, 0, 0);

	//IOPath(actIOPath);
}

void ModClass::save(File &f)
{
	lua_getglobal(L, "save");
    lua_call(L, 0, 0);

	//f << pathToMod;
}
Bool ModClass::load(File &f)
{
	lua_getglobal(L, "load");
    lua_call(L, 0, 0);

    //f >> pathToMod;

	return true;
}
/******************************************************************************/