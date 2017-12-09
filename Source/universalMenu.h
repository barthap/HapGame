/******************************************************************************/
extern void hideAllMenus();
extern void loadAllMenus();

extern void cicsOpenNewGame(Ptr);
extern void cicsOpenLoadGame(Ptr);
extern void cicsOpenOptions(Ptr);
//extern void cicsOpenControllers(Ptr);
//extern void cicsOpenCredits(Ptr);
extern void cicsEnd(Ptr);

extern Str fileToLoad;
extern bool loadNewGame;
/******************************************************************************/
struct _mainMenu
{
public:
	void init();

	void openNewGame(Ptr);
	void openLoadGame(Ptr);
	void openOptions(Ptr);
	//void openControllers(Ptr);
	//void openCredits(Ptr);
	void end(Ptr);

	void deinit();

	GuiObjs guiFile;
		Region *mainRegion;
		Button *mainButton[3];

	bool exit;
} extern mainMenu;

class _menuNewGame
{
public:
    void init();

	void deinit();

	GuiObjs guiFile;
} extern menuNewGame;

class _menuLoadGame
{
public:
	void init();

	void deinit();

	GuiObjs guiFile;
} extern menuLoadGame;

/*class _menuControllers
{
public:
	void init();

	void deinit();

	Window *window;
		Text *text;
		Button *button;
} extern menuControllers;*/

/*class _menuCredits
{
public:
	void init();

	void deinit();

	Window *window;
		Text *text;
		Button *button;
} extern menuCredits;*/
/******************************************************************************/
