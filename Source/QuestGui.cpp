/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
//GuiObjs questGuiObjs;
_QuestGui QuestGui;
/******************************************************************************/
void _QuestGui::create()
{
	//if(questGuiObjs.load("Gui/Obj/quest.gobj"))
	//{
		//Gui += questGuiObjs;

		windowStyle.load("Gui/Style/window.gstl");

		Gui += questWindow.create(Rect_LU(-0.768, -0.215, 1.41, 0.72), "Zadania");
		questWindow.style = &windowStyle;
		questWindow.pos(Vec2(-0.768, -0.215));
		questWindow.visible(false);

		questWindow += questRegion.create(Rect_LU(0.04, -0.03, 0.71, 0.6));
		questWindow += textQuestName.create(Rect_LU(0.78, -0.03, 0.6, 0.09), "");
		questWindow += textQuestDescription.create(Rect_LU(0.77, -0.15, 0.61, 0.48), "");

		textQuestDescription.auto_line = AUTO_LINE_SPACE;

		ListGroup lg[]=
        {
			ListGroup(MEMBER(_Quest, questName), questRegion.crect.w(), L"Nazwa"),
        };

        questRegion += questList.create(lg, Elms(lg));

        questList.cur_mode = LCM_MOUSE;
        questList.draw_mode = LDM_LIST;
	//}
}
void _QuestGui::update()
{
	if(Ms.bp(0))
	{
		if(Gui.ms() == &questList)
		{
			Int currentItem = questList.cur;

			if(currentItem > -1)
			{
				textQuestName.set(Quests[currentItem].questName);
				textQuestDescription.set(Quests[currentItem].questDescription);
			}
		}
	}
}

void _QuestGui::setGui()
{
	questList.clear();

	Bool *is = Alloc<Bool>(Quests.elms());

    SetMem(is, 1, Quests.elms());
    questList.setData(Quests, is);

    Free(is);
}
void _QuestGui::toggle()
{
	textQuestName.set("");
	textQuestDescription.set("");
	questWindow.visibleToggle();
}
/******************************************************************************/