/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
void Teleport1::create(Game::ObjParams &obj)
{
   __super::create(obj);
   
   target_world = obj.getParam("targetWorld").asText();
   target_waypoint = obj.getParam("targetWaypoint").asText();
}
/******************************************************************************/
void Teleport1::save(File &f)
{
   __super::save(f);

   f.putStr(target_world);
   f.putStr(target_waypoint);
}
Bool Teleport1::load(File &f)
{
   if(__super::load(f))
   {
      f.getStr(target_world);
	  f.getStr(target_waypoint);

      return true;
   }

   return false;
}
/******************************************************************************/
