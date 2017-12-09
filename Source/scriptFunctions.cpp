/******************************************************************************/
#include "stdafx.h"
#include "Main.h"

#include <luabind/class.hpp>
/******************************************************************************/
#pragma warning(disable:4800) //Forcing value to bool 'true' or 'false' (performance warning)
/******************************************************************************/

AI* scriptPointer; //tymczasowe ustawienie wskaznika na postac (np do dialogu)
Player* plrPointer = NULL;

bool addModPath = true;
Str addPath(Str modName, Str path)
{
	if(addModPath) path = S + "Mods/" + modName + "/" + path;

	return path;
}


/******************************************************************************/
void SF_registerAllFunctions(lua_State *L)
{

	//RegisterLuaBind(L);
	//RegisterKeyboardGlobals(L);

	//Getting Objects
	lua_register(L, "getObjPtr", &SF_getObjPtr);

	//Equipment - adding and removing
	lua_register(L, "eqGiveObj", &SF_eqGiveObj);
	lua_register(L, "eqRemoveObj", &SF_eqRemoveObj);
	lua_register(L, "eqCheckObj", &SF_eqCheckObj);

	//Get/Set
    lua_register(L, "Get", &SF_Get);
	lua_register(L, "Set", &SF_Set);

	//Dialogue
	lua_register(L, "DialogueText", &pto_DialogueText);
	lua_register(L, "Monologue", &pto_Monologue);
	lua_register(L, "Answer", &pto_Answer);
	lua_register(L, "deleteDialogue", &pto_deleteDialogue);

	//Quest
	lua_register(L, "setQuestName", &SF_setQuestName);
	lua_register(L, "setQuestDescription", &SF_setQuestDescription);
	lua_register(L, "qvGet", &SF_qvGet);
    lua_register(L, "qvSet", &SF_qvSet);
	lua_register(L, "endQuest", &SF_endQuest);

	//Starting
	lua_register(L, "startDialogue", &SF_startDialogue);
	lua_register(L, "startQuest", &SF_startQuest);

	//Script Variables
	lua_register(L, "declareVariable", &SF_declareVariable);
	lua_register(L, "getVariable", &SF_getVariable);
	lua_register(L, "setVariable", &SF_setVariable);
	lua_register(L, "variableExist", &SF_variableExist);
	lua_register(L, "deleteVariable", &SF_deleteVariable);

	//Creating
	lua_register(L, "chrCreate", &SF_chrCreate);
	lua_register(L, "itemCreate", &SF_itemCreate);

	lua_register(L, "addItem", &SF_addItem);

	lua_register(L, "setDialoguePath", &SF_setDialoguePath);

    lua_register(L, "posX", &SF_posX);
	lua_register(L, "posY", &SF_posY);
	lua_register(L, "posZ", &SF_posZ);

	lua_register(L, "moveWalking", &SF_moveWalking);
	lua_register(L, "moveTo", &SF_moveTo);

	lua_register(L, "attack", &SF_attack);
	lua_register(L, "actionBreak", &SF_actionBreak);

	//Other
	lua_register(L, "attackPlayer", &SF_attackPlayer);	//uzywane tylko w dialogu
	lua_register(L, "addGold", &SF_addGold);
	lua_register(L, "addExp", &SF_addExp);

	lua_register(L, "Message", &SF_Message);
	lua_register(L, "Text", &SF_Text);
	//lua_register(L, "Vec", &SF_Vec);

	lua_register(L, "Equal", &SF_Equal);

	lua_register(L, "getHour", &SF_getHour);
	lua_register(L, "getMinute", &SF_getMinute);
	lua_register(L, "getSecond", &SF_getSecond);

	lua_register(L, "activeFight", &SF_activeFight);

	lua_register(L, "triggerState", &SF_triggerState);

	lua_register(L, "talkingWith", &SF_talkingWith);

	lua_register(L, "KeyPressed", &SF_KeyPressed);	
}
void* SF_checkUserType(lua_State *L, int index, Char *typeName)
{
	char tmp[128];
	Set(tmp, typeName);

	void* udata = lua_touserdata(L, index);
	if(udata == 0) luaL_typerror(L, index, tmp);

	return udata;
}
/******************************************************************************/
Vec vecFromLua(lua_State *L, int first)
{
	return Vec(lua_tointeger(L, first), lua_tointeger(L, first + 1), lua_tointeger(L, first + 2));
}
/******************************************************************************/
int SF_getObjPtr(lua_State *L)
{
	int obj;
	obj = lua_tointeger(L, 1); //pobiera 1szy parametr
	
	//nie ma case 2: czyli itemow, sa celowo nie obslugiwane, jesli sie poda
	//funkcja zwroci nil
	switch(obj)
	{
	case 0: //player
		lua_pushlightuserdata(L, &Players[0]);
		break;
	case 1: //chr
		FREP(Chrs.elms())
		{
			if(Chrs[i].commanderID == lua_tointeger(L, 2)) lua_pushlightuserdata(L, &Chrs[i]);
		}
		break;
	case 3: //trigger
		FREP(Triggers.elms())
		{
			if(Triggers[i].id == lua_tointeger(L, 2)) lua_pushlightuserdata(L, &Triggers[i]);
		}
		break;
	default:
		lua_pushnil(L);
		break;
	} //switch

	return 1;
}

int SF_eqGiveObj(lua_State *L)
{
	Str pathToObj;

	Chr *chr;
	Item *item;

	chr = (Chr *)lua_touserdata(L, 1);
	pathToObj = lua_tostring(L, 2);

	Game::ObjParams &obj = *Game::Objs(pathToObj);

	Bool succes = Game::World.objCreate(obj, Matrix(obj.scale(), Vec(0, 0, 0)));

	if(succes == true)
	{
		item = &Items[Items.elms() - 1];

		chr->itemPickUp(*item);
	}

	return 0;
}
int SF_eqRemoveObj(lua_State *L)
{
	Str name;
	Int number;

	Chr *chrFrom;
	Chr *chrTo;

	chrFrom = (Chr *)lua_touserdata(L, 1);
	chrTo = (Chr *)lua_touserdata(L, 2);
	name = lua_tostring(L, 3);
	number = lua_tointeger(L, 4);

	FREP(chrFrom->inv.items.elms())
	{
		if(chrFrom->inv.items[i].name == name && number != 0)
		{
			chrFrom->inv.slot[SLOT_TEMP] = chrFrom->inv.items[i];
			chrFrom->itemPutTo(chrFrom->inv.slot[SLOT_TEMP](), *chrTo);

			number -= 1;
		}
	}

	return 0;
}
int SF_eqCheckObj(lua_State *L)
{
	Str name;
	Bool result;

	Chr *chr;

	chr = (Chr *)lua_touserdata(L, 1);
	name = lua_tostring(L, 2);

	result = false;

	FREP(chr->inv.items.elms())
	{
		if(chr->inv.items[i].name == name)
		{
			result = true;
		}
	}

	lua_pushboolean(L, result);

	return 1;
}

int SF_Get(lua_State *L)
{
	Str variable;
	variable = lua_tostring(L, 1);

	if(Contains(variable, "Player::Health"))
	{
		lua_pushnumber(L, Players[0].health);
	}
	else if(Contains(variable, "Player::Mana"))
	{
		lua_pushnumber(L, Players[0].mana);
	}
	else if(Contains(variable, "Player::exp"))
	{
		lua_pushnumber(L, Players[0].exp);
	}
	else if(Contains(variable, "Player::wood"))
	{
		lua_pushnumber(L, Players[0].wood);
	}
	else if(Contains(variable, "Player::stone"))
	{
		lua_pushnumber(L, Players[0].stone);
	}
	else if(Contains(variable, "Player::iron"))
	{
		lua_pushnumber(L, Players[0].iron);
	}
	else if(Contains(variable, "Monsters::skeleton::kills"))
	{
		lua_pushnumber(L, monsterKills[AIC_MONSTER]);
	}
	else
	{
		return 0;
	}

	return 1;
}
int SF_Set(lua_State *L)
{
	Str variable;
	variable = lua_tostring(L, 1);

	if(Contains(variable, "Player::Health"))
	{
		Players[0].health = lua_tonumber(L, 2);
	}
	else if(Contains(variable, "Player::Mana"))
	{
		Players[0].mana = lua_tonumber(L, 2);
	}
	else if(Contains(variable, "Player::exp"))
	{
		Players[0].exp = lua_tonumber(L, 2);
	}
	else if(Contains(variable, "Player::wood"))
	{
		Players[0].wood = lua_tonumber(L, 2);
	}
	else if(Contains(variable, "Player::stone"))
	{
		Players[0].stone = lua_tonumber(L, 2);
	}
	else if(Contains(variable, "Player::iron"))
	{
		Players[0].iron = lua_tonumber(L, 2);
	}
	else if(Contains(variable, "Monsters::skeleton::kills"))
	{
		monsterKills[AIC_MONSTER] = lua_tointeger(L, 2);
	}

	return 0;
}

int SF_setQuestName(lua_State *L)
{
	actQuestCheck->questName = lua_tostring(L, 1);

	return 0;
}
int SF_setQuestDescription(lua_State *L)
{
	actQuestCheck->questDescription = lua_tostring(L, 1);

	return 0;
}
int SF_qvGet(lua_State *L)
{
	Byte varType = lua_tointeger(L, 1);

	if(varType == 0)
	{
		int x = lua_tointeger(L, 2);
		lua_pushnumber(L, actQuestCheck->varI[x]);
	}
	else if(varType == 1)
	{
		lua_pushstring(L, reinterpret_cast<const char *> (actQuestCheck->varS[lua_tointeger(L, 2)]()));
	}
	else if(varType == 2)
	{
		lua_pushboolean(L, actQuestCheck->varB[lua_tointeger(L, 2)]);
	}

	return 1;
}
int SF_qvSet(lua_State *L)
{
    Byte varType = lua_tointeger(L, 1);

	if(varType == 0)
	{
		actQuestCheck->varI[lua_tointeger(L, 2)] = lua_tointeger(L, 3);
	}
	else if(varType == 1)
	{
		actQuestCheck->varS[lua_tointeger(L, 2)] = S + lua_tostring(L, 3);
	}
	else if(varType == 2)
	{
		actQuestCheck->varB[lua_tointeger(L, 2)] = lua_toboolean(L, 3);
	}

	return 0;
}
int SF_endQuest(lua_State *L)
{
	AddMessage(S + "Zakoñczono zadanie: " + actQuestCheck->questName);
	actQuestCheck->toRemove = true;

	return 0;
}

int SF_startDialogue(lua_State *L)
{
	if(Dialogue.now == false) Dialogue.scriptPointer = NULL;

	Dialogue.init((Char *)Str(lua_tostring(L, 1))());

	return 0;
}
int SF_startQuest(lua_State *L)
{
	_Quest *qst = &Quests.New();

	actQuestCheck = qst;

	qst->questGiver = Dialogue.scriptPointer;
	qst->loadScript((Char *)Str(lua_tostring(L, 1))());

	actQuestCheck = NULL;

	QuestGui.setGui();

	return 0;
}

int SF_declareVariable(lua_State *L)
{
	scriptVariable *sv = &scriptVariables.New();

	sv->name = lua_tostring(L, 1);
	sv->type = lua_tonumber(L, 2);

	sv->valueInt = 0;
	sv->valueStr = "";
	sv->valueBool = false;

	return 0;
}
int SF_getVariable(lua_State *L)
{
	Int varIndex;
	varIndex = svManager.find(lua_tostring(L, 1));

	if(varIndex > -1)
	{
		if(scriptVariables[varIndex].type == SVT_INT) lua_pushnumber(L, scriptVariables[varIndex].valueInt);
		else if(scriptVariables[varIndex].type == SVT_STR) lua_pushstring(L, reinterpret_cast<const char *> (scriptVariables[varIndex].valueStr()));
		else if(scriptVariables[varIndex].type == SVT_BOOL) lua_pushboolean(L, scriptVariables[varIndex].valueBool);

		return 1;
	}

	return 0;
}
int SF_setVariable(lua_State *L)
{
	Int varIndex;
	Str x;
	x = lua_tostring(L, 1);
	x = x;
	varIndex = svManager.find(lua_tostring(L, 1));

	if(varIndex > -1)
	{
		if(scriptVariables[varIndex].type == SVT_INT) scriptVariables[varIndex].valueInt = lua_tointeger(L, 2);
		else if(scriptVariables[varIndex].type == SVT_STR) scriptVariables[varIndex].valueStr = lua_tostring(L, 2);
		else if(scriptVariables[varIndex].type == SVT_BOOL) scriptVariables[varIndex].valueBool = lua_toboolean(L, 2);
	}

	return 0;
}
int SF_variableExist(lua_State *L)
{
	Int varIndex;
	varIndex = svManager.find(lua_tostring(L, 1));

	if(varIndex > -1) lua_pushboolean(L, true);
	else lua_pushboolean(L, false);

	return 1;
}
int SF_deleteVariable(lua_State *L)
{
	Int varIndex;
	varIndex = svManager.find(lua_tostring(L, 1));

	if(varIndex > -1) scriptVariables.remove(varIndex, false);

	return 0;
}

int SF_attackPlayer(lua_State *L)
{
	if(Dialogue.now)
	{
		pto_deleteDialogue(L);
		if(Dialogue.scriptPointer != NULL) Dialogue.scriptPointer->attack(Players[0]);
	}

	return 0;
}
int SF_addGold(lua_State *L)
{
	Players[0].gold += lua_tonumber(L, 1);

	return 0;
}
int SF_addExp(lua_State *L)
{
	int exp = lua_tonumber(L, 1);
	Players[0].exp += exp;
	AddMessage(S+"Doœwiadczenie +"+exp);

	return 0;
}
int SF_Message(lua_State *L)
{
	AddMessage(lua_tostring(L, 1));

	return 0;
}
int SF_Text(lua_State *L)
{
    Flt pos_x = lua_tonumber(L, 1);
    Flt pos_y = lua_tonumber(L, 2);

    CChar8* text = lua_tostring(L, 3);

    D.text(pos_x, pos_y, text);

	return 0;
}
int SF_Vec(lua_State *L)
{
	Vec x = Vec(lua_tointeger(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3));

	lua_pushlightuserdata(L, &x);

	return 1;
}
int SF_Equal(lua_State *L)
{
	Int  amount = lua_tointeger(L, 1);
	Bool result;

	result = true;

	FREP(amount)
	{
		if(lua_tointeger(L, amount + 1) != lua_tointeger(L, (amount + amount) + 1)) result = false;
	}
	
	lua_pushboolean(L, result);

	return 1;
}
int SF_getHour(lua_State *L)
{
	lua_pushnumber(L, time.getHours());

	return 1;
}
int SF_getMinute(lua_State *L)
{
	lua_pushnumber(L, time.getMinutes());

	return 1;
}
int SF_getSecond(lua_State *L)
{
	lua_pushnumber(L, time.getSeconds());

	return 1;
}
int SF_activeFight(lua_State *L)
{
	Bool result;
	Int distance;

	result = false;
	distance = lua_tointeger(L, 1);

	FREPA(Chrs)
	{
		if(Dist(scriptPointer->pos(), Chrs[i].pos()) <= distance)
		{
			if(Chrs[i].target.valid() && Chrs[i].inAttackRange(Chrs[i].target(), 1)) result = true;
		}
	}

	lua_pushboolean(L, result);

	return 1;
}
/******************************************************************************/
int SF_chrCreate(lua_State *L)
{
	//Vec *pos = (Vec *)lua_touserdata(L, 2);
	Str path = lua_tostring(L, 1);

	path = addPath(actModCheck->modDirectoryName, path);

	Game::ObjParams &obj = *Game::Objs(path);
    Game::World.objCreate(obj, Matrix(obj.scale(), vecFromLua(L, 2)));
	AI *newChr = &Chrs[Chrs.elms() - 1];

	lua_pushlightuserdata(L, newChr);

	return 1;
}
int SF_itemCreate(lua_State *L)
{
	Str path = lua_tostring(L, 1);

	path = addPath(actModCheck->modDirectoryName, path);

	Game::ObjParams &obj = *Game::Objs(path);
    Game::World.objCreate(obj, Matrix(obj.scale(), vecFromLua(L, 2)));
	Item *newItem = &Items[Items.elms() - 1];

	lua_pushlightuserdata(L, newItem);

	return 1;
}
int SF_addItem(lua_State *L)
{
	Chr  *chr = (Chr *)lua_touserdata(L, 1);
	Item  *item = (Item *)lua_touserdata(L, 2);

	chr->itemPickUp(*item);

	return 0;
}
int SF_setDialoguePath(lua_State *L)
{
	AI  *chr = (AI *)lua_touserdata(L, 1);
	Str path = lua_tostring(L, 2);

	//path = addPath(actModCheck->modDirectoryName, path);

	chr->dialoguePath = path;

	return 0;
}

/*int SF_pos(lua_State *L)
{
	AI  *chr = (AI *)lua_touserdata(L, 1);

	lua_pushlightuserdata(L, &chr->pos());

	return 1;
}*/

int SF_posX(lua_State *L)
{
	Chr  *chr = (Chr *)lua_touserdata(L, 1);

	lua_pushnumber(L, chr->pos().x);

	return 1;
}
int SF_posY(lua_State *L)
{
	Chr  *chr = (Chr *)lua_touserdata(L, 1);

	lua_pushnumber(L, chr->pos().y);

	return 1;
}
int SF_posZ(lua_State *L)
{
	Chr  *chr = (Chr *)lua_touserdata(L, 1);

	lua_pushnumber(L, chr->pos().z);

	return 1;
}

int SF_moveWalking(lua_State *L)
{
	AI  *chr = (AI *)lua_touserdata(L, 1);

	chr->move_walking = lua_toboolean(L, 2);

	return 0;
}
int SF_moveTo(lua_State *L)
{
	AI  *chr = (AI *)lua_touserdata(L, 1);
	//Vec *pos = (Vec *)lua_touserdata(L, 2);

	chr->actionMoveTo(vecFromLua(L, 2));

	return 0;
}
int SF_attack(lua_State *L)
{
	AI  *chr1 = (AI *)lua_touserdata(L, 1);
	Chr  *chr2 = (Chr *)lua_touserdata(L, 1);

	chr1->attack(*chr2);

	return 0;
}
int SF_actionBreak(lua_State *L)
{
	Chr  *chr = (Chr *)lua_touserdata(L, 1);

	chr->actionBreak();

	return 0; 
}
int SF_triggerState(lua_State *L)
{
	Trigger  *trigger = (Trigger *)lua_touserdata(L, 1);
	Bool state = lua_toboolean(L, 2);

	if(state == false)
	{
		trigger->enabled = false;
	}
	else
	{
		trigger->enabled = true;
	}

	return 0;
}
int SF_talkingWith(lua_State *L)
{
	AI* chr = (AI *)lua_touserdata(L, 1);
	if(chr!=0)chr->isTalking = true;
	Dialogue.scriptPointer = chr;

	return 0;
}
/******************************************************************************/
int SF_KeyPressed(lua_State *L)
{
	Str key = lua_tostring(L, 1);
	Bool b = lua_toboolean(L, 2);

	if(Contains(key, "KB_1"))			//Easy to read form
		lua_pushboolean(L, 
		(b ?
		(Kb.b(KB_1) ? true : false) :
		(Kb.bp(KB_1) ? true : false)
		)
		);
	//short forms
	if(Contains(key, "KB_2")) lua_pushboolean(L,(b ?(Kb.b(KB_2) ? true : false) : (Kb.bp(KB_2) ? true : false)));
	if(Contains(key, "KB_3")) lua_pushboolean(L,(b ?(Kb.b(KB_3) ? true : false) : (Kb.bp(KB_3) ? true : false)));
	if(Contains(key, "KB_4")) lua_pushboolean(L,(b ?(Kb.b(KB_4) ? true : false) : (Kb.bp(KB_4) ? true : false)));
	if(Contains(key, "KB_5")) lua_pushboolean(L,(b ?(Kb.b(KB_5) ? true : false) : (Kb.bp(KB_5) ? true : false)));
	if(Contains(key, "KB_6")) lua_pushboolean(L,(b ?(Kb.b(KB_6) ? true : false) : (Kb.bp(KB_6) ? true : false)));
	if(Contains(key, "KB_7")) lua_pushboolean(L,(b ?(Kb.b(KB_7) ? true : false) : (Kb.bp(KB_7) ? true : false)));
	if(Contains(key, "KB_8")) lua_pushboolean(L,(b ?(Kb.b(KB_8) ? true : false) : (Kb.bp(KB_8) ? true : false)));
	if(Contains(key, "KB_9")) lua_pushboolean(L,(b ?(Kb.b(KB_9) ? true : false) : (Kb.bp(KB_9) ? true : false)));
	if(Contains(key, "KB_0")) lua_pushboolean(L,(b ?(Kb.b(KB_0) ? true : false) : (Kb.bp(KB_0) ? true : false)));

	//letter keys
	if(Contains(key, "KB_A")) lua_pushboolean(L,(b ?(Kb.b(KB_A) ? true : false) : (Kb.bp(KB_A) ? true : false)));
	if(Contains(key, "KB_B")) lua_pushboolean(L,(b ?(Kb.b(KB_B) ? true : false) : (Kb.bp(KB_B) ? true : false)));
	if(Contains(key, "KB_C")) lua_pushboolean(L,(b ?(Kb.b(KB_C) ? true : false) : (Kb.bp(KB_C) ? true : false)));
	if(Contains(key, "KB_D")) lua_pushboolean(L,(b ?(Kb.b(KB_D) ? true : false) : (Kb.bp(KB_D) ? true : false)));
	if(Contains(key, "KB_E")) lua_pushboolean(L,(b ?(Kb.b(KB_E) ? true : false) : (Kb.bp(KB_E) ? true : false)));
	if(Contains(key, "KB_F")) lua_pushboolean(L,(b ?(Kb.b(KB_F) ? true : false) : (Kb.bp(KB_F) ? true : false)));
	if(Contains(key, "KB_G")) lua_pushboolean(L,(b ?(Kb.b(KB_G) ? true : false) : (Kb.bp(KB_G) ? true : false)));
	if(Contains(key, "KB_H")) lua_pushboolean(L,(b ?(Kb.b(KB_H) ? true : false) : (Kb.bp(KB_H) ? true : false)));
	if(Contains(key, "KB_I")) lua_pushboolean(L,(b ?(Kb.b(KB_I) ? true : false) : (Kb.bp(KB_I) ? true : false)));
	if(Contains(key, "KB_J")) lua_pushboolean(L,(b ?(Kb.b(KB_J) ? true : false) : (Kb.bp(KB_J) ? true : false)));
	if(Contains(key, "KB_K")) lua_pushboolean(L,(b ?(Kb.b(KB_K) ? true : false) : (Kb.bp(KB_K) ? true : false)));
	if(Contains(key, "KB_L")) lua_pushboolean(L,(b ?(Kb.b(KB_L) ? true : false) : (Kb.bp(KB_L) ? true : false)));
	if(Contains(key, "KB_M")) lua_pushboolean(L,(b ?(Kb.b(KB_M) ? true : false) : (Kb.bp(KB_M) ? true : false)));
	if(Contains(key, "KB_N")) lua_pushboolean(L,(b ?(Kb.b(KB_N) ? true : false) : (Kb.bp(KB_N) ? true : false)));
	if(Contains(key, "KB_O")) lua_pushboolean(L,(b ?(Kb.b(KB_O) ? true : false) : (Kb.bp(KB_O) ? true : false)));
	if(Contains(key, "KB_P")) lua_pushboolean(L,(b ?(Kb.b(KB_P) ? true : false) : (Kb.bp(KB_P) ? true : false)));
	if(Contains(key, "KB_Q")) lua_pushboolean(L,(b ?(Kb.b(KB_Q) ? true : false) : (Kb.bp(KB_Q) ? true : false)));
	if(Contains(key, "KB_R")) lua_pushboolean(L,(b ?(Kb.b(KB_R) ? true : false) : (Kb.bp(KB_R) ? true : false)));
	if(Contains(key, "KB_S")) lua_pushboolean(L,(b ?(Kb.b(KB_J) ? true : false) : (Kb.bp(KB_S) ? true : false)));
	if(Contains(key, "KB_T")) lua_pushboolean(L,(b ?(Kb.b(KB_K) ? true : false) : (Kb.bp(KB_T) ? true : false)));
	if(Contains(key, "KB_U")) lua_pushboolean(L,(b ?(Kb.b(KB_L) ? true : false) : (Kb.bp(KB_U) ? true : false)));
	if(Contains(key, "KB_V")) lua_pushboolean(L,(b ?(Kb.b(KB_M) ? true : false) : (Kb.bp(KB_V) ? true : false)));
	if(Contains(key, "KB_W")) lua_pushboolean(L,(b ?(Kb.b(KB_N) ? true : false) : (Kb.bp(KB_W) ? true : false)));
	if(Contains(key, "KB_X")) lua_pushboolean(L,(b ?(Kb.b(KB_O) ? true : false) : (Kb.bp(KB_X) ? true : false)));
	if(Contains(key, "KB_Y")) lua_pushboolean(L,(b ?(Kb.b(KB_P) ? true : false) : (Kb.bp(KB_Y) ? true : false)));
	if(Contains(key, "KB_Z")) lua_pushboolean(L,(b ?(Kb.b(KB_Q) ? true : false) : (Kb.bp(KB_Z) ? true : false)));

	return 1;

}