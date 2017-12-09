/******************************************************************************/
struct AI : Chr
{
   Bool isTalking;

   Vec ctrl_pos;

   Sound saySound;
   Str textToSay;
   Flt timeToSay;

   Str dialoguePath;

   Byte aiClassType;
   Int  routineWaypoint, rW;
   Int  actRoutinePoint;

   Bool attackOnAllMap,
		cantAttack;

	Flt time_to_random_move,
        time_to_sense,
        time_to_update_action,
        time_to_attack;

	Flt timeToRespawn;
	Bool wantToRespawn;

//	Flt strength; //attack strenght
	Flt time_to_walk;

	//Guard 2 and monsters
	Vec startPos;
	Vec2 startAngle;

	//Dependence thinking
	Flt commanderID;
	Reference<Chr> enemy;

	//Army
	Str command;
	Flt timeToUpdateCommand;
	Bool wantToUpdateCommand;
	Bool commandSet;

	//Merchant
	Bool nowTrading;

   Item *LootItem;

   Memb<Motion>   animations; // container for motion animations
   Reference<Chr> target;         // target enemy

   //Get / Set
   virtual Vec    pos   (              ){ return __super::pos   (      ); }
   virtual Matrix matrix(              ){ return __super::matrix(      ); }
   virtual void   pos   (Vec    &pos   ){        __super::pos   (pos   ); ctrl_pos = ctrl.actor.pos(); }
   virtual void   matrix(Matrix &matrix){        __super::matrix(matrix); ctrl_pos = ctrl.actor.pos(); }

   Memx<Game::Obj>* itemContainer() { Memx<Game::Obj> &items = inv.items; return &items;} //Get item container, this is required by the engine to point to a valid item container for a character
   void itemRemoved(Game::Item &item){ inv.itemRemoved(item); } //This is called when an item is being removed from the character
   void itemRemoved() { inv.itemRemoved(); }                    //This is called when an item has been removed from the character
   void itemAdded(Game::Item &item) { inv.itemAdded(item); }    //This is called when an item has been added   to   the character

   virtual void create(Game::ObjParams &obj);

   //Operations
   virtual void attack (Chr &target);
   virtual void hit    ();
   virtual void die(Chr *src);
   virtual void animate();
   virtual Bool update ();

   virtual void SF_Attack(luabind::object target ); //funkcja Attack dla luabind

   bool callReinforcements(Chr &target, int needed=2);

   void waypointMove();

   void randomMove();

   void updateAiAttacks();

   virtual void addDamage(Flt damage, Chr* src);

   virtual UInt drawPrepare();
   virtual void drawShadow ();

   //void setGui() { LootGui.setGui(); }
   //IO

   void save(File &f);
   Bool load(File &f);
   virtual void linkReferences();

   void l_update(Game::Chr &owner);

   AI(); //Constructor
};

extern Int monsterKills[AIMT_ENUM_ELEMENTS];
/******************************************************************************/