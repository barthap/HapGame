/******************************************************************************/
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
/******************************************************************************/
class DialogueClass
{
public:
	Bool now, laubindRegistered;

	AI *scriptPointer;

	Byte nextAnswer;
	const char* answerAction[3];

	Byte buttonID;

	Sound actSound;
	Byte actSoundType;

	Str textPath;
	Str answerPath[3];

	Int monologueAmount;
	Int actMonologue;
	Str monologueText[10];
	Str monologuePath[10];
	Bool monologueWithoutSound;
	Bool mDABSet;
	Str mDAB/*, mDAC, mDAP*/;

	Bool answerButtonV[3];

	//Gui controls
	Region dialogueRegion;
		Text chrText;
		Button answerButton[3];

	//Lua pointer
	lua_State* L;

	void create();
	void init(Char *dialogueFile);
	void update();
	void deinit();

	DialogueClass();
	~DialogueClass() { lua_close(L); }
};

extern int pto_DialogueText(lua_State *L);
extern int pto_Answer(lua_State *L);
extern int pto_Monologue(lua_State *L);
extern int pto_deleteDialogue(lua_State *L);

extern void pto_CDF0(Ptr);
extern void pto_CDF1(Ptr);
extern void pto_CDF2(Ptr);

extern void pto_callDialogueFunction(Byte buttonID);
/******************************************************************************/