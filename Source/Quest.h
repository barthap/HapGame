/******************************************************************************/
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
/******************************************************************************/
struct _Quest
{
	Str  varS[5];
	Int  varI[5];
	Bool varB[5];

	Str  questName;
	Str  questDescription;

    AI   *questGiver;

	void loadScript(Char *questPath);

	Bool completed();
	Bool toRemove;

	void save(File &f);
	Bool load(File &f);

	Str  pathToQuestFile;

	Bool initQuest;

	//Lua pointer
	lua_State* L;

	_Quest();
	~_Quest() { lua_close(L); }
};

extern Memb<_Quest> Quests;
extern _Quest *actQuestCheck;
/******************************************************************************/