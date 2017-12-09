/******************************************************************************/
STRUCT(Horse , Game::Obj)
//{
   MeshPtr        mesh ;
   CSkeleton      cskel;
   Controller     ctrl ;
   Ragdoll        kinematic_ragdoll;
   Reference<Chr> rider;
   Flt            angle,angle_offset;
   
   Flt  stand_run,turn;
   Vec2 dir;

   // manage
   virtual void create(Game::ObjParams &obj);
           void createKinematicRagdoll();

   // get / set
   virtual Vec    pos   (                );
   virtual Matrix matrix(                );
   virtual void   pos   (C Vec    &pos   );
   virtual void   matrix(C Matrix &matrix) {pos(matrix.pos);}

   // callbacks
   virtual void memoryAddressChanged();
   virtual void linkReferences();

   // update
   virtual Bool update();

   // operations
   virtual void emitSound(Str name, Flt range=1, Flt volume=1);

   // draw
   virtual UInt drawPrepare();
   virtual void drawShadow ();

   // enable / disable
   virtual void disable();
   virtual void  enable();

   // io
   virtual void save(File &f);
   virtual Bool load(File &f);

   Horse();
};
/******************************************************************************/
