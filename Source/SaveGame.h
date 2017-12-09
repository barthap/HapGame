/******************************************************************************/
struct SaveGame
{
   void del();
   void New(Str world_dir);

   void changeWorld(Str world_dir, Str waypoint_name);
   void requestChangeWorld(Str world_dir, Str waypoint_name);

   void update();

   Bool load(Str name);
   Bool save(Str name);

   struct WorldState
   {
      Str  world_dir;
      File world_state;

      Bool load(File &f);
      void save(File &f);
   };

   Str current_world;
   Memb<WorldState> world_states;

   Str request_world_dir;
   Str request_waypoint_name;

   WorldState* findWorldState(Str world_dir);

   void worldChanged();
   void saveCurrentWorld();
   void requestClear() { requestChangeWorld(CNULL, CNULL); }
};
extern SaveGame SG;
/******************************************************************************/
