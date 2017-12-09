/*
 * Copyright (c) Hapex 2009. All Rights Reserved.
 * Plik Options.cpp www.hapgame.hapex.com.pl
/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
OPTIONS Options;
/******************************************************************************/
// OPTION TEXT VALUES
/******************************************************************************/
static ListGroup mode_list_group[]=
{
   ListGroup(DATA_VECI2,0,SIZE(VecI2),LGW_PARENT,L"Size"),
};
static CChar8 *shd_size_t[]=
{
   "512",
   "768",
   "1024",
   "1536",
   "2048",
};
static CChar8 *shd_soft_t[]=
{
   "0",
   "1",
   "2",
};
static CChar8 *mtn_mode_t[]=
{
   "Wy³¹czone",
   "Œrednie",
   "Du¿e",
};
static CChar8 *shd_mod_t[]=
{
   "Nie",
   "Tak",
   "Tak HW",
};
static CChar8 *aspect_t[]=
{
	"4:3",
	"15:9",
	"15:10",
	"16:9",
	"16:10",
	"Auto",
};
   static CChar8 *blurs_t[]=
   {
      "1",
      "2",
      "3",
      "4",
   };

   static CChar8 *pads_t[]=
   {
	   "Klawiatura i Mysz",
	   "PlayStation 3 Pad",
	   "Xbox 360 Pad",
	   "Pad komputerowy",
   };
/******************************************************************************/
// SET OPTIONS INTO 'D' DISPLAY CONTROL
/******************************************************************************/
static void Mode(Ptr)
{
   Int m=Options.mode();
   if(InRange(m,D.modes()))
   {
      VecI2 mode=D.modeSize()[m];
      D.mode(mode.x,mode.y);
      Options.toGui();
   }
}
static void ShdSize(Ptr)
{
   Int s=Options.shd_size();
   if(InRange(s,shd_size_t))
   {
      D.shdMapSize(TextInt(shd_size_t[s]));
      Options.toGui();
   }
}
static void MotionMode(Ptr){D.mtnMode    (            Options.mtn_mode   () ? MOTION_HIGH   : MOTION_NONE).mtnSmooth(Options.mtn_mode()==2); Options.toGui();}
static void Shd       (Ptr){D.shdMode    (            (SHADOW_MODE)Options.shd        ()	); Options.toGui();}
static void Sync      (Ptr){D.sync       (            (Byte)Options.sync       ()!=0); Options.toGui();}
static void AmbMode   (Ptr){D.ambMode    (            (AMBIENT_MODE)Options.amb_mode   ()   ); Options.toGui();}
static void ShdSoft   (Ptr){D.shdSoft    (            Options.shd_soft   ()   ); Options.toGui();}
static void ShdJitter (Ptr){D.shdJitter  (            Options.shd_jitter ()!=0); Options.toGui();}
static void BumpMode  (Ptr){D.bumpMode   (            Options.bump_mode  () ? BUMP_PARALLAX : BUMP_FLAT ); Options.toGui();}
static void VolOn     (Ptr){D.volLight   (            Options.voll       ()!=0); Options.toGui();}
static void HpRt      (Ptr){D.hpRt       (            Options.hprt       ()!=0); Options.toGui();}
static void FScreen	 (Ptr){D.full		  (				Options.fscreen	 ()!=0); Options.toGui();}
static void Gama		 (Ptr){D.gamma		 (					Options.gama		 ()	); Options.toGui();}
static void	ControllerFun(Ptr){activeCtrl = (CONTROLLER)Options.controller(); Options.toGui();}
static void SteeringShow(Ptr){Options.instructionWnd.show();Options.instructionWnd.moveToTop();}
static void SteeringHide(Ptr){Options.instructionWnd.hide();}
/******************************************************************************/
// CREATE
/******************************************************************************/
void Powr(Ptr)
{
	Options.toggle();
}
void OPTIONS::create()
{
	CChar8 *tab[]=
   {
      "Grafika",
      "DŸwiêki",
      "Ró¿ne",
   };

   Gui+=window.create(Rect_C(-0.458,0.147,2.2,1.65),"Ustawienia");
	window+=tabs  .create(Rect_LU(0,0,window.crect.w(),0.08),0,tab,Elms(tab));

	Gui+=instructionWnd.create(Rect_C(-0.458,0.147,2.2,1.65),"Sterowanie");
	instructionWnd.hide();

	Flt wcrectw = window.crect.w()/2;
   Flt y=-0.07, // starting position
       h= 0.07, // element height
       s= 0.11; // spacing between elements

   Flt xl=wcrectw*0.25, // left  x position
       xr=wcrectw*0.75, // right x position
       w =wcrectw*0.40, // element width
		 xl2=xl+wcrectw,
		 xr2=xr+wcrectw;

   Str steeringPath("gfx/steering/");
   //INSTRUKCJA STEROWANIA
   Flt y2 = y, s2 = s*0.75;

   instructionWnd+=tBtn.create(Vec2(xl, y2), "Akcja");
   instructionWnd+=tKb.create(Vec2(wcrectw-wcrectw/3, y2), "Klawiatura/Mysz");
   instructionWnd+=tPS3.create(Vec2(wcrectw, y2), "PlayStation3 Pad");
   instructionWnd+=tX360.create(Vec2(wcrectw+wcrectw/3, y2), "Xbox 360 Pad");
   instructionWnd+=tPad.create(Vec2(xr2, y2), "Pad komputerowy"); y2 -=s2*1.5;

   instructionWnd+=tMoving.create(Vec2(xl, y2), "Poruszanie sie");
   instructionWnd+=kbMoving.create(Vec2(wcrectw-wcrectw/3, y2), "WASD"); y2-=s2;

   instructionWnd+=tAction.create(Vec2(xl, y2), "Wykonaj akcjê");
   instructionWnd+=kbAction.create(Vec2(wcrectw-wcrectw/3, y2), "F");
   instructionWnd+=pAction.create(Rect_C(wcrectw, y2, h/2), Images(steeringPath+"ps3/hold-x.gfx"));
   instructionWnd+=xAction.create(Rect_C(wcrectw+wcrectw/3, y2, h/2), Images(steeringPath+"x360/hold-A.gfx"));y2-=s2;

   instructionWnd+=steeringHide.create(Rect_C(wcrectw, y2, w, h), "Zamknij").func(SteeringHide);

   y-=s;
	tabs.tab(0)+= tmode       .create(Vec2(xl,y),"Rozdzielczoœæ"   ); tabs.tab(0)+= mode       .create(Rect_C(xr,y,w,h)                                  ).func(Mode      ).setGroups(mode_list_group,Elms(mode_list_group)).setData(D.modeSize(),D.modes());
	tabs.tab(0)+= tgama			.create(Vec2(xl2,y),"Jasnoœæ"				 ); tabs.tab(0)+= gama		 .create(Rect_C(xr2,y,w,h), D.gamma()).desc("Ustawia jasnoœæ ekranu"); y-=s;
   tabs.tab(0)+= tshd_size   .create(Vec2(xl,y),"Rozmiar ShadowMap"  ); tabs.tab(0)+= shd_size   .create(Rect_C(xr,y,w,h),shd_size_t   ,Elms(shd_size_t   )).func(ShdSize   ).desc("Shadow map resolution\nhigher resolutions reduce blockiness of shadows"); y-=s;
   tabs.tab(0)+= tshd_soft   .create(Vec2(xl,y),"Wyg³adzanie cieni"  ); tabs.tab(0)+= shd_soft   .create(Rect_C(xr,y,w,h),shd_soft_t   ,Elms(shd_soft_t   )).func(ShdSoft   ).desc("Uruchamia wyg³adzanie cieni (Shadow Softing)"); y-=s;
   tabs.tab(0)+= tmtn_mode   .create(Vec2(xl,y),"Rozmazywanie"       ); tabs.tab(0)+= mtn_mode   .create(Rect_C(xr,y,w,h),mtn_mode_t   ,Elms(mtn_mode_t   )).func(MotionMode).desc("Rozmazuje szybko poruszaj¹ce siê obiekty"); y-=s;
   tabs.tab(0)+= tshd        .create(Vec2(xl,y),"Cienie"             ); tabs.tab(0)+= shd        .create(Rect_C(xr,y,w,h),shd_mod_t		,Elms(shd_mod_t	 )).func(Shd		 ).desc("W³¹cza cienie. Opcja HW uruchamia sprzêtow¹\nobs³ugê cieni (Tylko karty z Shader Model 3.0)"); y-=s;
   tabs.tab(0)+= tvoll        .create(Vec2(xl,y),"Volumetryczne Œwiat³a");tabs.tab(0)+= voll        .create(Rect_C(xr,y,h,h), VolOn	   ).desc("Uruchamia automatyczne dostosowywanie jasnoœci (Tone Mapping)"); y-=s;
	voll.func(VolOn);
   tabs.tab(0)+= thprt       .create(Vec2(xl,y),"Wysoka dok³adnoœæ RT's"); tabs.tab(0)+= hprt    .create(Rect_C(xr,y,h,h), HpRt	   ).desc("Uruchamia 64 bitowe renderowanie\nTo zwiêksza liczbê widocznych kolorów"); y-=s;
	hprt.func(HpRt);
	tabs.tab(0)+= tfscreen	  .create(Vec2(xl,y),"Pe³ny Ekran"			); tabs.tab(0)+= fscreen	 .create(Rect_C(xr,y,h,h), FScreen).desc("W³¹cza/Wy³¹cza pe³ny ekran"); y-=s;
	fscreen.func(FScreen);
   tabs.tab(0)+= tsync       .create(Vec2(xl,y),"Synchronizacja"     ); tabs.tab(0)+= sync       .create(Rect_C(xr,y,h,h), Sync		).desc("Synchronizujê prêdkoœæ gry z liczb¹ klatek/sek.\nMo¿e te¿ ograniczaæ liczbê klatek/sek."); y-=s;
	sync.func(Sync);
	tabs.tab(0)+= tbump_mode  .create(Vec2(xl,y),"Nierównoœci"        ); tabs.tab(0)+= bump_mode  .create(Rect_C(xr,y,h,h),D.bumpMode()	).func(BumpMode).desc("Symuluje nierównoœci terenu (Bump Mapping).\nW³¹czone nierównoœci obci¹¿aj¹ kartê graficzn¹"); y-=s;
	shd_jitter.func(ShdJitter);
	window+=back		  .create(Rect_C(wcrectw,y,w,h),"Powrót").func(Powr);

	y=-0.07, // starting position
		y-=(s*2);
	tabs.tab(1)+= vol_global .create(Rect_C(xr,y,w,h),SoundVolume.global ()); tabs.tab(1)+= tvol_global .create(Vec2(xl,y),"G³oœnoœæ g³ówna"); y-=s;
	y-=s;
	tabs.tab(1)+= vol_fx     .create(Rect_C(xr,y,w,h),SoundVolume.fx     ()); tabs.tab(1)+= tvol_fx     .create(Vec2(xl,y),"Efekty"     ); y-=s;
   tabs.tab(1)+= vol_music  .create(Rect_C(xr,y,w,h),SoundVolume.music  ()); tabs.tab(1)+= tvol_music  .create(Vec2(xl,y),"Muzyka"  ); y-=s;
   tabs.tab(1)+= vol_ambient.create(Rect_C(xr,y,w,h),SoundVolume.ambient()); tabs.tab(1)+= tvol_ambient.create(Vec2(xl,y),"Otoczenie");y-=s;
	tabs.tab(1)+= vol_voice  .create(Rect_C(xr,y,w,h),SoundVolume.voice	()); tabs.tab(1)+= tvol_voice	 .create(Vec2(xl,y),"Mowa");

	y=-0.07,
	y-=(s*2);
	tabs.tab(2)+= msspd		.create(Rect_C(xr,y,w,h),Ms.speed());				tabs.tab(2)+=tmsspd.create(Vec2(xl,y),"Czu³oœæ myszy");y-=s;
	tabs.tab(2)+= tSteering.create(Vec2(xl, y),"Sterowanie"); tabs.tab(2)+=controller.create(Rect_C(xr, y, w, h), pads_t, Elms(pads_t)).func(ControllerFun).desc("Czym chcesz sterowaæ w grze?");y-=s;
	tabs.tab(2)+=steering.create(Rect_C(wcrectw, y, w*1.5, h), "Instrukcje sterowania").func(SteeringShow);

   toGui();
}
/******************************************************************************/
// CONVERT OPTIONS FROM 'D' DISPLAY CONTROL TO VISUAL GUI
/******************************************************************************/
void OPTIONS::toGui()
{

   // set gui settings from current display options
   {
      Int set;

      set=-1; REP(D.modes())if(D.x()==D.modeSize()[i].x && D.y()==D.modeSize()[i].y){set=i; break;} mode.set(set);

      set=-1; REPA(shd_size_t)if(TextInt(shd_size_t[i])==D.shdMapSize()){set=i; break;} shd_size.set(set);

      if(!D.mtnMode  ())mtn_mode.set(0);else
      if( D.mtnSmooth())mtn_mode.set(2);else
                        mtn_mode.set(1);
      sync       .set(    D.sync       ()                         );
      shd        .set(Mid(D.shdMode    (),0,Elms(shd_mod_t	  )-1));
      shd_soft   .set(Mid(D.shdSoft    (),0,Elms(shd_soft_t   )-1));
      shd_jitter .set(	  D.shdJitter  ()									);
		bump_mode  .set(    D.bumpMode  ()									);
      voll       .set(	  D.volLight   ()									);
      hprt       .set(	  D.hpRt       ()									);
		fscreen	  .set(	 D.full()											);
		gama		  .set(		D.gamma		()									);
		msspd		  .set(		Ms.speed		()									);
		controller .set(activeCtrl);
   }
}
/******************************************************************************/
// TOGGLE OPTIONS WINDOW VISIBILITY
/******************************************************************************/
void OPTIONS::toggle()
{
   // toggle options window visibility
   window.visibleToggle();

   // disable inventory window
   //InvGui.window.disabled(window.visible());
   //FlagSet(InvGui.window.flag,WIN_MOVABLE,window.hidden());

   // set mouse visibility
   setMouseVisibility();
}
/******************************************************************************/
// UPDATE OPTIONS
/******************************************************************************/
void OPTIONS::update()
{
	D.gamma(gama());
	SoundVolume.fx     (Options.vol_fx     ());
   SoundVolume.music  (Options.vol_music  ());
   SoundVolume.ambient(Options.vol_ambient());
	SoundVolume.global (Options.vol_global	());
	Ms.speed(Options.msspd());
}
/******************************************************************************/
