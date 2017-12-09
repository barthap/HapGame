/******************************************************************************/
struct Teleport1 : Game::ObjParticles
{
   Str target_world;
   Str target_waypoint;

   virtual void create(Game::ObjParams &obj);

   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
