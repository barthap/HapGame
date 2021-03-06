/******************************************************************************/
enum FireBall_Type
{
	FB_DEFAULT,
	FB_FIRE_ATTACK,
	FB_FIRE_HEALTH,
	FB_F_USE_FORCE,
};

struct Fireball : Game::Obj
{
   Bool             exploded;
   Flt              power;
   Ball             ball;
   Vec              vel;
   Reference<Chr> caster;
   Particles        particles;

	Byte type;

   //Manage
   virtual void create(Game::ObjParams &obj);

   //Get / Set
   virtual Vec pos() { return ball.pos; }
   virtual Matrix matrix() { return Matrix(ball.pos); }
   virtual void pos(C Vec &pos) { ball.pos = pos; }
   virtual void matrix(C Matrix &matrix) { pos(matrix.pos); }

   //Update
   virtual Bool update();

   //Operations
   virtual void explode(Plane *plane);

   //Draw
   virtual UInt drawPrepare();
   virtual void drawPalette();
   virtual void drawBlend();

   //Enable / Disable
   virtual void disable();
   virtual void  enable();

   //IO
   virtual void save(File &f);
   virtual Bool load(File &f);

   virtual void linkReferences();

   Fireball();
};
/******************************************************************************/
