/******************************************************************************/
//extern GuiObjs questGuiObjs;

struct _QuestGui
{
	GuiStyle windowStyle;

	List<_Quest> questList;

	Window questWindow;

	Region questRegion;
	Text textQuestName;
	Text textQuestDescription;

	void create();
	void update();

	void setGui();
	void toggle();
};

extern _QuestGui QuestGui;
/******************************************************************************/