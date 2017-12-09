/******************************************************************************/
struct _Shortcuts
{
	GuiObjs guiObjects;

	GuiImage *bg;
	GuiImage *slot[9];

	Reference<Item> slots[9];

	void create();
	void del();

	void useSlot(Int i);
} extern Shortcuts;
/******************************************************************************/