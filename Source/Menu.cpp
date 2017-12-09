/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Image *img;
MusicTheme *backgroundMusic;

Str BgImgs[]=
{
    "0.gfx",
	"1.gfx",
	"2.gfx",
	"3.gfx",
	"4.gfx",
	"5.gfx",
	"6.gfx",
};
/******************************************************************************/
Bool InitMenu()
{
   img = Images("gfx/bg/menu.gfx");			//Images(S + "gfx/bg/" + BgImgs[Random(Elms(BgImgs))]);

   mainMenu.init();

	backgroundMusic = new MusicTheme();
	*backgroundMusic += "sound/menumusic.ogg";
	Music.play(backgroundMusic);

	Options.window.visible(false);
	PauseGui.window.visible(false);
	Ms.visible(true);

   return true;
}
void ShutMenu()
{
	Music.play(NULL);
   //mainMenu.deinit();
}
/******************************************************************************/
Bool UpdateMenu()
{   
   if(mainMenu.exit == true) return false;

   Gui.update();
   
   return true;
}
void DrawMenu()
{
   D.clear();

   img->drawFs();
	D.text(0, 0.9, "Menu gry cRPG :)");

   Gui.draw();
}
/******************************************************************************/
State StateMenu(UpdateMenu,DrawMenu,InitMenu,ShutMenu);
/******************************************************************************/
