/******************************************************************************/
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
/******************************************************************************/
struct ModClass
{
	Str pathToMod;
	Str modDirectoryName;

	Str modName;
	Str modDescription;
	Str modAuthor;

	void init(Str modPath, Str modDirectoryName);
	void deinit();

	void loadInfo();

	void LuaBind();

	void onInit();
	void onUpdate();
	void onDraw();
	void onShut();
	
	void save(File &f);
	Bool load(File &f);

	//Lua pointer
	lua_State* L;

	ModClass();
	~ModClass() { lua_close(L); }
};

extern Memb<ModClass> Mods;
extern ModClass *actModCheck;
/******************************************************************************/