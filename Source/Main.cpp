/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Font *defaultFont;
TextDS mainTds;
/******************************************************************************/
//Bool newGame;
//Str  fileToLoad;
/******************************************************************************/
void InitPre()
{
   App.name("cRPG");
   //App.flag = APP_ALLOW_RT_FORWARD; //There is no longer APP_ALLOW_RT_FORWARD

   DataPath("Data");

   Paks.add("engine.pak");

   //Mods
   if(Str(App.cmdLine()).is())
   {
	   Memb<Str> tmpContainer;
	   
	   if(split("-", S + App.cmdLine(), true, tmpContainer))
	   {
			FREPA(tmpContainer)
			{
				Str modPath;

			    modPath += S + "Mods/";
			    modPath += tmpContainer[i];
			    modPath += "/main.mod";

			    ModClass *newMod = &Mods.New();

			    newMod->init((Char *)modPath(), tmpContainer[i]);
			}
	   }
	   /*Str modPath;
	   modPath += S + "Mods/";
	   modPath += App.cmdLine();
	   modPath += "/main.mod";
	   _Mod *newMod = &Mods.New();
	   newMod->init((Char *)modPath(), App.cmdLine());*/
   }

   D.ambPower(0.3).bloomOriginal(0.9).bloomScale(0.7).bloomOverbright(true).bloomCut(0.4).bloomHalf(false);
   D.viewRange(200);
   //Set default options
   D.full(0).sync(true);
   D.ambSoft(1).ambRange(0.2).ambJitter(true).ambContrast(2).ambHalfRes(true).ambMode(AMBIENT_FLAT);
   D.hpRt(false).hdr(false).hdrExposure(0.6);
   D.mtnMode(MOTION_NONE).mtnSmooth(true).mtnLeaks(2);
   D.bumpMode(BUMP_NORMAL);
   D.shdMapSize(512).shdMapNum(6).shdJitter(true).shdSoft(true).shdMode(SHADOW_MAP);
   D.grassRange(125);

   ConfigLoad("config.dat");
}
/******************************************************************************/
void setFont(Font *fontToSet)
{
	Gui.tds_button.font = fontToSet;
}
Bool Init()
{
   Physics.create(CSS_NONE, true, "Tools/PhysX");

   defaultFont = Fonts("Data/dafxter.font");
	mainTds.font = defaultFont;

   //Setting gui to use default font
   setFont(defaultFont);
   Gui.tds_button.font = defaultFont;
   Gui.tds_cmenu.font = defaultFont;
   Gui.tds_desc.font = defaultFont;
   Gui.tds_list.font = defaultFont;
   Gui.tds_text.font = defaultFont;
   Gui.tds_textline.font = defaultFont;
   Gui.tds_window.font = defaultFont;
	Gui.style_window.load("gui/style/window.gstl");
	Gui.style_region.load("gui/style/region.gstl");
	Gui.image_button=Images("gui/gfx/button.gfx");
	Gui.image_list_group=Images("gui/gfx/button.gfx");
	Gui.image_win_bar=Images("gui/gfx/bar.gfx");
	Gui.kb_lit=Color(0,0,0);

   Sun.image = Images("Gfx/Sky/sun.gfx");
   Sky.atmospheric();

   Clouds.layered.set(3, Images("Gfx/Clouds/Layers/0.gfx"));

   StateMenu.set(0);

   loadAllMenus();

   return true;
}
/******************************************************************************/
void Shut()
{
	ConfigSave("config.dat");
}
/******************************************************************************/
Bool Update()
{
   return true;
}
/******************************************************************************/
void Draw()
{
   
}
/******************************************************************************/
