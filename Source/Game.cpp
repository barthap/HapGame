/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
#include "Console.h"
/******************************************************************************/
Game::Obj *Lit; //Highlighted world object
Game::ObjMemx<Game::Static>			Statics;
Game::ObjMemx<Tree>					Trees;
Game::ObjMemx<Door>					Doors;
Game::ObjMemx<Item>					Items;
Game::ObjMemx<Player>				Players;
Game::ObjMemx<AI>					Chrs;
Game::ObjMemx<Fireball>				Fireballs;
Game::ObjMemx<Teleport1>			Teleports1;
Game::ObjMemx<Horse>				Horses;
Game::ObjMemx<DestructibleEC>		Destructs;
Game::ObjMemx<Trigger>				Triggers;
Game::ObjMemx<Sound3D>				Sounds3D;
Game::ObjMemx<Game::ObjLightPoint>	Lights;
Game::ObjMemx<Game::ObjParticles>	Particless;
Game::ObjMemx<Torchiere>			Torchieres;

/******************************************************************************/
UInt View = VIEW_TPP; //Current VIEW_MODE
DialogueClass Dialogue;

SPELL_TYPE Spell;

Bool modsInitialized;

_Weather *weather; 
/******************************************************************************/
/////////////////////////////CONSOLE COMMANDS///////////////////////////////////
EEDevTools::Console console;

Bool displayFPS;
bool drawPhys;

/******************************************************************************/
Flt progress;
Flt	strongress;

Progress *proHealth;
Progress *proStrong;

Text *tmana, *theal;

GuiObjs guiObjs;

Flt h, mana;

bool CameraMode = false;

//dla shadera
Flt x = 0.0f;
Vec color1(0.0f, 0.0f, 1.0f);
Vec color2(1.0f, 0.2f, 0.0f);
Flt userAlpha = 0.0f;
Button aUp, aDn;

Window w;

ObjectiveMarker objectiveMarker;

//AI progress
Progress aiProHealth;
/******************************************************************************/

void CompileShaders()
{
  // ShaderCompile("Shader/ColourShader.cpp", S+DataPath()+"Shader/2/User/ColourShader", SM_2);
  // ShaderCompile("Shader/ColourShader.cpp", S+DataPath()+"Shader/3/User/ColourShader", SM_3);
  // ShaderCompile("Shader/ColourShader.cpp", S+DataPath()+"Shader/4/User/ColourShader", SM_4);
}
void getObjectUnderCursor()
{
	Lit = NULL;
	if(!Gui.ms() || Gui.ms() == Gui.desktop())
	{
		Vec pos, dir;
		
		OrientP &head=Players[0].cskel.getPoint("head");
		pos = head.pos;
		dir=head.dir;

		PhysHit phys_hit;
		if(Physics.ray(pos, dir * D.viewRange(), &phys_hit, ~IndexToFlag(AG_CONTROLLER))) Lit = phys_hit.obj;
		
		if(!Lit)
		{
			ScreenToPosDir(Vec2(0, 0), pos, dir);

			if(Physics.ray(pos, dir * D.viewRange(), &phys_hit, ~IndexToFlag(AG_CONTROLLER))) Lit = phys_hit.obj;
		}
	}
}
/******************************************************************************/
void setMouseVisibility()
{
	Ms.visible(Options.window.visible() || InvGui.window.visible() || StatsGui.window.visible() || Dialogue.now || LootGui.L_window.visible() || QuestGui.questWindow.visible() || Storage.L_window.visible() || MerchantGui.window.visible() || PauseGui.window.visible());
}
static Vec Col(Byte R, Byte G, Byte B)
{
   return Vec(R, G, B) / 255;
}
/******************************************************************************/
Bool split(Str delimeter, Str string, Bool removeSpaces, Memb<Str> &container)
{
	Str afterSpaces;
	Int actElement;
	Str tmpString;

	actElement = -1;
	tmpString = "";

	if(removeSpaces)
	{
		FREP(string.length())
		{
			if(!Contains(S + string[i], " ")) afterSpaces += string[i];
		}
	}
	else
	{
		afterSpaces = string;
	}

	FREP(afterSpaces.length())
	{
		if(Contains(S + afterSpaces[i], delimeter))
		{
			container.New();

			actElement = container.elms() - 1;
		}
		else
		{
			container[actElement] += afterSpaces[i];
		}
	}

	return true;
}
Int countSigns(Str sign, Str string)
{
	Int result;

	result = 0;

	FREP(string.length())
	{
		if(Contains(Str(string[i]), sign)) result += 1;
	}

	return result;
}


void setQuality(Byte value)
{
	switch (value)
	{
	case 1: { D.superSample(0).edgeSoftenMode(EDGE_SOFTEN_NONE).shdMode(SHADOW_NONE).bumpMode(BUMP_FLAT)    .mtnMode(MOTION_NONE).ambMode(AMBIENT_FLAT) .volLight(false).tesselation(false).tesselationHeightmap(false).hdr(false).hpRt(false).texFilter(0) .texDetail(TEX_USE_DISABLE); break; }
	case 2: { D.superSample(1).edgeSoftenMode(EDGE_SOFTEN_NONE).shdMode(SHADOW_NONE).bumpMode(BUMP_FLAT)    .mtnMode(MOTION_NONE).ambMode(AMBIENT_LOW)  .volLight(false).tesselation(false).tesselationHeightmap(false).hdr(false).hpRt(false).texFilter(1) .texDetail(TEX_USE_DISABLE); break; }
	case 3: { D.superSample(2).edgeSoftenMode(EDGE_SOFTEN_NEAR).shdMode(SHADOW_MAP) .bumpMode(BUMP_NORMAL)  .mtnMode(MOTION_NONE).ambMode(AMBIENT_MED)  .volLight(false).tesselation(false).tesselationHeightmap(false).hdr(false).hpRt(false).texFilter(2) .texDetail(TEX_USE_SINGLE);  break; }
	case 4: { D.superSample(4).edgeSoftenMode(EDGE_SOFTEN_BOTH).shdMode(SHADOW_MAP) .bumpMode(BUMP_RELIEF)  .mtnMode(MOTION_LOW) .ambMode(AMBIENT_HIGH) .volLight(true) .tesselation(true) .tesselationHeightmap(false).hdr(true) .hpRt(true) .texFilter(8) .texDetail(TEX_USE_MULTI);   break; }
	case 5: { D.superSample(5).edgeSoftenMode(EDGE_SOFTEN_FXAA).shdMode(SHADOW_MAP) .bumpMode(BUMP_PARALLAX).mtnMode(MOTION_HIGH).ambMode(AMBIENT_ULTRA).volLight(true) .tesselation(true) .tesselationHeightmap(true) .hdr(true) .hpRt(true) .texFilter(16).texDetail(TEX_USE_MULTI);   break; }
	}
}
void spawnItems(Byte value)
{
	CChar8 *items[] =
	{
		"Obj/item/armor/armor.obj",
		"Obj/item/weapon/blunt/mace/0.obj",
		"Obj/item/misc/potions/hBig/bigHealthPotion.obj",
		"Obj/item/misc/potions/mBig/bigManaPotion.obj",
		"Obj/item/weapon/blunt/club/0.obj"
	};

	FREP(value)
	{
		Game::ObjParams obj = *Game::Objs.ptrRequire(items[Random(Elms(items))]);
		Game::World.objCreate(obj, Matrix(obj.scale(), Players[0].pos() + Vec(3, 1, 3)));
	}
}

void setViewRange(Flt value)
{
	D.viewRange(value);
}
/******************************************************************************/
AI* getChrByID(Int id)
{
	if(Chrs.elms())
	{
		FREPA(Chrs)
		{
			if(Chrs[i].uniqueID == id)
			{
				return &Chrs[i];
			}
		}
	}

	return NULL;
}
/******************************************************************************/
void AUp(Ptr)
{
	userAlpha += 0.1f;
}
void ADn(Ptr)
{
	userAlpha -= 0.1f;
}

	Flt wcrectw = 0.5;
   Flt y=-0.07, // starting position
       hX= 0.07, // element height
       s= 0.11; // spacing between elements

   Flt xl=wcrectw*0.25, // left  x position
       xr=wcrectw*0.75, // right x position
       wX =wcrectw*0.40, // element width
		 xl2=xl+wcrectw,
		 xr2=xr+wcrectw;
   void initMods()
   {
	   if(Mods.elms())
		   FREPA(Mods)
	   {
		   Mods[i].LuaBind();
		   actModCheck = &Mods[i];
		   Mods[i].onInit();
		   actModCheck = NULL;
	   }

	   modsInitialized = true;
   }

   void InitConsoleCommands();

   Bool InitGame()
{
	//SZADERY
	CompileShaders();

	

	/*Gui +=*/ w.create(Rect_C(-0.458,0.147,1.1,0.7), "Color shader params");

	w+=aUp.create(Rect_C(xl,y,wX,hX), "Alpha +").func(AUp);
	w+=aDn.create(Rect_C(xr,y,wX,hX), "Alpha -").func(ADn); y-=s;

	//RESZTA
   modsInitialized = false;
   
   Game::World.init();

   Game::World.setObjType(Statics,    OBJ_STATIC)
			.setObjType(Trees,      OBJ_TREE)
			.setObjType(Players,    OBJ_PLAYER)
			.setObjType(Items,      OBJ_ITEM)
			.setObjType(Chrs,       OBJ_CHR)
			.setObjType(Fireballs,  OBJ_FIREBALL)
			.setObjType(Teleports1, OBJ_TELEPORT1)
			.setObjType(Horses,     OBJ_HORSE)
			.setObjType(Destructs,  OBJ_DESTRUCTIBLE)
			.setObjType(Triggers,   OBJ_TRIGGER)
			.setObjType(Sounds3D,	OBJ_3D_SOUND)
			.setObjType(Doors,		OBJ_DOOR)
			.setObjType(Particless, OBJ_PARTICLES)
			.setObjType(Lights,		OBJ_LIGHT_POINT)
			.setObjType(Torchieres, OBJ_TORCHIERE);

   //Game::World.New("World/hapgame.world");
   SG.New("World/mainWorld.world");
	

 Game::ObjParamsPtr playerCharacter; playerCharacter.require("Obj/Chr/Warrior/0.obj");

 if(!playerCharacter)Exit("plrChr");

 if(Game::Waypoint *waypoint=Game::World.getWaypoint("playerStart"))
 {

	 if(waypoint->points.elms())
	 {
		 if(Players.elms())
		 {
			 REPA(Players)
			 Players.removeValid(i);
		 }

		 Game::ObjParams obj;
		 obj.base(playerCharacter);
		 obj.type(true,"OBJ_PLAYER");
		 Game::World.objCreate(obj,Matrix().setScalePos(obj.scale(),waypoint->points[0].pos));
		 
		 Cam.setPosDir(waypoint->pos(0));

	 }
	 else LogN("Waypoint \"playerStart\" has 0 points");
 }
 else LogN("Nie znaleziono waypointa \"playerStart\"");

  //if(!Players.elms())Exit("Players.elms is 0 (after Loader.start()");
	//plrPointer = &Players[0];
   
 InitConsoleCommands();


   weather = new _Weather();
   weather->create(); //create wywoluje sie teraz w konstruktorze
   weather->init();

   Water.images(Images("water/0.gfx"),Images("water/0.n.gfx"),Images("water/reflection.gfx")); // set water from textures 
   Water.draw      =true; // enable drawing
   Water.wave_scale = 0.7;

   Water.scale_diffuse = 150;
   Water.specular = 0.35;
   Water.reflect = 0.5;
   Water.density = 0.3;

   Water.plane.pos = Vec(0, -0.805, 0);

   InvGui.create();
   LootGui.create();
   Storage.create();
   MerchantGui.create();
   PauseGui.create();
   StatsGui.create();
   QuestGui.create();
   Forces.create();
   //Shortcuts.create();
	PauseGui.window.visible(false);
   setMouseVisibility();
	
	//loading health and mana progress bars
	if(guiObjs.load("Gui/Obj/main.gobj"))
   {
		if(!CameraMode)
		{
      	Gui += guiObjs;
		}
		proHealth = &guiObjs.getProgress("health");
		proStrong = &guiObjs.getProgress("strong");
		tmana = &guiObjs.getText("t_sil");
		theal = &guiObjs.getText("t_heal");
    }

	FREP(AIMT_ENUM_ELEMENTS)
	{
		monsterKills[i] = 0;
	}

	Dialogue.create();

	QuestGui.setGui();

	Ms.hide();

	//AI Progress create
	Gui += aiProHealth.create(Rect_C(0, 0.8, 0.8, 0.05)*0.6, PROGRESS_PERCENT);
	aiProHealth.set(1);
	aiProHealth.visible(false);

	if(!loadNewGame && !Contains(fileToLoad, ""))
	{
		SG.load(fileToLoad);
		loadNewGame = true;
	}

	AddMessage("U¿yj klawiszy WASD, aby chodziæ", 6);
	AddMessage("U¿yj myszy, aby siê rozgl¹daæ", 6);

	AddMessage("IdŸ porozmawiaæ z marynarzem", 4);
	
	AddMessage("Wyceluj w postaæ i wciœnij F, aby rozmawiaæ", 5);

    return true;
}
void ShutGame()
{
	if(Mods.elms())
		FREPA(Mods)
		{
			actModCheck = &Mods[i];
			Mods[i].onShut();
			actModCheck = NULL;
		}

	InvGui.window.visible(false);
	PauseGui.window.visible(false);
	StatsGui.window.visible(false);
	LootGui.L_window.visible(false);
	Storage.L_window.visible(false);
	MerchantGui.window.visible(false);

	Game::World.del();
	Dialogue.deinit();
	guiObjs.del();
	Messages.del();
	Mods.del();
	Quests.del();
	//questGuiObjs.del();
	QuestGui.questWindow.visible(false);
	Forces.del();
	//Shortcuts.del();

	scriptVariables.del();

	aiProHealth.del();

	weather->deleteweather();

	Ms.visible(true);
}
/******************************************************************************/
void UpdateGuiBars()
{
	if(Players.elms())
	{
		h = Players[0].health;
		mana = Players[0].mana;
	}
	if(h == 0) mana = 0;

	strongress = mana / 100;
	proStrong->set(strongress);
	progress = h / 100;
    proHealth->set(progress);
	tmana->set(S + (int)mana).asText();
	theal->set(S + (int)h).asText();
}

void updateChrIdleTexts()
{
	FREPA(Chrs)
	{
		if(Chrs[i].alive() && (Chrs[i].aiType != AI_MONSTER_1) && Players.elms())
		{
			//drawTds.reset();

			//Chrs[i].timeToEnd -= Time.d();
			Chrs[i].timeToSay -= Time.d();

			if(Dist(Chrs[i].pos(), Players[0].pos()) <= 10)
			{

				if(!Chrs[i].saySound.playing() && Chrs[i].timeToSay <= 0)
					Chrs[i].saySound.play(Chrs[i].textToSay, Chrs[i].pos(), 2, false, 1, VOLUME_VOICE);
			}

			if(Chrs[i].timeToSay <= 0)
			{
				Chrs[i].timeToSay = RandomF(11, 25);
				// Chrs[i].timeToEnd = 3;

				scriptPointer = &Chrs[i];

				lua_State* L;
				L = lua_open();
				luaL_openlibs(L);

				//Defining Lua functions
				SF_registerAllFunctions(L);
				RegisterLuaBind(L);
				RegisterWorldGlobals(L);
				luaL_dofile(L, "Scripts/texts.gs");

				lua_getglobal(L, "main");
				lua_call(L, 0, 1);

				Chrs[i].textToSay = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_close(L);

				scriptPointer = NULL;
			}
		}
	}
}

Bool UpdateKeyboardActions()
{
	if(Kb.b(KB_NP2)) x += 0.1f;
	if(Kb.b(KB_NP1)) x -= 0.1f;


	if(Kb.bp(KB_ESC) || Ps3.Btn(BTN_PLAYSTATION, false))
	{
		PauseGui.toggle();

		Time.speed((float)!PauseGui.window.visible());
	}
	if(Kb.bp(KB_F2))  SG.save("Saves/quicksave.sav"); //Game::World.save("Saves/quicksave.sav", Save);
	if(Kb.bp(KB_F3))  SG.load("Saves/quicksave.sav"); //Game::World.load("Saves/quicksave.sav", Load);

	if(Kb.bp(KB_F10)) Renderer.screenShots("Screenshot/", "jpg");

	if(Kb.bp(KB_I))   StatsGui.toggle();
	if(Kb.bp(KB_Z))   QuestGui.toggle();

	if(Kb.bp(KB_1))   Spell = SPELL_HEALTH;
	if(Kb.bp(KB_2))   Spell = SPELL_HEALTH_SELF;
	if(Kb.bp(KB_3))   Spell = SPELL_ATTACK;
	if(Kb.bp(KB_4))	Spell = SPELL_AVADA_KEDAVRA;

	if(Kb.bp(KB_V))
	{
		if(Players[0].isWanted)
		{
			Players[0].timeToGiveUp = 3;
			Players[0].giveUp.set(Players[0].cskel, "anim/give-up.anim");
		}

	}
	if(Game::World.updated())
	if(Players[0].timeToGiveUp < 0)
	{
		REPA(Chrs)if(Chrs[i].target.valid() && Chrs[i].target == Players[0])Chrs[i].target.clear();

		Players[0].isWanted=false;
		Players[0].timeToGiveUp = 4;
		Players[0].pos(Game::World.getWaypoint("TELEPORT_ID20")->points[0].pos);
	}

	//ToggleCameraMode
	CameraMode ^= Kb.bp(KB_NP3);

	// update camera
	if(CameraMode)
	{
		if(Kb.b(KB_A    ))Cam.matrix.pos-=Cam.matrix.x*Time.d()*2.5;
		if(Kb.b(KB_D    ))Cam.matrix.pos+=Cam.matrix.x*Time.d()*2.5;
		if(Kb.b(KB_W    ))Cam.matrix.pos+=Cam.matrix.z*Time.d()*2.5;
		if(Kb.b(KB_S    ))Cam.matrix.pos-=Cam.matrix.z*Time.d()*2.5;
		if(Kb.b(KB_SPACE))Cam.matrix.pos+=Cam.matrix.y*Time.d()*2.5;
		if(Kb.b(KB_LCTRL))Cam.matrix.pos-=Cam.matrix.y*Time.d()*2.5;
		if(Ms.hidden() && Ms.b(1))
		{
			Cam.yaw  -=Ms.d().x;
			Cam.pitch+=Ms.d().y;
		}
		Cam.setAngle(Cam.matrix.pos,Cam.yaw,Cam.pitch).updateVelocities().set();
	}

	if(Kb.bp(KB_H))objectiveMarker.Set(Vec4(0, 0.6, 1, 1),Vec(1562, 14, 1582), true);
	/*
	if(Kb.bp(KB_H))
	{
	if(Chrs.elms())
	{
	getChrByID(0)->enemy = getChrByID(1);
	getChrByID(1)->enemy = getChrByID(0);

	//getChrByID(0)->commandSet = false;
	//getChrByID(1)->commandSet = false;

	getChrByID(0)->wantToUpdateCommand = true;
	getChrByID(1)->wantToUpdateCommand = true;
	}
	}*/
	return true;
}

Bool UpdateGame()
{
   /*if(newGame == true)
   {
	    lua_State* L;

		SF_registerAllFunctions(L);
	    luaL_dofile(L, "Scripts/start_main_1.gs");

		lua_getglobal(L, "main");
		lua_call(L, 0, 0);

		lua_close(L);

		newGame = false;
   }*/

	if(Loader.update()) return true;

	updateChrIdleTexts();


	UpdateKeyboardActions();

	

   SG.update();
   

   objectiveMarker.Update();

   Gui.update();
   
   DetectTeleport();
   UpdateMessages();
   Dialogue.update();
   UpdateGuiBars();
   QuestGui.update();
   Forces.update();
   console.update();

   if(Quests.elms())
   {
		REPA(Quests)
		{
			if(Quests[i].toRemove == true)
			{
				Quests.remove(i, true);
				QuestGui.setGui();
			}
			else if(Quests[i].initQuest == true)
			{
				actQuestCheck = &Quests[i];
				Quests[i].loadScript((Char *)Quests[i].pathToQuestFile());
				actQuestCheck = NULL;
			}
			else
			{
				actQuestCheck = &Quests[i];
				Quests[i].completed();
				actQuestCheck = NULL;
			}
		}
   }

   if(LootGui.L_window.visible()) InvGui.window.visible(true);

   aiProHealth.visible(false);

   if(modsInitialized)
   {
	   if(Mods.elms())
			FREPA(Mods)
		    {
				actModCheck = &Mods[i];
				Mods[i].onUpdate();
				actModCheck = NULL;
		    }
   }

   setMouseVisibility();

	SPSet("alfa", userAlpha);

	if(!Game::World.updated()) // if current world hasn't yet been updated, it means it's a new or load game, so let background loader handle it
	{
		Loader.start();
	}
	else
	{
		Game::World.update(Cam.at);
		getObjectUnderCursor();
		//Camera
		if(!CameraMode)
		{
			cam.changeView();
			cam.updateCam();
		}
		if(!Players.elms())LogN("Players.elms is 0"); 
		weather->update(Time.time());

		Water.update(Vec2(0.01,0.01));

		Listener.pos(Cam.matrix.pos).orn(Cam.matrix.z, Cam.matrix.y);

	}

   return true;
}
void DrawMiniMap(C Rect &rect, C Vec2 &world_center_position, Flt radius)
{
	Vec2 world_min=world_center_position-radius, // minimum coordinates of world position drawn on the mini map
		world_max=world_center_position+radius; // maximum coordinates of world position drawn on the mini map

	Vec2 image_min   =Game::World.mini_map.worldToImage(world_min),
		image_max   =Game::World.mini_map.worldToImage(world_max),
		image_center=Game::World.mini_map.worldToImage(world_center_position);

	VecI2 image_mini=Floor(image_min), // minimum indexes of used image maps
		image_maxi=Floor(image_max); // maximum indexes of used image maps

	Vec2 image_size=rect.size()/(image_max-image_min); // size of a single map image on the screen

	// iterate through all image maps to be displayed
	for(Int y=image_mini.y; y<=image_maxi.y; y++)
		for(Int x=image_mini.x; x<=image_maxi.x; x++)
		{
			// calculate screen position of the (x,y) image map
			Vec2 image_pos=rect.center() - image_center*image_size;
			image_pos.x+=x*image_size.x;
			image_pos.y+=y*image_size.y;

			// prepare the screen rectangle of the (x,y) image map
			Rect_LD map_image_rect(image_pos,image_size.x,image_size.y);

			// draw the image
			Image &image=Game::World.mini_map(VecI2(x,y)); // access the image map from the world manager
			if(    image.is())image.drawMask(WHITE, Color(0,0,0,1), map_image_rect, *Images("gfx/mini map alpha.gfx"), rect); // if the image map is valid then draw it on the screen using masked drawing
		}

		rect.center().draw(WHITE);
}
void DrawSpellIcon()
{
   CChar8 *icon = NULL;

   switch(Spell)
   {
      case SPELL_HEALTH_SELF : icon = "gfx/spell/heal.gfx"; break;
      case SPELL_HEALTH	: icon = "gfx/spell/healthBall.gfx"; break;
      case SPELL_ATTACK	: icon = "gfx/spell/fireBall.gfx"; break;
	  case SPELL_USE_FORCE : icon = "gfx/spell/force.gfx"; break;
	  case SPELL_AVADA_KEDAVRA: icon = "gfx/spell/skull.gfx"; break;
   }

   if(icon) Images(icon)->drawFit(Rect_RD(D.w() - 0.05f, -D.h() + 0.05f, 0.2f, 0.2f));
}

void DetectTeleport()
{
   if(Players.elms() && Teleports1.elms())
   {
	   FREPA(Teleports1)
	   {
		  if(Dist(Players[0].pos(), Teleports1[i].pos()) <= 2)
		  {
			 if(Teleports1[i].target_world.is() && Teleports1[i].target_waypoint.is())
			 {
				 SG.requestChangeWorld(Teleports1[i].target_world, Teleports1[i].target_waypoint);
			 }
		  }
	   }
   }
}
static void DrawCrosshair()
{
	Vec2 offset = offset.zero();
	

	//if(Lit)
		//PosToScreen(Lit->pos(), offset);

   //if(View != VIEW_ISO)
   //{
      VI.line(Color(255, 255, 255, 20), Color(255, 255, 255, 50), offset+Vec2(-0.03f, 0.00f), offset+Vec2(0));
      VI.line(Color(255, 255, 255, 20), Color(255, 255, 255, 50), offset+Vec2( 0.03f, 0.00f), offset+Vec2(0));
      VI.line(Color(255, 255, 255, 20), Color(255, 255, 255, 50), offset+Vec2( 0.00f,-0.03f), offset+Vec2(0));
      VI.line(Color(255, 255, 255, 20), Color(255, 255, 255, 50), offset+Vec2( 0.00f, 0.03f), offset+Vec2(0));
      VI.end();
   //}
}
static void DrawHud()
{
	  if (displayFPS) D.text(-1.2f, 0.9f, S + Time.fps());

   if(Players.elms())
	{
		TextDS tds;
	  
	  tds.font = defaultFont;
	  tds.align.set(0, 1);

	  if(Time.speed() == 0)D.text(0,0, "PAUZA");

	  D.text(tds, -1.333 + (tds.textWidth(S + "Drewno: " + Players[0].wood) * 0.5) + 0.05, -D.h() + 0.33, S + "Drewno: " + Players[0].wood);
	  D.text(tds, -1.333 + (tds.textWidth(S + "Kamieñ: " + Players[0].stone) * 0.5) + 0.05, -D.h() + 0.26, S + "Kamieñ: " + Players[0].stone);
	  D.text(tds, -1.333 + (tds.textWidth(S + "¯elazo: " + Players[0].iron) * 0.5) + 0.05, -D.h() + 0.19, S + "¯elazo: " + Players[0].iron);

	  tds.align.set(1, 0);
	  D.text(tds, -1.333 + (tds.textWidth(S + "Godzina " + ((int)time.getHours()) + ":" + (int)time.getMinutes())), 0.9, S + "Godzina " + ((int)time.getHours()) + ":" + ((int)time.getMinutes()));
	  //D.text(tds, -1.333 + (tds.textWidth(S + "Godzina: " + (Byte)gameHour) * 0.5) + 0.05, D.h() -0.05, S + "Godzina: " + (Byte)gameHour);
   }
}

void Render()
{
	Game::World.draw();

	switch(Renderer())
	{
	case RM_BLEND:
		objectiveMarker.drawDecal();
		break;
	}
}
void DrawGame()
{
	if(Loader.draw()) return;

	Renderer(Render);

	if(drawPhys)Physics.draw();

	objectiveMarker.Draw();

	if(!modsInitialized)
	{
		initMods();
		//objectiveMarker.Set(Vec4(0, 1, 0, 1), getChrByID(666112997)->pos());
	}

	SPSet("CustomColor", color1);
	Shaders("User/ColourShader")->getTech("CustomTechnique")->draw(NULL,&Rect(-D.w(),-D.h(),x,D.h()));
	SPSet("CustomColor", color2);
	Shaders("User/ColourShader")->getTech("CustomTechnique")->draw(NULL,&Rect(x,-D.h(),D.w(),D.h()));


	TextDS drawTds;

	if(!CameraMode)
	{

		Vec2 pos;


		if(!Ms.visible()) DrawCrosshair();
		DrawHud();


		TextDS nameTDS, tekstDS;

		REPA(Items)
		{
			if(Players[0].inRange(Items[i], 6))
			if(PosToScreen(Items[i].pos(), pos)){
				

				if(Item* item = CAST(Item, Lit))if(item==&Items[i])
					tekstDS.color.b = sin(Time.time()*2)*255;

				D.text(tekstDS, pos, S + Items[i].name);
			}
		}

		/*
		//Draw name of selected object
				if(Item *selectedItem = CAST(Item, Lit))
				{
					//if(selectedItem->hasOwner == false)
					//{
					D.text(pos, S + selectedItem->name);
					//}
				}
				else */
		pos = pos.zero();

		REPA(Chrs)
		{
			if(Players[0].inAttackRange(Chrs[i], 6))if(PosToScreen(Chrs[i].pos(), pos))
			{
				if(Chrs[i].target.valid())
				{
					Reference<Chr> plr = &Players[0];
					if(Chrs[i].target == plr)
					{
						nameTDS.color = RED;
					}
				}
				else if(Chrs[i].aiClassType == AIC_MONSTER)
				{
					nameTDS.color = RED;
				}
				else
				{
					nameTDS.color = GREEN;
				}
				nameTDS.font=defaultFont;

				if(AI* chr = CAST(AI, Lit))if(chr == &Chrs[i])
					nameTDS.color.a = sin(RadToDeg(StateActive->time()))*255;
				//Vec2 screen = PosToScreen(selectedAI->pos() + Vec(0, selectedAI->mesh->box.h(), 0));

				D.text(nameTDS, pos, S + Chrs[i].name/* + " Dist: " + Dist(selectedAI->pos(), Players[0].pos())*/);
				aiProHealth.visible(true);
				aiProHealth.pos(pos+Vec2(-(aiProHealth.size().x/2),0.1));
				aiProHealth.set(Chrs[i].health, 100);
			}
		}

/*
		if(AI *selectedAI = CAST(AI, Lit))
		{
			if(selectedAI->target.valid())
			{
				Reference<Chr> plr = &Players[0];
				if(selectedAI->target == plr)
				{
					nameTDS.color = RED;
				}
			}
			else if(selectedAI->aiType == (AI_MONSTER_1))
			{
				nameTDS.color = RED;
			}
			else
			{
				nameTDS.color = GREEN;
			}
			nameTDS.font=defaultFont;
			//Vec2 screen = PosToScreen(selectedAI->pos() + Vec(0, selectedAI->mesh->box.h(), 0));

			D.text(nameTDS, pos, S + selectedAI->name/ * + " Dist: " + Dist(selectedAI->pos(), Players[0].pos())* /);
			aiProHealth.visible(true);
			aiProHealth.pos(pos+Vec2(-(aiProHealth.size().x/2),0.1));
			aiProHealth.set(selectedAI->health, 100);
		}
		*/		
		if(Players[0].isWanted)
		{
			D.text(0, -0.7, S+"Jesteœ poszukiwany, naciœnij V aby siê poddaæ.");
			D.text(0, -0.8, S+"Masz " + Round(Players[0].timeToEndWanted) + "sekund.");
		}

		DrawMiniMap(Rect_RU(D.w(),D.h(),0.8f,0.8f).extend(-0.05f), Players[0].pos().xz(), 35);

		if(!CameraMode)
			Gui.draw();

		InvGui.draw();
		LootGui.draw();
		Storage.draw();
		MerchantGui.draw();
		DrawMessages();
		DrawSpellIcon();
		//Forces.draw();
		console.draw();

		if(modsInitialized)
		if(Mods.elms())
			FREPA(Mods)
		{
			actModCheck = &Mods[i];
			Mods[i].onDraw();
			actModCheck = NULL;
		}

	}
	else
	{
		Gui.draw();
		InvGui.draw();
		LootGui.draw();
		Storage.draw();
		MerchantGui.draw();
	}
}

void InitConsoleCommands()
{
	console.init();
	console.setCommand("cg_displayFPS", displayFPS);
	console.setCommand("cg_setQuality", setQuality, 1, 5);
	console.setCommand("cg_wireframe", Renderer.wire);
//	console.setCommand("cg_viewRange", setViewRange, 50, 1000);

	console.setCommand("gm_drawPhysics", drawPhys);
	console.setCommand("gm_cameraTpp", View, 0, 1);
	console.setCommand("gm_spawnItems", spawnItems, 1, 10);
	console.setCommand("gm_freeCamera", CameraMode);
}

/******************************************************************************/
State StateGame(UpdateGame, DrawGame, InitGame, ShutGame);
/******************************************************************************/
