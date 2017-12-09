/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
DialogueClass *dialoguePointer;
/******************************************************************************/
DialogueClass::DialogueClass()
{
		now = false;
		laubindRegistered = false;

		actSound.create("", false, VOLUME_VOICE);

		scriptPointer = NULL;

		//Lua init
		L = lua_open();
		luaL_openlibs(L);

		//Defining Lua functions
		SF_registerAllFunctions(L);
		
}
/******************************************************************************/
void DialogueClass::create()
{
	

	//Region
	Gui += dialogueRegion.create(Rect_C(Vec2(0, -0.875), 2.666, 0.4));
	dialogueRegion.visible(false);

	//Text
	dialogueRegion += chrText.create(Rect_C(Vec2(dialogueRegion.rect.w() * 0.5, -0.055), 2.222, 0.05), "Tu bêdzie tekst wypowiadany przez postaæ");
	chrText.tds->font = defaultFont;

	//Answers
	dialogueRegion += answerButton[0].create(Rect_C(Vec2(dialogueRegion.rect.w() * 0.5, -0.135), 2.222, 0.05), "OdpowiedŸ pierwsza").func(pto_CDF0);
	dialogueRegion += answerButton[1].create(Rect_C(Vec2(dialogueRegion.rect.w() * 0.5, -0.215), 2.222, 0.05), "OdpowiedŸ druga").func(pto_CDF1);
	dialogueRegion += answerButton[2].create(Rect_C(Vec2(dialogueRegion.rect.w() * 0.5, -0.295), 2.222, 0.05), "OdpowiedŸ trzecia").func(pto_CDF2);
	FREP(3)
	{
		answerButton[i].visible(false);
	}
}
void DialogueClass::init(Char *dialogueFile)
{
	//scriptPointer = NULL;

	dialogueRegion.style->load("gui/style/window.gstl");
	dialogueRegion.visible(true);
	chrText.visible(true);

	monologueWithoutSound = false;

	nextAnswer = 0;
	FREP(3)
	{
		answerAction[i] = "";
	}

	FREP(3)
	{
		answerButton[i].visible(false);
	}

	char tmp[128];
	Set(tmp, S+"Scripts/"+dialogueFile);

	if(!laubindRegistered)
	{
		laubindRegistered = true;
		RegisterLuaBind(L);
		RegisterWorldGlobals(L);
	}

	//Run dialogue script
	luaL_dofile(L, tmp);

	lua_getglobal(L, "main");
    lua_call(L, 0, 0);

	now = true;
	setMouseVisibility();
}
void DialogueClass::update()
{
	if(now == true)
	{
		if(!actSound.playing())
		{
			if(Dialogue.actSoundType == 0)
			{
				FREP(3)
				{
					if(Dialogue.answerButtonV[i] == true) Dialogue.answerButton[i].visible(true);
				}
			}
			else if(Dialogue.actSoundType == 1)
			{
				Dialogue.nextAnswer = 0;

				Dialogue.chrText.set("");
				FREP(3)
				{
					if(Dialogue.monologueWithoutSound == false) Dialogue.answerButtonV[i] = false;
					Dialogue.answerButton[i].visible(false);
					Dialogue.answerButton[i].disabled(false);
				}

				if(Dialogue.monologueWithoutSound == true)
				{
					Dialogue.answerButton[0].text = mDAB;
					//Dialogue.answerAction[0] = (const char *)Str(mDAC)();
					//Dialogue.answerPath[0] = mDAP;

					Dialogue.monologueWithoutSound = false;

					if(Dialogue.actMonologue == Dialogue.monologueAmount)
					{
						Dialogue.actSoundType = 0;

						Dialogue.answerButton[0].text = mDAB;

						Dialogue.monologueWithoutSound = false;
					}
					else
					{
						if(Dialogue.monologuePath[Dialogue.actMonologue + 1] == Str("")() && (Dialogue.actMonologue + 1) != Dialogue.monologueAmount)
						{
							Dialogue.monologueWithoutSound = true;
						}

						Dialogue.actMonologue += 1;

						Dialogue.actSound.del();
						Dialogue.actSoundType = 2;
						Dialogue.chrText.set(Dialogue.monologueText[Dialogue.actMonologue]);
						Dialogue.actSound.play(Dialogue.monologuePath[Dialogue.actMonologue]);

						if(Dialogue.monologueWithoutSound == true)
						{
							if(Dialogue.mDABSet == false)
							{
								mDAB = Dialogue.answerButton[0].text;
								Dialogue.mDABSet = true;
							}
							//mDAC = Dialogue.answerAction[0];
							//mDAP = Dialogue.answerPath[0];

							Dialogue.answerButton[0].text = "(...)";
							//Dialogue.answerAction[0] = "";
							//Dialogue.answerPath[0] = "";

							Dialogue.answerButton[0].visible(true);
						}
					}
				}
				else
				{
					lua_getglobal(Dialogue.L, Dialogue.answerAction[Dialogue.buttonID]);
					lua_call(Dialogue.L, 0, 0);
				}
			}
			else if(Dialogue.actSoundType == 2)
			{
				if(Dialogue.monologueWithoutSound == true)
				{
					if(Dialogue.mDABSet == false)
					{
						mDAB = Dialogue.answerButton[0].text;
						Dialogue.mDABSet = true;
					}
					//mDAC = Dialogue.answerAction[0];
					//mDAP = Dialogue.answerPath[0];

					Dialogue.answerButton[0].text = "(...)";
					//Dialogue.answerAction[0] = "";
					//Dialogue.answerPath[0] = "";

					Dialogue.answerButton[0].visible(true);
				}
				else
				{
					if(Dialogue.actMonologue == Dialogue.monologueAmount)
					{
						Dialogue.actSoundType = 0;

						Dialogue.answerButton[0].text = mDAB;

						Dialogue.monologueWithoutSound = false;
					}
					else
					{
						Dialogue.monologueWithoutSound = false;

						if(Dialogue.monologuePath[0] == Str("")() && Dialogue.actMonologue == Dialogue.monologueAmount)
						{
							Dialogue.monologueWithoutSound = true;
						}

						Dialogue.actMonologue += 1;

						Dialogue.actSound.del();
						Dialogue.actSoundType = 2;
						Dialogue.chrText.set(Dialogue.monologueText[Dialogue.actMonologue]);
						Dialogue.actSound.play(Dialogue.monologuePath[Dialogue.actMonologue]);
					}
				}
			}
		}
	}
}
void DialogueClass::deinit()
{
	//Region
	//dialogueRegion.del();
	dialogueRegion.visible(false);
	dialogueRegion.style->load("gui/style/region.gstl");

	//Text
	//chrText.del();
	//chrText.visible(false);

	//Answers
	//answerButton[0].del();
	//answerButton[1].del();
	//answerButton[2].del();
	/*FREP(3)
	{
		answerButton[i].visible(false);
	}*/

	now = false;
}

int pto_DialogueText(lua_State *L)
{
	Dialogue.chrText.set(lua_tostring(L, 1));
	Dialogue.textPath = lua_tostring(L, 2);

	Dialogue.actSoundType = 0;
	Dialogue.actSound.play(Dialogue.textPath);

	return 0;
}
int pto_Answer(lua_State *L)
{
	if(Dialogue.nextAnswer <= 2)
	{
		Dialogue.answerButton[Dialogue.nextAnswer].text = lua_tostring(L, 1);
		Dialogue.answerAction[Dialogue.nextAnswer] = lua_tostring(L, 2);
		Dialogue.answerPath[Dialogue.nextAnswer] = lua_tostring(L, 3);

		//Dialogue.answerButton[Dialogue.nextAnswer].visible(true);
		Dialogue.answerButtonV[Dialogue.nextAnswer] = true;
		
		Dialogue.nextAnswer += 1;
	}

	return 0;
}
int pto_Monologue(lua_State *L)
{
	Dialogue.monologueWithoutSound = false;
	Dialogue.mDABSet = false;

	Dialogue.actMonologue = 0;
	Dialogue.monologueAmount = lua_tonumber(L, 1) - 1;

	for(int i = 0; i <= Dialogue.monologueAmount; i++)
	{
		Dialogue.monologueText[i] = lua_tostring(L, i * 2 + 2);
		Dialogue.monologuePath[i] = lua_tostring(L, i * 2 + 3);
	}

	Dialogue.actSound.del();
	Dialogue.actSoundType = 2;
	Dialogue.chrText.set(Dialogue.monologueText[0]);
    Dialogue.actSound.play(Dialogue.monologuePath[0]);

	if(Dialogue.monologuePath[0] == Str("")())
	{
		Dialogue.monologueWithoutSound = true;
	}

	return 0;
}
int pto_deleteDialogue(lua_State *L)
{
	if(Dialogue.scriptPointer != NULL) Dialogue.scriptPointer->isTalking = false;
	Dialogue.deinit();
	setMouseVisibility();

	return 0;
}

void pto_CDF0(Ptr)
{
	pto_callDialogueFunction(0);
}
void pto_CDF1(Ptr)
{
	pto_callDialogueFunction(1);
}
void pto_CDF2(Ptr)
{
	pto_callDialogueFunction(2);
}

void pto_callDialogueFunction(Byte buttonID)
{
	FREP(3)
	{
		Dialogue.answerButton[i].disabled(true);
	}

	Dialogue.buttonID = buttonID;

	Dialogue.actSound.del();
	Dialogue.actSoundType = 1;
    Dialogue.actSound.play(Dialogue.answerPath[buttonID]);
}
/******************************************************************************/