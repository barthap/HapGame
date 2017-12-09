#include <EsenthelEngine/EsenthelEngine.h>
#include "F:\EsenthelEngineSDK\Data\Enum\_enums.h"

bool toKill = false;

Ptr mHwndMain=NULL;

void TransFormToChild(void     *Ptr,void *Parent);


/******************************************************************************/
// ITEM
/******************************************************************************/
struct Item : Game::Item // extend items
{
	
   Char name[64]; // add new parameter 'name'
   Int  value   ; // add new parameter 'value'

   virtual void create(Game::ObjParams &obj); // extend default creation

   // since new parameters are declared we need to properly initialize them in constructor, and save/load them in IO methods:

   // constructor
   Item();

   // io methods
   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
void Item::create(Game::ObjParams &obj)
{
   __super::create(obj); // default create

   // now setup custom parameters from 'obj'
   if(Param *par=obj.findParam("name" ))Set(name, par->asText());
   if(Param *par=obj.findParam("value"))    value=par->asInt() ;
}
/******************************************************************************/
Item::Item()
{
   name[0]=0;
   value=0;
}
/******************************************************************************/
void Item::save(File &f)
{
   __super::save(f); // default save

   f<<name<<value; // save custom parameters
}
Bool Item::load(File &f)
{
   if(__super::load(f)) // if default load was successful
   {
      f>>name>>value; // load custom parameters
      return true;    // return success
   }
   return false; // return failure
}
/******************************************************************************/
// MAIN
/******************************************************************************/
Game::ObjMemx<Item> Items;
/******************************************************************************/
void InitPre()
{
	
   App.name("Custom Parameters");
   App.flag=APP_NO_TITLE_BAR|APP_NO_CLOSE|APP_MINIMIZABLE|APP_RESIZABLE|APP_WORK_IN_BACKGROUND;
  
   DataPath("F:/Tools/EsenthelEngineSDK/Data");
   
   Paks.add("engine.pak");
   
 
   D.full(false).sync(true).ambPower(0.3).hpRt(true);
   
   //ViewportFull.range=50;
   Cam.at.set(16,0,16);
   Cam.dist = 10;
   Cam.pitch=-PI_3;
}
/******************************************************************************/
Bool Init()
{

 TransFormToChild( App.hwnd(),mHwndMain);
	  

   
 Text_ds.scale*=0.8;

   Physics.create(CSS_NONE, true, "PhysX");
   Sky    .atmospheric();
   Sun.image=Images("gfx/sky/sun.gfx"); Sun.light_color=1-D.ambColor();

   Game::World.init      (              )
              .setObjType(Items,OBJ_ITEM)
              .New       ("world/custom params.world");

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/

bool show = true;
Bool Update()
{
    if(toKill)
   {
	//   HideWindow(App.hwnd(),!show);
	   return false;
   }

   CamHandle(0.1,100,CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   Game::World.update(Cam.at);

   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   Renderer(Render);

   // draw item parameters
   REPA(Items)
   {
      Item  &item  =Items[i];                // get i-th Items
      Vec2   screen=PosToScreen(item.pos()); // convert world position to screen position
      D.text(screen,item.name);              // draw item's name
   }
}
/******************************************************************************/
