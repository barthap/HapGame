/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Memb<_Quest> Quests;
_Quest *actQuestCheck;
/******************************************************************************/
_Quest::_Quest()
{
	initQuest = true;

	questGiver = NULL;
	pathToQuestFile = NULL;

	//Lua init
	L = lua_open();
	luaL_openlibs(L);

	//Defining Lua functions
	SF_registerAllFunctions(L);
}
void _Quest::loadScript(Char *questPath)
{
	initQuest = false;

	//actQuestCheck->toRemove = false;
	T.toRemove = false;
	T.pathToQuestFile = questPath;

	char tmp[128];
	Set(tmp, S+"Scripts/"+questPath);

	RegisterLuaBind(L);
	RegisterWorldGlobals(L);

	//Run quest script
	luaL_dofile(L, tmp);

	lua_getglobal(L, "init");
    lua_call(L, 0, 0);

	AddMessage(S + "Otrzymano zadanie: " + T.questName);
}
Bool _Quest::completed()
{
	lua_getglobal(L, "completed");
	lua_call(L, 0, 0);

	return true;
}
void _Quest::save(File &f)
{
	FREP(Elms(varS))
	{
		f.putStr(varS[i]);
	}
	FREP(Elms(varI))
	{
		f.putInt(varI[i]);
	}
	FREP(Elms(varB))
	{
		f.putBool(varB[i]);
	}

    //f << questName << questDescription << questGiver << toRemove << pathToQuestFile;
	f.putStr(questName);
	f.putStr(questDescription);
	f.put(&questGiver, SIZE(questGiver));
	f.putBool(toRemove);
	f.putStr(pathToQuestFile);
}
Bool _Quest::load(File &f)
{
	FREP(Elms(varS))
	{
		varS[i] = f.getStr();
	}
	FREP(Elms(varI))
	{
		varI[i] = f.getInt();
	}
	FREP(Elms(varB))
	{
		varB[i] = f.getBool();
	}

	//f >> questName >> questDescription >> questGiver >> toRemove >> pathToQuestFile;
    questName = f.getStr();
	questDescription = f.getStr();
	f.get(&questGiver, SIZE(questGiver));
	toRemove = f.getBool();
	pathToQuestFile = f.getStr();

	initQuest = true;
	//actQuestCheck = this;
	//T.loadScript((Char *)pathToQuestFile());

	return true;
}
/******************************************************************************/