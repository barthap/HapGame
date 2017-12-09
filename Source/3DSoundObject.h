
struct Sound3D : Game::Obj
{

   Vec     position; // the class will contain only position
   Sound sound;

   Str path;
   Flt range;
   Flt volume;

   // provide necessary methods required by Game::Obj :
      virtual void create(Game::ObjParams &obj); // extend default creation

      virtual Vec		pos(          )			{return position    ;} // get position
      virtual void		pos(C Vec &pos)			{     T.position=pos;} // set position
      virtual Matrix	matrix(                ){return position           ;} // get matrix
      virtual void		matrix(C Matrix &matrix){     T.position=matrix.pos;} // set matrix

      virtual Bool update     ();// object update
      virtual UInt drawPrepare();               // object draw prepare

   // constructor
   Sound3D();
};
/******************************************************************************/
