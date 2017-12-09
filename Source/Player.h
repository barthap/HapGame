/******************************************************************************/
/*extern Str AttackRightHand[];
extern Str CastSpellNormal[];
extern Str CastSpellSelf[];*/
static Str AttackRightHand[]=
{
   "anim/swing/r-l.anim" ,
   "anim/swing/r-r.anim" ,
   "anim/swing/r-ld.anim",
   "anim/swing/r-rd.anim",
};
static Str CastSpellNormal[]=
{
   "anim/cast-bolt.anim" ,
   "anim/cast-boltw.anim",
   "anim/cast-cncn.anim" ,
};
static Str CastSpellSelf[]=
{
   "anim/cast-self.anim" ,
};
/******************************************************************************/
struct Player : Chr
{
   //Parameters
   //Int strength;
   Int dexterity;
   Int intelligence;
   Int charisma;

   Int gold;

   Int wood;
   Int stone;
   Int iron;

   Flt timeToAction;
   Flt exp, lvl; //Experience & level

	Bool usedAvadaKedavra;
	Flt timeToAvadaKedavraEnd;
	Particles avadaKedavraParticle;

	Motion giveUp;

   Memx<Game::Obj>* itemContainer() { Memx<Game::Obj> &items = inv.items; return &items;} //Get item container, this is required by the engine to point to a valid item container for a character
   void itemRemoved(Game::Item &item){ inv.itemRemoved(item); } //This is called when an item is being removed from the character
   void itemRemoved() { inv.itemRemoved(); }                    //This is called when an item has been removed from the character
   void itemAdded(Game::Item &item) { inv.itemAdded(item); }    //This is called when an item has been added   to   the character

   void itemDestroy(Item &item);

   virtual void create(Game::ObjParams &obj);

   Vec ctrl_pos;

   virtual Vec    pos   (              ) { return __super::pos   (      ); }
   virtual Matrix matrix(              ) { return __super::matrix(      ); }
   virtual void   pos   (Vec    &pos   ) {        __super::pos   (pos   ); ctrl_pos = ctrl.actor.pos(); }
   virtual void   matrix(Matrix &matrix) {        __super::matrix(matrix); ctrl_pos = ctrl.actor.pos(); }

   virtual Bool update();
   virtual UInt drawPrepare();
   virtual void drawShadow ();
   virtual void animate();

   void updateSpells();

	//For attack
	Vec        weapon_start,weapon_end;		//weapon phys
	Tri        weapon_sweep[2];				//weapon phys
	Memc<Byte> collision;

	void Attack();		//attack function

//	Motion spellAnim;
	Memb<Motion>   animations;

   //IO
   void save(File &f);
   Bool load(File &f);

   //Loot
   Item *L_loot;
   void link(Item *item);

   //AI *Chr_loot;
   //void link2(AI *Chr_loot);
   void* getScriptPointer() {return dynamic_cast<void*>(this);}

   LOOTGUI *loot_gui;

   Player();
};
/******************************************************************************/
