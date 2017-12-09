/******************************************************************************/
#include "stdafx.h"
#include "Main.h"

Str fileToLoad;
bool loadNewGame = true;
/******************************************************************************/
void hideAllMenus()
{

}
/******************************************************************************/
void cicsOpenNewGame(Ptr)
{
	mainMenu.openNewGame(NULL);
}
void cicsOpenLoadGame(Ptr)
{
	mainMenu.openLoadGame(NULL);
}
void cicsOpenOptions(Ptr)
{
	mainMenu.openOptions(NULL);
}
/*void cicsOpenControllers(Ptr)
{
	mainMenu.openControllers(NULL);
}*/
/*void cicsOpenCredits(Ptr)
{
	mainMenu.openCredits(NULL);
}*/
void cicsEnd(Ptr)
{
	mainMenu.end(NULL);
}
/*void cicsDeinitCredits(Ptr)
{
	menuCredits.deinit();
}*/
/*void cicsDeinitControllers(Ptr)
{
	menuControllers.deinit();
}*/
/******************************************************************************/
_mainMenu        mainMenu;
_menuNewGame     menuNewGame;
_menuLoadGame    menuLoadGame;
//_menuControllers menuControllers;
//_menuCredits     menuCredits;
/******************************************************************************/
void loadAllMenus()
{
	/*                         MAIN MENU                         */
	if(mainMenu.guiFile.load("Gui/Obj/mainMenu.gobj"))
	   {
		   Gui += mainMenu.guiFile;

		   mainMenu.mainRegion = &mainMenu.guiFile.getRegion("mainRegion");
		                                      /* UPDATE HERE IF WIDTH IS CHANGED */
			mainMenu.mainRegion->pos(Vec2(-(D.w()-0.2), mainMenu.mainRegion->pos().y));

		   mainMenu.mainButton[0] = &mainMenu.guiFile.getButton("openNewGame");
		   mainMenu.mainButton[1] = &mainMenu.guiFile.getButton("openLoadGame");
		   mainMenu.mainButton[2] = &mainMenu.guiFile.getButton("openOptions");
		   //mainMenu.mainButton[3] = &mainMenu.guiFile.getButton("openControllers");
		   //mainMenu.mainButton[4] = &mainMenu.guiFile.getButton("openCredits");
		   mainMenu.mainButton[3] = &mainMenu.guiFile.getButton("exit");

		   mainMenu.mainButton[0]->func(cicsOpenNewGame);
		   mainMenu.mainButton[1]->func(cicsOpenLoadGame);
		   mainMenu.mainButton[2]->func(cicsOpenOptions);
		   //mainMenu.mainButton[3]->func(cicsOpenControllers);
		   //mainMenu.mainButton[4]->func(cicsOpenCredits);
		   mainMenu.mainButton[3]->func(cicsEnd);

		   mainMenu.mainRegion->visible(false);
			   mainMenu.mainButton[0]->visible(true);
			   mainMenu.mainButton[1]->visible(true);
			   mainMenu.mainButton[2]->visible(true);
			   mainMenu.mainButton[3]->visible(true);
			   //mainMenu.mainButton[4]->visible(true);
			   //mainMenu.mainButton[5]->visible(true);

		/*                         OPTIONS MENU                         */
		Options.create();
	
		/*                       CONTROLLERS MENU                       */
		/*menuControllers.window = &mainMenu.guiFile.getWindow("controllersWindow");
		 //menuCredits.window->pos(Vec2(-menuControllers.window->crect.w() * 0.50, /*-menuControllers.window->crect.h() * 0.50*//* 0.83));
		 //menuCredits.window->pos(Options.window.pos());

		menuControllers.text = &mainMenu.guiFile.getText("controllersText");
		menuControllers.button = &mainMenu.guiFile.getButton("controllersExit");

		menuControllers.button->func(cicsDeinitControllers);

		menuControllers.window->visible(false);*/

		/*                         CREDITS MENU                         */
		/*menuCredits.window = &mainMenu.guiFile.getWindow("creditsWindow");
		 //menuCredits.window->pos(Vec2(-(menuCredits.window->crect.w \ 2), menuCredits.window->pos().y));
		 //menuCredits.window->pos(Vec2(-menuCredits.window->crect.w() * 0.50, /*-menuCredits.window->crect.h() * 0.50*//* 0.83));

		menuCredits.text = &mainMenu.guiFile.getText("creditsText");
		menuCredits.button = &mainMenu.guiFile.getButton("creditsExit");

		menuCredits.button->func(cicsDeinitCredits);

		menuCredits.window->visible(false);*/
	}

}
/******************************************************************************/
//_mainMenu class
	void _mainMenu::init()
	{
		//Hide all menus and show Main Menu
		hideAllMenus();

		this->mainRegion->visible(true);
	}
	void _mainMenu::openNewGame(Ptr)
	{
		loadNewGame = true;
		fileToLoad = "";

		StateGame.set(0);

		mainMenu.deinit();
	}
	void _mainMenu::openLoadGame(Ptr)
	{
		loadNewGame = false;
		fileToLoad = "Saves/quicksave.sav";

		StateGame.set(0);
		mainMenu.deinit();
		
	}
	void _mainMenu::openOptions(Ptr)
	{
		Options.toggle();
	}
	/*void _mainMenu::openControllers(Ptr)
	{
		menuControllers.init();
	}
	void _mainMenu::openCredits(Ptr)
	{
		menuCredits.init();
	}*/
	void _mainMenu::end(Ptr)
	{
		this->exit = true;
	}
	void _mainMenu::deinit()
	{
		//Hide Main Menu without hiding any other
		this->mainRegion->visible(false);

		//Hide all menus
		Options.window.visible(false);
	}

//_menuNewGame class
	void _menuNewGame::init()
	{
		//Show New Game Menu
	}
	void _menuNewGame::deinit()
	{
		//Hide all New Game menus and show Main Menu
	}
//_menuLoadGame class
	void _menuLoadGame::init()
	{
		//Show New Game Menu
	}
	void _menuLoadGame::deinit()
	{
		//Hide all New Game menus and show Main Menu
	}
//_menuControllers class
	/*void _menuControllers::init()
	{
		menuCredits.window->pos(Options.window.pos());
		this->window->visible(true);
	}
	void _menuControllers::deinit()
	{
		this->window->visible(false);
	}
//_menuCredits class
	void _menuCredits::init()
	{
		this->window->visible(true);
	}
	void _menuCredits::deinit()
	{
		this->window->visible(false);
	}*/
/******************************************************************************/
