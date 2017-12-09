/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
SaveGame SG;
/******************************************************************************/
void SaveGame::WorldState::save(File &f)
{
   //Save world name
   f.putStr(world_dir);
   
   //Save world state
   world_state.pos(0);
   f.putUInt(world_state.size());
   world_state.copy(f);
}
/******************************************************************************/
Bool SaveGame::WorldState::load(File &f)
{
   //Get world name
   f.getStr(world_dir);

   //Load world state
   world_state.writeMem();
   f.copy(world_state, 0, NULL, f.getUInt());

   return true;
}
/******************************************************************************/
// GET
/******************************************************************************/
SaveGame::WorldState* SaveGame::findWorldState(Str world_dir)
{
   REPA(world_states)
      if(EqualPath(world_states[i].world_dir, world_dir))
         return &world_states[i];

   return NULL;
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void SaveGame::del()
{
   requestClear();
   current_world.clear();
   world_states.del();
   Game::World.del();
}
/******************************************************************************/
void SaveGame::New(Str world_dir)
{
   del();

   Game::World.New(world_dir);
   current_world = world_dir;

   worldChanged();
}
/******************************************************************************/
void SaveGame::worldChanged()
{
   //SetEnvironment(Indoors()); //Set environment settings
}
/******************************************************************************/
void SaveGame::saveCurrentWorld()
{
   Str world_dir = Game::World.worldDir();
   if(world_dir.is())
   {
      WorldState *ws = findWorldState(world_dir);

      if(!ws)
      {
         ws = &world_states.New();
         ws->world_dir = world_dir;
      }

      ws->world_state.writeMem();
      Game::World.save(ws->world_state);
   }
}
/******************************************************************************/
void SaveGame::changeWorld(Str world_dir, Str waypoint_name)
{
   Flt plr_height;
   Vec old_pos;

   File player_data;

   player_data.writeMem();

   if(Players.elms())
   {
      old_pos = Players[0].pos();
      plr_height = Players[0].ctrl.height();
      Players[0].save(player_data);

      Players.removeValid(0);
   }

   saveCurrentWorld();

   current_world = world_dir;
   if(WorldState *ws = findWorldState(world_dir))
   {
      ws->world_state.pos(0);
      Game::World.load(ws->world_state);
   }
   else
   {
      Game::World.New(world_dir);
   }

   if(player_data.size())
   {
      Bool reposition = false;
      Vec new_pos;

      if(Game::Waypoint *waypoint = Game::World.findWaypoint(S + "TELEPORT_" + waypoint_name))
         if(waypoint->points.elms())
		 {
			 reposition = true;
			 new_pos = waypoint->points[0].pos;
			 new_pos.y += plr_height / 2;
			 Cam += new_pos - old_pos;
		 }

      player_data.pos(0);
      Game::World.objInject(OBJ_PLAYER, player_data, reposition ? &new_pos : NULL);
   }

   worldChanged();
}
/******************************************************************************/
void SaveGame::requestChangeWorld(Str world_dir, Str waypoint_name)
{
   T.request_world_dir = world_dir;
   T.request_waypoint_name = waypoint_name;
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void SaveGame::update()
{
   if(request_world_dir.is())
   {
      changeWorld(request_world_dir, request_waypoint_name);
      requestClear();
   }
}
/******************************************************************************/
// IO
/******************************************************************************/
Bool SaveGame::save(Str name)
{
   File f;
   
   if(f.writeTry(name))
   {
      saveCurrentWorld();

      f << Cam << View;

	  FREP(AIMT_ENUM_ELEMENTS)
	  {
		  f.putInt(monsterKills[i]);
	  }

	  //Time
	  //time.saveTime(f);

	  //Quests
	  Quests.save(f);

	  //Script variables
	  scriptVariables.save(f);

      f.putStr(current_world);

      world_states.save(f);

      return true;
   }

   return false;
}
/******************************************************************************/
Bool SaveGame::load(Str name)
{
   File f;
   
   if(f.readTry(name))
   {
      f >> Cam >> View;

	  FREP(AIMT_ENUM_ELEMENTS)
	  {
		  monsterKills[i] = f.getInt();
	  }

	  //Time
	  //if(!time.loadTime(f)) return false;
	
	  //Quests
	  Quests.clear();
	  QuestGui.setGui();
      if(!Quests.load(f)) return false;
	  QuestGui.setGui();

	  //Script variables
	  if(!scriptVariables.load(f)) return false;

      f.getStr(current_world);
      world_states.load(f);

      //Load current world
      if(WorldState *ws = findWorldState(current_world))
      {
         ws->world_state.pos(0);
         Game::World.load(ws->world_state);
         worldChanged();
         setMouseVisibility();
      }

      return true;
   }

   return false;
}
/******************************************************************************/
