/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
_StatsGui StatsGui;

TextDS statsGuiTDS;
/******************************************************************************/
void _StatsGui::create()
{
   //TDS
   statsGuiTDS = Gui.tds_text;
   statsGuiTDS.scale *= 1.5;
   statsGuiTDS.font = defaultFont;
   statsGuiTDS.align = (0, 0.95);

   //Window
   Gui += window.create(Rect_R(D.w() - 0.1, 0, 1.1, 1.75f), "Statystyki");
   window.visible(false);

   //Main Stats
   {
	   window += mainStatsT[0].create(Vec2(0.04, -0.2), "Si³a: "); 
	   window += mainStatsT[1].create(Vec2(0.04, mainStatsT[0].rect.min.y - 0.08), "Obrona: ");

		window +=levelInfo[0].create(Vec2(0.04, mainStatsT[1].rect.min.y - 0.32), "Poziom: ");
		window +=levelInfo[1].create(Vec2(0.04, levelInfo[0].rect.min.y - 0.08), "Doœwiadczenie: ");
		window +=levelInfo[2].create(Vec2(0.04, levelInfo[1].rect.min.y - 0.08), "Pozosta³o exp: ");

	   FREP(4)
	   {
		   mainStatsT[i].tds = &statsGuiTDS;
		   mainStatsT[i].size(Vec2(0.49, 0.09));
	   }
		FREP(3)
		{
			levelInfo[i].tds = &statsGuiTDS;
			levelInfo[i].size(Vec2(0.49, 0.09));
		}

	   //window += mainStatsV[0].create(Vec2(0.54, -0.2), "0"); 
	   //window += mainStatsV[1].create(Vec2(0.53, mainStatsV[0].rect.min.y - 0.08), "0");
	   //window += mainStatsV[2].create(Vec2(0.53, mainStatsV[1].rect.min.y - 0.08), "0");
	   //window += mainStatsV[3].create(Vec2(0.53, mainStatsV[2].rect.min.y - 0.08), "0");

	   /*FREP(4)
	   {
		   mainStatsV[i].tds = &statsGuiTDS;
	   }*/
   }
}
void _StatsGui::toggle()
{
	window.visibleToggle();
	setMouseVisibility();
}
void _StatsGui::update()
{
	mainStatsT[0].set(S + "Si³a: " + Players[0].strength);
	mainStatsT[1].set(S + "Obrona: " + (int)Players[0].defense);

	levelInfo[0].set(S+"Poziom: "+(int)Players[0].lvl);
	levelInfo[1].set(S+"Doœwiadczenie: "+(int)Players[0].exp);
	levelInfo[2].set(S+"Pozosta³o exp: "+(int)(GetExp(Players[0].lvl+1)-Players[0].exp));
}
/******************************************************************************/