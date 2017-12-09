/******************************************************************************/
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
/******************************************************************************/

#include "scriptObjects.h"

/******************************************************************************/
extern bool addModPath;
Str addPath(Str modName, Str path);
/******************************************************************************/

void  SF_registerAllFunctions(lua_State *L);
extern void* SF_checkUserType(int index, Char *typeName);
/******************************************************************************/
extern Vec vecFromLua(lua_State *L, int first);
/******************************************************************************/
extern int SF_getObjPtr(lua_State *L);

extern int SF_eqGiveObj(lua_State *L);
extern int SF_eqRemoveObj(lua_State *L);
extern int SF_eqCheckObj(lua_State *L);

extern int SF_Get(lua_State *L);
extern int SF_Set(lua_State *L);

extern int SF_setQuestName(lua_State *L);
extern int SF_setQuestDescription(lua_State *L);
extern int SF_qvGet(lua_State *L);
extern int SF_qvSet(lua_State *L);
extern int SF_endQuest(lua_State *L);

extern int SF_startDialogue(lua_State *L);
extern int SF_startQuest(lua_State *L);

extern int SF_declareVariable(lua_State *L);
extern int SF_getVariable(lua_State *L);
extern int SF_setVariable(lua_State *L);
extern int SF_variableExist(lua_State *L);
extern int SF_deleteVariable(lua_State *L);

extern int SF_chrCreate(lua_State *L);

extern int SF_attackPlayer(lua_State *L);
extern int SF_addGold(lua_State *L);
extern int SF_addExp(lua_State *L);

extern int SF_Message(lua_State *L);
extern int SF_Text(lua_State *L);
//extern int SF_Vec(lua_State *L);

extern int SF_Equal(lua_State *L);

extern int SF_getHour(lua_State *L);
extern int SF_getMinute(lua_State *L);
extern int SF_getSecond(lua_State *L);

extern int SF_activeFight(lua_State *L);
/******************************************************************************/
extern int SF_chrCreate(lua_State *L);
extern int SF_itemCreate(lua_State *L);

extern int SF_addItem(lua_State *L);

extern int SF_setDialoguePath(lua_State *L);

extern int SF_posX(lua_State *L);
extern int SF_posY(lua_State *L);
extern int SF_posZ(lua_State *L);

extern int SF_moveWalking(lua_State *L);
extern int SF_moveTo(lua_State *L);

extern int SF_attack(lua_State *L);
extern int SF_actionBreak(lua_State *L);

extern int SF_triggerState(lua_State *L);

extern int SF_talkingWith(lua_State *L);
/******************************************************************************/
extern int SF_KeyPressed(lua_State *L);
/******************************************************************************/