struct Trigger : Game::Obj
{
   Bool enabled;
   Str  script;
   Int  range;
   Int  id;

	Vec position;
	
	virtual Vec  pos(          ) {return position    ;} // get position
	virtual void pos(C Vec &pos) {     T.position=pos;} // set position

	virtual Matrix matrix(                ) {return position           ;} // get matrix
	virtual void   matrix(C Matrix &matrix) {     T.position=matrix.pos;} // set matrix

   virtual void create(Game::ObjParams &obj);

	virtual UInt drawPrepare()
	{
		return 0;
	}

   virtual Bool update();

   void save(File &f);
   Bool load(File &f);

   Trigger();
};