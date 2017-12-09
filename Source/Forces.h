/******************************************************************************/
enum FORCES
{
	F_FORCE,
	F_TELEKINESIS,
	F_NUMBER_OF_FORCES,
};
/******************************************************************************/
struct _Forces
{
   bool enabled[F_NUMBER_OF_FORCES];

   Byte signedForce;

   //Telekinesis
   Reference<Chr> tObject;
   Vec2 posInPrevFrame;
   Flt distanceToObj;

   GuiObjs guiObjects;

   GuiImage *forceImages[F_NUMBER_OF_FORCES];

   //Create
   void create();
   void del();

   //Update
   Bool update();

   //Draw
   void draw();

   //Get
   Int nextEnabledForce();
   Int previousEnabledForce();
   Int enabledForces();

   //IO
   void save(File &f);
   Bool load(File &f);

   _Forces();
} extern Forces;
/******************************************************************************/