/*
 * Copyright (c) Hapex 2009. All Rights Reserved.
 * Plik Options.h www.hapgame.hapex.com.pl
/******************************************************************************/
struct OPTIONS
{
   Window   window, instructionWnd;
   Text     tfps,tmode,tsync,trender_type,tshd,tshd_size,tshd_soft,tshd_jitter,tbump_mode,tmtn_mode,
		tmtn_leak,tamb_mode,tvoll,thprt, tfscreen,ttexq,tgama,tmsspd, tSteering;
   ComboBox       mode, render_type, shd_size, shd_soft, mtn_mode, mtn_leak, amb_mode, shd, controller;
	CheckBox			fscreen, sync, shd_jitter, voll, hprt, bump_mode;
	Slider		texq, gama, msspd;
	Button	back, steering, steeringHide;
	Tabs tabs;
	Slider      vol_fx     , // volume bars
            vol_music  ,
            vol_ambient,
				vol_global,
				vol_voice;
	Text       tvol_fx     , // volume text
           tvol_music  ,
           tvol_ambient,
			  tvol_global,
			  tvol_voice;

   void create(); // create gui controls
   void toGui (); // set gui controls from display settings
   void toggle(); // toggle visibility of options
   void update(); // set fps text counter

	Text tBtn, tKb, tPS3, tX360, tPad;
	Text tMoving, tAction;
	Text kbMoving, kbAction;
	GuiImage pMoving, pAction;
	GuiImage xMoving, xAction;

}extern
   Options;
/******************************************************************************/